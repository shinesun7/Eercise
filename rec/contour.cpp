#include"contour.h"


ContourDetector::ContourDetector(const std::string& filename,double minAreaThreshold)
:mfilename(filename),minArea(minAreaThreshold)
{

}

void ContourDetector:: detectContours()//检测轮并绘制
{
     mimage=cv::imread(mfilename);
    // 查找轮廓

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binaryImage(mfilename), contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // 在原始图像上绘制大于阈值的轮廓
    for (size_t i = 0; i < contours.size(); ++i) {
        double area = cv::contourArea(contours[i]);
        if (area >= minArea) {
            cv::drawContours(mimage, contours, static_cast<int>(i), cv::Scalar(0, 255, 0), 2);
        }
    }

}




