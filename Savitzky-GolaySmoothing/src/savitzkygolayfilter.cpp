/*
 * Performs Savitzky Golay Filtering
 *
 * Developed by Anubhav Rohatgi
 * Date 26/04/2016
 */

#include "savitzkygolayfilter.h"


void smoothSavGolFilter(const cv::Mat &src, cv::Mat &dst, const cv::Size &window_size, const int hor_degree, const int vert_degree)
{
    //Check for the conditions
    if(src.type() != CV_8UC1)
        throw std::invalid_argument("SmoothSavGolFilter: The input source type is invalid (!GrayScale)");

    if( hor_degree < 0 || vert_degree < 0)
        throw std::invalid_argument("SmoothSavGolFilter: invalid polynomial degree!");

    if(window_size.width > src.size().width || window_size.height > src.size().height)
        throw std::invalid_argument("SmoothSavGolFilter: invalid window size!");

    if(calcNumTerms(hor_degree,vert_degree) > (window_size.width* window_size.height))
            throw std::invalid_argument("SmoothSavGolFilter: Order is too big for chosen window");


    const int width = src.cols;
    const int height = src.rows;

    //kernel dimensions
    const int kw = window_size.width;
    const int kh = window_size.height;

    if(kw > width || kh > height){
        dst = src.clone();
        return;
    }

    /*
     * Consider a 5x5 kernel:
     * |x|x|T|x|x|
     * |x|x|T|x|x|
     * |L|L|C|R|R|
     * |x|x|B|x|x|
     * |x|x|B|x|x|
     */

    //Coordinates of central point C of the kernel
    const cv::Point k_center(kw/2,kh/2);

    cv::Point k_origin;

    //length of the top segment T
    const int k_top = k_center.y;

    //length of the bottom segment B
    const int k_bottom = kh - k_top -1;

    //length of the left segment L
    const int k_left = k_center.x;

    //length of the right segment R
    const int k_right = kw - k_left - 1;


    const uint8_t* src_data = src.data;
    const int src_bpl = src.step; //bytes per line

    dst = cv::Mat::zeros(src.size(),CV_8UC1);
    uint8_t* const dst_data = dst.data;
    int const dst_bpl = dst.step;


    // Top-left corner.
    uint8_t const* src_line = src_data;
    uint8_t* dst_line = dst_data;

    SavitzkyGolayFilter kernel(window_size, cv::Point(0, 0), hor_degree, vert_degree);
    for (int y = 0; y < k_top; ++y, dst_line += dst_bpl) {
       for (int x = 0; x < k_left; ++x) {
            k_origin = cv::Point(x,y);
            kernel.recalcForOrigin(k_origin);
            kernel.convolve(dst_line + x, src_line, src_bpl);
        }
    }

    // Top area between two corners.
   // k_origin.setX(k_center.x());
    src_line = src_data - k_left;
    dst_line = dst_data;
    for (int y = 0; y < k_top; ++y, dst_line += dst_bpl) {
        k_origin = cv::Point(k_center.x,y);
        kernel.recalcForOrigin(k_origin);
        for (int x = k_left; x < width - k_right; ++x) {
            kernel.convolve(dst_line + x, src_line + x, src_bpl);
        }
    }

    // Top-right corner.
    //k_origin.setY(0);
    src_line = src_data + width - kw;
    dst_line = dst_data;
    for (int y = 0; y < k_top; ++y, dst_line += dst_bpl) {
        k_origin = cv::Point((k_center.x + 1),0);
        for (int x = width - k_right; x < width; ++x) {
            kernel.recalcForOrigin(k_origin);
            kernel.convolve(dst_line + x, src_line, src_bpl);
            k_origin = cv::Point(k_origin.x + 1, k_origin.y);
        }
        k_origin = cv::Point(k_origin.x, k_origin.y + 1);
    }

    // Central area.
    // Take advantage of Savitzky-Golay filter being separable.
    SavitzkyGolayKernel const hor_kernel(
        cv::Size(window_size.width, 1),
        cv::Point(k_center.x, 0), hor_degree, 0
    );
    SavitzkyGolayKernel const vert_kernel(
        cv::Size(1, window_size.height),
        cv::Point(0, k_center.y), 0, vert_degree
    );

    int const shift = kw - 1;

    //Savitzky Golay Filter is linearly separable hence we
    //make use of this and split it into horizontal and vertical
    //directions
    int const temp_stride = (width - shift + 3) & ~3;
    AlignArray<float, 4> temp_array(temp_stride * height);


    // Horizontal pass.
    src_line = src_data - shift;
    float* temp_line = temp_array.data() - shift;
    for (int y = 0; y < height; ++y) {
        for (int i = shift; i < width; ++i) {
            float sum = 0.0f;

            uint8_t const* src = src_line + i;
            for (int j = 0; j < kw; ++j) {
                sum += src[j] * hor_kernel[j];
            }
            temp_line[i] = sum;
        }
        temp_line += temp_stride;
        src_line += src_bpl;
    }

    // Vertical pass.
    dst_line = dst_data + k_top * dst_bpl + k_left - shift;
    temp_line = temp_array.data() - shift;
    for (int y = k_top; y < height - k_bottom; ++y) {
        for (int i = shift; i < width; ++i) {
            float sum = 0.0f;

            float* tmp = temp_line + i;
            for (int j = 0; j < kh; ++j, tmp += temp_stride) {
                sum += *tmp * vert_kernel[j];
            }
            int const val = static_cast<int>(sum);
            dst_line[i] = static_cast<uint8_t>(MAX(0, MIN(val, 255)));
        }

        temp_line += temp_stride;
        dst_line += dst_bpl;
    }


    // Left area between two corners.
    //k_origin.setX(0);
    k_origin = cv::Point(0,(k_center.y + 1));
    for (int x = 0; x < k_left; ++x) {
        src_line = src_data;
        dst_line = dst_data + dst_bpl * k_top;

        kernel.recalcForOrigin(k_origin);
        for (int y = k_top; y < height - k_bottom; ++y) {
            kernel.convolve(dst_line + x, src_line, src_bpl);
            src_line += src_bpl;
            dst_line += dst_bpl;
        }
        k_origin = cv::Point(k_origin.x + 1,k_origin.y);
    }

    // Right area between two corners.
    k_origin = cv::Point(k_center.x + 1, k_center.y);
    //k_origin.setY(k_center.y());
    for (int x = width - k_right; x < width; ++x) {
        src_line = src_data + width - kw;
        dst_line = dst_data + dst_bpl * k_top;

        kernel.recalcForOrigin(k_origin);
        for (int y = k_top; y < height - k_bottom; ++y) {
            kernel.convolve(dst_line + x, src_line, src_bpl);
            src_line += src_bpl;
            dst_line += dst_bpl;
        }
        k_origin = cv::Point(k_origin.x + 1,k_origin.y);
    }

    // Bottom-left corner.
    //k_origin.setY(k_center.y() + 1);
    k_origin = cv::Point(k_origin.x,k_center.y + 1);
    src_line = src_data + src_bpl * (height - kh);
    dst_line = dst_data + dst_bpl * (height - k_bottom);
    for (int y = height - k_bottom; y < height; ++y, dst_line += dst_bpl) {
        for (int x = 0; x < k_left; ++x) {
            k_origin = cv::Point(x,k_origin.y);
            kernel.recalcForOrigin(k_origin);
            kernel.convolve(dst_line + x, src_line, src_bpl);
        }
        k_origin = cv::Point(k_origin.x,k_origin.y + 1);
    }

    // Bottom area between two corners.
    k_origin = cv::Point(k_center.x,k_center.y + 1);
    src_line = src_data + src_bpl * (height - kh) - k_left;
    dst_line = dst_data + dst_bpl * (height - k_bottom);
    for (int y = height - k_bottom; y < height; ++y, dst_line += dst_bpl) {
        kernel.recalcForOrigin(k_origin);
        for (int x = k_left; x < width - k_right; ++x) {
            kernel.convolve(dst_line + x, src_line + x, src_bpl);
        }
        k_origin = cv::Point(k_origin.x, k_origin.y + 1);
    }

    // Bottom-right corner.
    //k_origin.setY(k_center.y() + 1);
    k_origin = cv::Point(k_origin.x,k_center.y + 1);
    src_line = src_data + src_bpl * (height - kh) + (width - kw);
    dst_line = dst_data + dst_bpl * (height - k_bottom);
    for (int y = height - k_bottom; y < height; ++y, dst_line += dst_bpl) {
        k_origin = cv::Point(k_center.x + 1, k_origin.y);
        for (int x = width - k_right; x < width; ++x) {
            kernel.recalcForOrigin(k_origin);
            kernel.convolve(dst_line + x, src_line, src_bpl);
            k_origin = cv::Point(k_origin.x + 1, k_origin.y);
        }
        k_origin = cv::Point(k_origin.x,k_origin.y + 1);
    }
}
