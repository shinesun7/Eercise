#ifndef OCR_PROCESSOR_H
#define OCR_PROCESSOR_H

#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

class OCRProcessor {
public:
    OCRProcessor(const std::string& language);
    ~OCRProcessor();

    std::string performOCR(const cv::Mat& image);

private:
    tesseract::TessBaseAPI tess;

    cv::Mat preprocessImage(const cv::Mat& inputImage);
    std::vector<std::vector<cv::Point>> findTargetContours(const cv::Mat& image);
};

#endif // OCR_PROCESSOR_H

