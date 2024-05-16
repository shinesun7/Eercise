#include <opencv2/opencv.hpp>
#include<iostream>
using namespace std;
inline cv::Mat grayImage(const string& path)
{   cv::Mat image = cv::imread(path);
    if (image.empty())
    {
        std::cerr << "Error: Image is empty." << std::endl;
        return cv::Mat();
    }

    // 将图像转换为灰度图像
    cv::Mat grayimage;
    cv::cvtColor(image, grayimage, cv::COLOR_BGR2GRAY);
    return grayimage;
}
inline cv::Mat binaryImage(const string& path)
{   cv::Mat image = cv::imread(path);
    // 检查图像是否为空
    if (image.empty()) {
        std::cerr << "Error: Image is empty." << std::endl;
        return cv::Mat();
    }
    // 将图像转换为灰度图像
    cv::Mat grayimage;
    cv::cvtColor(image, grayimage, cv::COLOR_BGR2GRAY);
    cv::Mat binaryimage;
    cv::threshold(grayimage, binaryimage, 140, 255, cv::THRESH_BINARY_INV);
    return binaryimage;
}

