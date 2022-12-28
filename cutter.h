// This header file contains a class for image cutting.
// Function:
// 1. Cut document from original image.
// 2. Output the content of the document using OCR.

#ifndef CUTTER_H
#define CUTTER_H

#include <opencv2/opencv.hpp>

#include <tesseract/baseapi.h>

#include <string>
using std::string;

#include <vector>
using std::vector;

class Cutter
{
public:
    Cutter();
    Cutter(const cv::Mat &image);
    ~Cutter();

    // Import original image.
    // Input: original image.
    // Output: none.
    void importImage(cv::Mat &image);

    // Cut document from original image.
    // Input: None.
    // Output: document image.
    cv::Mat cut();

    // Output the content of the document using OCR.
    // Input: None.
    // Output: content of the document.
    string ocr();

private:
    cv::Mat originalImage;
    cv::Mat documentImage;
    string content;
};

#endif // CUTTER_H