

#include <opencv2/opencv.hpp>
#include<iostream>
class LineDistanceCalculator
{
private:
    cv::Mat image;
    cv::Point pt1{-1, -1};
    cv::Point pt2{-1, -1};

public:
    LineDistanceCalculator(const cv::Mat& inputImage);

    static void onMouse(int event, int x, int y, int flags, void* userdata);

    void handleMouse(int event, int x, int y, int flags);
};



