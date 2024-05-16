#include "circle.h"
CircleDetector::CircleDetector() {
    pixelsPerCm_ = 0.0;
}

CircleDetector::~CircleDetector() {
}

void CircleDetector::detectCircles(cv::Mat image)
{
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(9, 9), 2, 2);

    cv::HoughCircles(gray, circles_, cv::HOUGH_GRADIENT, 1,
                     gray.rows /5,  // change this value to detect circles with different distances to each other
                     100, 30, 10, 200 // change the last two parameters
                     );

    // 过滤检测到的圆
    std::vector<cv::Vec3f> filteredCircles;
    for (const auto& circle : circles_) {
        cv::Point center(cvRound(circle[0]), cvRound(circle[1]));
        int radius = cvRound(circle[2]);
        // 过滤条件：半径适中
        if (radius > 100 && radius < 145) {
            filteredCircles.push_back(circle);
        }
    }
    circles_ = filteredCircles;

    // 保存检测到的圆心位置
    if (circles_.size() >= 2) {
        circleCenter1_ = cv::Point(circles_[0][0], circles_[0][1]);
        circleCenter2_ = cv::Point(circles_[1][0], circles_[1][1]);
    }
}

void CircleDetector::drawDetectedCircles(cv::Mat& image) {
    for (size_t i = 0; i < circles_.size(); i++) {
        cv::Vec3i c = circles_[i];
        cv::Point center = cv::Point(c[0], c[1]);
        // circle center
        cv::circle(image, center, 1, cv::Scalar(0, 100, 100), 3, cv::LINE_AA);
        // circle outline
        int radius = c[2];
        cv::circle(image, center, radius, cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
    }
    // 绘制连接两个圆心的线
    if (circleCenter1_.x && circleCenter2_.x) {
        cv::line(image, circleCenter1_, circleCenter2_, cv::Scalar(0, 0, 255), 2);

        // 计算两个圆心之间的距离
        double distance = calculateDistance(circleCenter1_, circleCenter2_);

        // 创建文本框显示距离
        std::string distanceText = "Distance: " + std::to_string(distance/calculatePixelsPerCm()) + " cm";
        cv::putText(image, distanceText, cv::Point(image.cols / 2, image.rows / 2), cv::FONT_HERSHEY_SIMPLEX, 3, cv::Scalar(0, 0, 255), 2);
    }
}

double CircleDetector::calculateDistance(const cv::Point& pt1, const cv::Point& pt2) const {
    return cv::norm(pt1 - pt2);
}

double CircleDetector::calculatePixelsPerCm() const {
    // Assuming you have calibrated your image and know the distance represented by the ruler in pixels
    double pixelDistance = 4032; // calculate the pixel distance between two points on the ruler
    double realDistance = 17.5; // actual distance in cm on the ruler
    return pixelDistance / realDistance;
}
cv::Point CircleDetector::getCircleCenter1() const {
    return circleCenter1_;
}

cv::Point CircleDetector::getCircleCenter2() const {
    return circleCenter2_;
}
