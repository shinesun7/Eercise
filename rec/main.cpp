#include <opencv2/opencv.hpp>
#include<iostream>
#include "RustDetection.h"
#include "OCRProcessor.h"
#include "ImageAnnotator.h"
#include"contour.h"
#include"circle.h"
#include "LineDistanceCalculator.h"
#include "image_processing.hpp"

using namespace std;
using namespace cv;

int main() {

    //计算羊角距离
    cv::Mat contours_image = cv::imread("contours_image.jpg");
    if (contours_image.empty()) {
        std::cerr << "Error: Couldn't load contours_image ! ." << std::endl;
        return 1;
    }

    LineDistanceCalculator calculator(contours_image);
    cv::namedWindow("Coumpter Distance(click mouse)", cv::WINDOW_NORMAL);
    //cv::imshow("Image1", image1);
    //cv::setMouseCallback("Image1", LineDistanceCalculator::onMouse, &calculator);


    //圆孔距离的检测
    cv::Mat image_circle = cv::imread("coumpter_circle.jpg");
    if (image_circle.empty()) {
       std::cerr << "Failed to load image  coumpter_circle ! ." << std::endl;
        return -1;
    }

    // 创建 ContourDetector 对象
    ContourDetector contourDetector("coumpter_circle.jpg",170.00);
    // 检测轮廓
    contourDetector.detectContours();
        // 显示包含轮廓的图像
    cv::namedWindow("Contours on Original Image", cv::WINDOW_NORMAL);
   // cv::imshow("Contours on Original Image", contourDetector.mimage);

    // 创建圆检测器对象
    CircleDetector detector;
    // 检测圆孔
    detector.detectCircles(image_circle);
    // 绘制检测到的圆与圆心连线
    detector.drawDetectedCircles(image_circle);
    // 计算每厘米的像素数
    double pixelsPerCm = detector.calculatePixelsPerCm();
    // 计算标尺的实际距离
    double circleDiameterInCm = detector.calculateDistance(detector.getCircleCenter1(), detector.getCircleCenter2()) / pixelsPerCm;
    std::cout << "Circle diameter in cm: " << circleDiameterInCm << std::endl;
    // 显示图像
    cv::namedWindow("Detected Circles", cv::WINDOW_NORMAL);
   // cv::imshow("Detected Circles", image2);

    cv::Mat image_original = cv::imread("shachepian.jpg");
    if (image_original.empty()) {
        std::cerr << "Error: Couldn't load image shachepian ! ." << std::endl;
        return -1;
    }

    //字符识别
    OCRProcessor ocrProcessor("eng");
    std::string recognizedText = ocrProcessor.performOCR(image_original);
    std::cout << "识别的字符: \n" << recognizedText << std::endl;

    // 标注识别字符在原图的位置
    ImageAnnotator::annotateImage(image_original, recognizedText);

    //计算铁锈总面积并输出
    double totalArea;
    detectRust(image_original, totalArea);

    //缩小显示字符和铁锈面积窗口的大小
    cv::namedWindow("Result_image", cv::WINDOW_NORMAL);
    cv::resizeWindow("Result_image", image_original.cols / 4, image_original.rows / 4);

    // 读取图像
    Mat image = imread("Scratch_detection.jpg");
    Mat color_image = image.clone(); // 复制彩色图像用于绘制直线
    // 划痕图像处理
    processImage(image, color_image);


    //显示原图轮廓
    cv::imshow("Contours on Original Image", contourDetector.mimage);

    //点击鼠标测量羊角尺寸
    cv::imshow("Coumpter Distance(click mouse)", contours_image);
    cv::setMouseCallback("Coumpter Distance(click mouse)", LineDistanceCalculator::onMouse, &calculator);

    //测量圆心距离
    cv::imshow("Detected Circles", image_circle);

    //在图片上显示字符、铁锈面积和大小
    cv::imshow("Result_image", image_original);

    // 显示划痕
    imshow("Detected Lines", color_image);

    cv::waitKey(0);
    return 0;
}

