#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <cstdlib>



int main(int argc, char *argv[])
{
    int d=0;
    std::vector<std::string> paths;
    paths.push_back("/home/anubhav/Downloads/1_2016-06-22_09-59-16.mp4");
    paths.push_back("/home/anubhav/Downloads/1_2016-06-22_09-52-21.mp4");
    paths.push_back("/home/anubhav/Downloads/1_2016-06-22_09-41-29.mp4");
    paths.push_back("/home/anubhav/Downloads/1_2016-06-22_10-10-54.mp4");
    paths.push_back("/home/anubhav/Downloads/1_2016-06-22_10-10-54.mp4");
    paths.push_back("/home/anubhav/Downloads/VideoBom-800x600.mp4");

//    std::string dPath = "/home/anubhav/Downloads/1_2016-06-22_09-59-16.mp4";

    for(auto& i : paths){
    cv::VideoCapture cap(i);
    if(!cap.isOpened())
        return -1;

    cv::Mat frame;
    int c = 0;

    for(;;){
        cap>>frame;

        if(frame.empty())
            break;

        cv::resize(frame,frame,cv::Size(),0.6,0.6,cv::INTER_LINEAR);
        cv::imshow("frame",frame);

        if(c%40 == 0){
//            char fname[100];
//            scanf(fname,"/home/anubhav/Desktop/data/%i.jpg",d);
            std::string fname = "/home/anubhav/Desktop/data/" + std::to_string(d) + ".jpg";
            std::cout<<fname<<std::endl;
            cv::imwrite(fname,frame);
            d++;
        }
        c++;
        char c = cv::waitKey(1);
        if(c == 27)
            break;

    }

    cap.release();
    }
    return 0;
}
