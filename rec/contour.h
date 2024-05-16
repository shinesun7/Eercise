#include"basefunc.h"
class ContourDetector
{
public:
    const std::string& mfilename;
    double minArea;
    cv::Mat mimage;
    cv::Point prevPt{-1, -1}; // 上一个点击点;
public:
    ContourDetector(const std::string& filename,double minAreaThreshold);
    void  detectContours();
};


