/*
 * Performs Savitzky Golay Filtering
 *
 * Developed by Anubhav Rohatgi
 * Date 26/04/2016
 */
#pragma once 

#ifndef SAVITZKYGOLAYFILTER_H
#define SAVITZKYGOLAYFILTER_H

#include <iostream>
#include <stdexcept>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "savitzkygolaykernel.h"



/**
 * @brief smoothSavGolFilter Performs grayscale smoothing using Savitzky Golay filter.
 *                      The method is equivalent to fitting a small neighborhood around
 *                      each pixel to a polynomial and the nrecaluclating the pixel value
 *                      from it. The resultant image is smoothed grayscale.
 * @param src           The source image 8 bit grayscale single channel
 * @param dst           The output 8 bit grayscale image.
 * @param window_size   The aperture size. If it doenst fit completely in the image area,
 *                      no filtering will take place.
 * @param hor_degree    The degree of polynomial in horizontal direction.
 * @param vert_degree   The degree of polynomial in vertical direction.
 * @note  The window size and degrees are not completely independent. The following
 *        inequality must be fulfilled.
 * \code
 *        window_width * window_height >= (hor_degree + 1) * (vert_degree + 1)
 * \endcode
 *
 * @note Good results for 300dpi scans are acheived with 7x7 window and 4x4 degree.
 *           DPI < 200          window = 5 and degree = 3
 *           200 > DPI < 400    window = 7 and degree = 4
 *           400 > DPI < 800    window = 11 and degree = 4
 *           OTHERWISE          window = 11 and degree = 2
 */
void smoothSavGolFilter(const cv::Mat& src, cv::Mat& dst, const cv::Size& window_size,
                        const int hor_degree, const int vert_degree);





class SavitzkyGolayFilter : public SavitzkyGolayKernel
{
public:
    SavitzkyGolayFilter(cv::Size const& size,
                        cv::Point const& origin,
                        int hor_degree, int vert_degree) :
                        SavitzkyGolayKernel(size,origin,hor_degree,vert_degree) {}

    /**
     * @brief convolve Convolves the kernel with the src image
     * @param dst Destination to the smoothed image data
     * @param src_top_left Pointer to the sources top left corner data point (begin)
     * @param src_bpl source bytes per line
     */
    void convolve( uint8_t* dst, uint8_t const* src_top_left, int src_bpl) const;
};

inline void SavitzkyGolayFilter::convolve(uint8_t *dst, const uint8_t *src_top_left, int src_bpl) const
{
    const uint8_t* p_src = src_top_left;
    const float* p_kernel = data();
    float sum = 0.5; // For rounding purposes.

    const int w = width();
    const int h = height();

    for (int y = 0; y < h; ++y, p_src += src_bpl) {
        for (int x = 0; x < w; ++x) {
            sum += p_src[x] * (*p_kernel);
            ++p_kernel;
        }
    }

    const int val = static_cast<int>(sum);
    *dst = static_cast<uint8_t>( MAX(0, MIN(val, 255)));
}

#endif // SAVITZKYGOLAYFILTER_H
