/*  Performs clsutering of data based on the input
 *  set of conditions. Currently based on density based
 *  clustering methods.
 *  Developed by Anubhav Rohatgi
 *  Date: 25/04/2016
 */
 
#include <iostream>
#include <vector>
#include <map>
#include <list>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "clustering.h"

/**
 * @brief distance_point This is a sample distance function
 *        Calculates the euclidean distance between the two
 *        points of interest.
 * @param a Data Point a
 * @param b Data Point b
 * @return Returns the calulated euclidean distance
 */
double distance_point(cv::Point2d const& a, cv::Point2d const& b)
{
    return static_cast<double>(sqrt( pow(a.x - b.x,2) + pow( a.y - b.y,2)));
}

int main() {

    std::vector<cv::Point2d> data;

    cv::RNG rng(1234);

    cv::Mat img = cv::Mat::zeros(400,400,CV_8UC3);

    for(int i =0; i < 300; i++)
        data.push_back(cv::Point2d(rng.uniform(0,400),rng.uniform(0,400)));

    for(int i =0 ; i < 300; i++)
    {
        cv::circle(img,data.at(i),2,cv::Scalar(200,0,200),-1);
    }

    cv::imshow("DataSet",img);

    std::vector<cv::Point2d> negatives;

    std::vector<clustering::cluster<cv::Point2d>> clusters = clustering::Cluster(&data[0],negatives, 300, 20.0, 2, &distance_point);

    for (clustering::cluster<cv::Point2d> c : clusters) {
        for (cv::Point2d i : c) {
            std::cout << i;
        }
        std::cout << std::endl;
    }



    //display clusters

    int count =0;
    cv::Mat fimg = cv::Mat::zeros(400,400,CV_8UC3);

    for(size_t i =0; i < clusters.size(); i++)
    {
        cv::Scalar color = cv::Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));
        for(size_t j =0; j < clusters.at(i).size(); j++) {
            cv::circle(fimg,clusters.at(i).at(j),2,color,-1);
        }
        count+=clusters.at(i).size();
    }

    for(size_t i =0 ; i < negatives.size(); i++)
    {
        cv::circle(fimg,negatives.at(i),3,cv::Scalar(255,255,255),1);
        cv::imshow("Clusters",fimg);
        cv::waitKey(50);
    }

   // count += negatives.size();
    std::cout<<"\n\nTotal inclass data size = "<< count<<std::endl;
    std::cout<<"\n\nTotal negative data size = "<< negatives.size()<<std::endl;
    std::cout<<"\n\n\nClusters Size = "<<clusters.size()<<std::endl;
    cv::imshow("Clusters",fimg);

    cv::waitKey();

    return 0;

}
