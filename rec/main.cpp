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

    //������Ǿ���
    cv::Mat contours_image = cv::imread("contours_image.jpg");
    if (contours_image.empty()) {
        std::cerr << "Error: Couldn't load contours_image ! ." << std::endl;
        return 1;
    }

    LineDistanceCalculator calculator(contours_image);
    cv::namedWindow("Coumpter Distance(click mouse)", cv::WINDOW_NORMAL);
    //cv::imshow("Image1", image1);
    //cv::setMouseCallback("Image1", LineDistanceCalculator::onMouse, &calculator);


    //Բ�׾���ļ��
    cv::Mat image_circle = cv::imread("coumpter_circle.jpg");
    if (image_circle.empty()) {
       std::cerr << "Failed to load image  coumpter_circle ! ." << std::endl;
        return -1;
    }

    // ���� ContourDetector ����
    ContourDetector contourDetector("coumpter_circle.jpg",170.00);
    // �������
    contourDetector.detectContours();
        // ��ʾ����������ͼ��
    cv::namedWindow("Contours on Original Image", cv::WINDOW_NORMAL);
   // cv::imshow("Contours on Original Image", contourDetector.mimage);

    // ����Բ���������
    CircleDetector detector;
    // ���Բ��
    detector.detectCircles(image_circle);
    // ���Ƽ�⵽��Բ��Բ������
    detector.drawDetectedCircles(image_circle);
    // ����ÿ���׵�������
    double pixelsPerCm = detector.calculatePixelsPerCm();
    // �����ߵ�ʵ�ʾ���
    double circleDiameterInCm = detector.calculateDistance(detector.getCircleCenter1(), detector.getCircleCenter2()) / pixelsPerCm;
    std::cout << "Circle diameter in cm: " << circleDiameterInCm << std::endl;
    // ��ʾͼ��
    cv::namedWindow("Detected Circles", cv::WINDOW_NORMAL);
   // cv::imshow("Detected Circles", image2);

    cv::Mat image_original = cv::imread("shachepian.jpg");
    if (image_original.empty()) {
        std::cerr << "Error: Couldn't load image shachepian ! ." << std::endl;
        return -1;
    }

    //�ַ�ʶ��
    OCRProcessor ocrProcessor("eng");
    std::string recognizedText = ocrProcessor.performOCR(image_original);
    std::cout << "ʶ����ַ�: \n" << recognizedText << std::endl;

    // ��עʶ���ַ���ԭͼ��λ��
    ImageAnnotator::annotateImage(image_original, recognizedText);

    //������������������
    double totalArea;
    detectRust(image_original, totalArea);

    //��С��ʾ�ַ�������������ڵĴ�С
    cv::namedWindow("Result_image", cv::WINDOW_NORMAL);
    cv::resizeWindow("Result_image", image_original.cols / 4, image_original.rows / 4);

    // ��ȡͼ��
    Mat image = imread("Scratch_detection.jpg");
    Mat color_image = image.clone(); // ���Ʋ�ɫͼ�����ڻ���ֱ��
    // ����ͼ����
    processImage(image, color_image);


    //��ʾԭͼ����
    cv::imshow("Contours on Original Image", contourDetector.mimage);

    //�����������ǳߴ�
    cv::imshow("Coumpter Distance(click mouse)", contours_image);
    cv::setMouseCallback("Coumpter Distance(click mouse)", LineDistanceCalculator::onMouse, &calculator);

    //����Բ�ľ���
    cv::imshow("Detected Circles", image_circle);

    //��ͼƬ����ʾ�ַ�����������ʹ�С
    cv::imshow("Result_image", image_original);

    // ��ʾ����
    imshow("Detected Lines", color_image);

    cv::waitKey(0);
    return 0;
}

