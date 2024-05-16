#include "image_processing.hpp"

void processImage(const Mat& image, Mat& color_image) {
    // 将图像转换为灰度图像
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);

    // 将灰度范围为30-80的部分提取出来
    int min_gray = 30;
    int max_gray = 80;
    Mat masked_image = Mat::zeros(gray.size(), CV_8UC1);
    for (int i = 0; i < gray.rows; ++i) {
        for (int j = 0; j < gray.cols; ++j) {
            if (gray.at<uchar>(i, j) >= min_gray && gray.at<uchar>(i, j) <= max_gray) {
                masked_image.at<uchar>(i, j) = gray.at<uchar>(i, j);
            }
        }
    }

    // 对提取出来的部分进行直方图均衡化
    Mat equalized_masked_image;
    equalizeHist(masked_image, equalized_masked_image);

    // 使用阈值分割将图像二值化
    int threshold_value = 70; // Renamed to avoid conflict
    Mat output_image;
    threshold(gray, output_image, threshold_value, 255, THRESH_BINARY);

    // 对二值化图像进行膨胀操作
    Mat kernel = getStructuringElement(MORPH_RECT, Size(2, 2));
    dilate(output_image, output_image, kernel, Point(-1, -1), 2);

    // 使用Canny边缘检测
    Mat edges;
    Canny(output_image, edges, 1, 300, 3);

    // 执行概率霍夫直线检测
    std::vector<Vec4i> lines;
    HoughLinesP(edges, lines, 1, CV_PI / 180, 1, 90, 8);

    // 在彩色图像上绘制直线
    if (!lines.empty()) {
        for (size_t i = 0; i < lines.size(); i++) {
            Vec4i l = lines[i];
            line(color_image, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 2);
        }
    }
}

