// Source code for cutter.h

#include "cutter.h"

#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <algorithm>
using std::sort;

Cutter::Cutter()
{
}

Cutter::Cutter(const cv::Mat &image) : originalImage(image)
{
}

Cutter::~Cutter()
{
}

void Cutter::importImage(cv::Mat &image)
{
    originalImage = image;
}


string Cutter::ocr()
{
    if(originalImage.empty())
    {
        cout << "Image is empty" << endl;
        return "";
    }
    else if(documentImage.empty())
    {
        documentImage = originalImage;
    }
    tesseract::TessBaseAPI tess;
    tess.Init(NULL, "eng", tesseract::OEM_DEFAULT);
    tess.SetPageSegMode(tesseract::PSM_AUTO);
    tess.SetImage(documentImage.data, documentImage.cols, documentImage.rows, 3, documentImage.step);
    char *out = tess.GetUTF8Text();
    content = string(out);
    delete[] out;
    return content;
}

// Cut document from original image.
cv::Mat Cutter::cut()
{
    // Convert to grayscale.
    cv::Mat grayImage;
    cv::cvtColor(originalImage, grayImage, cv::COLOR_BGR2GRAY);

    // Apply Gaussian blur.
    cv::Mat blurImage;
    cv::GaussianBlur(grayImage, blurImage, cv::Size(3, 3), 0);

    // Apply Canny edge detection.
    cv::Mat cannyImage;
    cv::Canny(blurImage, cannyImage, 75, 200);

    // Find contours.
    vector<vector<cv::Point>> contours;
    vector<cv::Vec4i> hierarchy;
    cv::findContours(cannyImage, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Find the largest contour.
    vector<cv::Point> largestContour;
    double largestArea = 0;
    for (int i = 0; i < contours.size(); i++)
    {
        double area = cv::contourArea(contours[i]);
        if (area > largestArea)
        {
            largestArea = area;
            largestContour = contours[i];
        }
    }

    // Find the bounding rectangle of the largest contour.
    cv::RotatedRect boundingRect = cv::minAreaRect(largestContour);

    // Find the four vertices of the bounding rectangle.
    cv::Point2f vertices[4];
    boundingRect.points(vertices);

    // Sort the vertices in clockwise order.
    sort(vertices, vertices + 4, [](cv::Point2f a, cv::Point2f b) {
        return a.x < b.x;
    });
    if (vertices[0].y > vertices[1].y)
    {
        cv::Point2f temp = vertices[0];
        vertices[0] = vertices[1];
        vertices[1] = temp;
    }
    if (vertices[2].y < vertices[3].y)
    {
        cv::Point2f temp = vertices[2];
        vertices[2] = vertices[3];
        vertices[3] = temp;
    }

    // Find the perspective transform matrix.
    cv::Point2f sourceVertices[4] = {vertices[0], vertices[1], vertices[2], vertices[3]};
    cv::Point2f destinationVertices[4] = {cv::Point2f(0, 0), cv::Point2f(0, 800), cv::Point2f(800, 800), cv::Point2f(800, 0)};
    cv::Mat transformMatrix = cv::getPerspectiveTransform(sourceVertices, destinationVertices);

    // Apply perspective transform.
    cv::warpPerspective(originalImage, documentImage, transformMatrix, originalImage.size());

    return documentImage;
}