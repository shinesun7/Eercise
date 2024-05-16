#ifndef RUST_DETECTION_HPP
#define RUST_DETECTION_HPP

#include <opencv2/opencv.hpp>

cv::Mat computeGLCM(const cv::Mat& src, int dx, int dy);
float computeTextureFeature(const cv::Mat& glcm);
void detectRust(const cv::Mat& src, double& totalArea);

#endif // RUST_DETECTION_HPP
