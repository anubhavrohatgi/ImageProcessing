Savitzky-Golay Smoothing
The method performs savitzky golay filtering on image.
The fundamental idea is to fit a different polynomial 
to the data surrounding each data point. The smoothed 
points are computed by replacing each data point with
 the value of its fitted polynomial. Numerical 
derivatives come from computing the derivative of each 
fitted polynomial at each data point.

Developed by Anubhav Rohatgi 26/04/2016


Dependancies 
1. OpenCV 3.1
2. C++14
3. Linux

Steps to Run 
1. go to the directory
2. mkdir build
3. cd build
4. cmake ..
5. make




