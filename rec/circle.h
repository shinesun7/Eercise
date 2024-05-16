
#include <opencv2/opencv.hpp>

class CircleDetector {
public:
    CircleDetector();
    ~CircleDetector();

    void detectCircles(cv::Mat image);
    void drawDetectedCircles(cv::Mat& image);
    double calculateDistance(const cv::Point& pt1, const cv::Point& pt2) const;
    double calculatePixelsPerCm() const;

    // 新增获取圆心的成员函数
    cv::Point getCircleCenter1() const;
    cv::Point getCircleCenter2() const;

    double getPixelsPerCm() const
    {
        return pixelsPerCm_;//得到像素比例
    }

private:
    std::vector<cv::Vec3f> circles_;
    double pixelsPerCm_;
    // 新增私有变量保存两个圆的中心位置
    cv::Point circleCenter1_;
    cv::Point circleCenter2_;
};





