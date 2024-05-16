#include "ImageAnnotator.h"

void ImageAnnotator::annotateImage(cv::Mat& image, const std::string& recognizedText) {
    int textHeight = 30; // �ı��߶�
    int margin = 10;     // �߾�

    // ��ԭʼͼ���ϱ�עǰ����ַ������Ͻ�
    cv::putText(image, recognizedText.substr(0, 5), cv::Point(margin + 200, textHeight + 700), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);

    // ��ԭʼͼ���ϱ�ע����߸��ַ������Ͻ�
    cv::putText(image, recognizedText.substr(recognizedText.length() - 6, 5), cv::Point((image.cols - textHeight * 3 - margin) - 200, textHeight + 700), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);
}

