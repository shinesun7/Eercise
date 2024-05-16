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
    // ת����ɫ�ռ䵽HSV
    cv::Mat hsv;
    cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);

    // �趨��ɫ��ֵ����������������ɫ��H:0-34, S:36, V:36��Χ��
    cv::Mat mask;
    cv::inRange(hsv, cv::Scalar(0, 46, 46), cv::Scalar(28, 255, 255), mask);

    // ��̬ѧ������ȥ����������ʹ����������ͨ
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(mask, mask, cv::MORPH_DILATE, kernel);

    // Ѱ������
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // ����������������ˣ��޳�С�������������
    std::vector<std::vector<cv::Point>> filteredContours;
    double pixelToCmRatio = 0.00624; // ����ÿ�����ش���0.1����
    totalArea = 0; // ���ڴ洢�����
    for (const auto& contour : contours)
    {
        double area = cv::contourArea(contour);
        double actualArea = area * pixelToCmRatio * pixelToCmRatio; // ���������ת��Ϊʵ�����
        if (actualArea >= 0.09 && actualArea <= 5) // ����������������ֵ
        {
            filteredContours.push_back(contour);

            // �ۼ�ʵ�����
            totalArea += actualArea;

            // ��ͼ������ʾʵ�������Ϣ
            std::string areaText = std::to_string(actualArea) + " cm^2";
            cv::putText(src, areaText, contour[0], cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 255), 2);
        }
    }

    // ��������
    std::cout << "Total Area: " << totalArea << " cm^2" << std::endl;

    // ��ԭͼ�ϻ��ƾ���������˵�����
    cv::drawContours(src, filteredContours, -1, cv::Scalar(0, 255, 255), 2);

    // ����Ҷȹ�������
    cv::Mat glcm = computeGLCM(src, 1, 0);

    // ������������
    //float contrast = computeTextureFeature(glcm);
    //std::cout << "Contrast: " << contrast << std::endl;
}
