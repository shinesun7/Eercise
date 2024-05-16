#include "RustDetection.h"
#include <iostream>

cv::Mat computeGLCM(const cv::Mat& src, int dx, int dy)
{
    cv::Mat glcm(256, 256, CV_32F, cv::Scalar(0));
    for (int y = 0; y < src.rows - dy; ++y)
    {
        for (int x = 0; x < src.cols - dx; ++x)
        {
            int value1 = src.at<uchar>(y, x);
            int value2 = src.at<uchar>(y + dy, x + dx);
            glcm.at<float>(value1, value2) += 1;
        }
    }
    cv::normalize(glcm, glcm, 0, 255, cv::NORM_MINMAX);
    return glcm;
}

float computeTextureFeature(const cv::Mat& glcm)
{
    float contrast = 0;
    for (int i = 0; i < glcm.rows; ++i)
    {
        for (int j = 0; j < glcm.cols; ++j)
        {
            contrast += (i - j) * (i - j) * glcm.at<float>(i, j);
        }
    }
    return contrast;
}

void detectRust(const cv::Mat& src, double& totalArea)
{
    // 转换颜色空间到HSV
    cv::Mat hsv;
    cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);

    // 设定颜色阈值，这里假设铁锈的颜色在H:0-34, S:36, V:36范围内
    cv::Mat mask;
    cv::inRange(hsv, cv::Scalar(0, 46, 46), cv::Scalar(28, 255, 255), mask);

    // 形态学操作，去除噪声，并使铁锈区域连通
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(mask, mask, cv::MORPH_DILATE, kernel);

    // 寻找轮廓
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // 对轮廓进行面积过滤，剔除小面积的铁锈区域
    std::vector<std::vector<cv::Point>> filteredContours;
    double pixelToCmRatio = 0.00624; // 假设每个像素代表0.1厘米
    totalArea = 0; // 用于存储总面积
    for (const auto& contour : contours)
    {
        double area = cv::contourArea(contour);
        double actualArea = area * pixelToCmRatio * pixelToCmRatio; // 将像素面积转换为实际面积
        if (actualArea >= 0.09 && actualArea <= 5) // 根据需求调整面积阈值
        {
            filteredContours.push_back(contour);

            // 累加实际面积
            totalArea += actualArea;

            // 在图像上显示实际面积信息
            std::string areaText = std::to_string(actualArea) + " cm^2";
            cv::putText(src, areaText, contour[0], cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 255), 2);
        }
    }

    // 输出总面积
    std::cout << "Total Area: " << totalArea << " cm^2" << std::endl;

    // 在原图上绘制经过面积过滤的轮廓
    cv::drawContours(src, filteredContours, -1, cv::Scalar(0, 255, 255), 2);

    // 计算灰度共生矩阵
    cv::Mat glcm = computeGLCM(src, 1, 0);

    // 计算纹理特征
    //float contrast = computeTextureFeature(glcm);
    //std::cout << "Contrast: " << contrast << std::endl;
}
