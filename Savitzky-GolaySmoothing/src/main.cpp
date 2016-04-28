#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "savitzkygolayfilter.h"



int main()
{
    std::string dPath ="/home/coin/Desktop/Windows-share/";
    cv::Mat img = cv::imread(dPath + "scan5.tif",CV_LOAD_IMAGE_GRAYSCALE);

    if(img.empty()) {

        std::cout<<"\nImage is empty\n";
        return -1;
    }

   // cv::resize(img,img,cv::Size(), .30,.30,cv::INTER_LINEAR);
    cv::imshow("Original",img);


    cv::Mat dst;

    smoothSavGolFilter(img,dst,cv::Size(7,7),4,4);

    cv::imshow("dst",dst);
    cv::waitKey();

    return 0;
}

