
#ifndef IMAGE_ANNOTATOR_H
#define IMAGE_ANNOTATOR_H

#include <opencv2/opencv.hpp>

class ImageAnnotator {
public:
    static void annotateImage(cv::Mat& image, const std::string& recognizedText);
};

#endif // IMAGE_ANNOTATOR_H


