
/* Simple usage of reshape function in OpenCV  */

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>





int main(int argc, char *argv[])
{

    cv::VideoCapture cap("/home/anubhav/Desktop/video.mp4");

    if(!cap.isOpened()){
        return -1;
    }

    cv::Mat frame;


    std::vector<cv::Mat> frames;

    for(;;){

        cap>>frame;
        if(frame.empty())
            break;
        cv::resize(frame,frame,cv::Size(),0.3,0.3,cv::INTER_LINEAR);
        cv::imshow("Frame",frame);

        frames.push_back(frame);

        char c = cv::waitKey(27);
        if(c == 27){
            break;
        }
    }

    cap.release();

    std::cout<<"\nSize of the Single frame = "<<frame.size()<<std::endl;
    std::cout<<"\nSize of the vector of mats = "<<frames.size()<<std::endl;

    int height = frames[0].rows;

    //convert the vector of mats to a single matrix with
    cv::Mat matVector(frames.size(), frames[0].cols*frames[0].rows, frames[0].type());

    //reshapes the matrix to single column
    for(size_t i=0; i<frames.size(); i++){
         (frames.at(i).reshape(0,1)).copyTo(matVector.row(i));
        std::cout<<"\nSize of Mat after reshape = "<<matVector.row(i).size();
    }

    //This shows that the data is stored as columnwise where rows indicate the number of frames
    //and cols will indicate the size of single mat as rows x cols
    // ie. row1 = Mat 1
    // row2 = Mat 2 and so on
    std::cout<<"\nMatVector Size = cols  "<<matVector.cols<<" rows "<<matVector.rows<<std::endl;


    //display each image in matVector separately

    for(size_t r = 0; r < matVector.rows; r++  ){

        cv::Mat img1d;
        matVector.row(r).copyTo(img1d);

        int cn = img1d.channels();
        std::cout<<"\n"<<img1d.channels();

        cv::Mat img;
        img1d.reshape(cn,height).copyTo(img);

        cv::imshow("Frame ReConstructed",img);
        cv::waitKey();
    }

   return 0;
}
