#include "LineDistanceCalculator.h"
#include <iostream>

LineDistanceCalculator::LineDistanceCalculator(const cv::Mat& inputImage) : image(inputImage) {}

void LineDistanceCalculator::onMouse(int event, int x, int y, int flags, void* userdata) {
    if (userdata) {
        LineDistanceCalculator* calculator = static_cast<LineDistanceCalculator*>(userdata);
        calculator->handleMouse(event, x, y, flags);
    }
}

void LineDistanceCalculator::handleMouse(int event, int x, int y, int flags) {
    if (event == cv::EVENT_LBUTTONDOWN) {
        if (pt1.x == -1 && pt1.y == -1) {
            pt1 = cv::Point(x, y);
        } else if (pt2.x == -1 && pt2.y == -1) {
            pt2 = cv::Point(x, y);

            // 计算两点之间的距离
            double distance = cv::norm(pt1 - pt2);

            // 在图像上绘制直线
            cv::line(image, pt1, pt2, cv::Scalar(0, 0, 255), 2);

            // 显示距离
            std::cout << "realDistance between points: " << distance *(17.5/4032) << std::endl;

            // 在图像上添加文本显示距离
            std::string distanceText = "Distance: " + std::to_string(distance * (17.5 / 4032)) + " cm";
            cv::putText(image, distanceText, cv::Point(image.cols/2, image.rows/2), cv::FONT_HERSHEY_SIMPLEX, 5, cv::Scalar(0, 0, 255), 6);

            // 创建一个窗口并显示图像
            cv::namedWindow("distance_calculator", cv::WINDOW_NORMAL);
            cv::imshow("distance_calculator", image);

            // 重置点
            pt1 = cv::Point(-1, -1);
            pt2 = cv::Point(-1, -1);
        }
    }
}
