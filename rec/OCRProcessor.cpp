#include "OCRProcessor.h"
#include <leptonica/allheaders.h>

OCRProcessor::OCRProcessor(const std::string& language) {
    tess.Init(NULL, language.c_str());
}

OCRProcessor::~OCRProcessor() {
    tess.End();
}

std::string OCRProcessor::performOCR(const cv::Mat& image) {
    cv::Mat processedImage = preprocessImage(image);
    std::vector<std::vector<cv::Point>> filteredContours = findTargetContours(processedImage);

    cv::Mat contourImage = cv::Mat::ones(processedImage.size(), CV_8UC1) * 255;
    for (size_t i = 0; i < filteredContours.size(); ++i) {
        cv::drawContours(contourImage, filteredContours, static_cast<int>(i), cv::Scalar(0), cv::FILLED);
    }

    cv::Mat rotatedContourImage;
    cv::rotate(contourImage, rotatedContourImage, cv::ROTATE_90_CLOCKWISE);

    tess.SetImage(rotatedContourImage.data, rotatedContourImage.cols, rotatedContourImage.rows, 1, rotatedContourImage.cols);
    char* outText = tess.GetUTF8Text();
    std::string result(outText);
    delete[] outText;

    std::string formattedResult;
    int length = result.length();
    if (length > 12) {
        formattedResult = result.substr(0, 5) + '\n' + result.substr(length - 7);
    } else {
        formattedResult = result;
    }

    formattedResult.erase(std::remove(formattedResult.begin(), formattedResult.end(), ' '), formattedResult.end());

    return formattedResult;
}

cv::Mat OCRProcessor::preprocessImage(const cv::Mat& inputImage) {
    cv::Mat grayImage;
    cv::cvtColor(inputImage, grayImage, cv::COLOR_BGR2GRAY);

    int thresholdValue = 44;
    cv::Mat thresholdedImage;
    cv::threshold(grayImage, thresholdedImage, thresholdValue, 255, cv::THRESH_BINARY_INV);

    int dilationSize = 1;
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,
        cv::Size(2 * dilationSize + 1, 2 * dilationSize + 1),
        cv::Point(dilationSize, dilationSize));

    cv::dilate(thresholdedImage, thresholdedImage, element);
    cv::erode(thresholdedImage, thresholdedImage, element);

    return thresholdedImage;
}

std::vector<std::vector<cv::Point>> OCRProcessor::findTargetContours(const cv::Mat& image) {
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(image.clone(), contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<std::vector<cv::Point>> filteredContours;
    for (size_t i = 0; i < contours.size(); ++i) {
        cv::Rect boundingRect = cv::boundingRect(contours[i]);
        int contourWidth = boundingRect.width;
        int contourHeight = boundingRect.height;
            if (contourWidth >= 2 && contourWidth <= 100 &&
            contourHeight >= 10 && contourHeight <= 200) {
            double contourArea = cv::contourArea(contours[i]);
            float aspectRatio = static_cast<float>(contourHeight) / contourWidth;
            if (contourArea >= 20 && contourArea <= 20000 &&
                aspectRatio >= 0.15 && aspectRatio <= 0.7) {
                filteredContours.push_back(contours[i]);
            }
        }
    }
    return filteredContours;
}


