#ifndef FILTER
#define FILTER
#include<opencv2/core.hpp>

int greyscale(cv::Mat& src, cv::Mat& dst); // Converts BGR image to Greyscale

int blur5x5(cv::Mat& src, cv::Mat& dst);  // Blurs image using a 5x5 Gaussian Filter

int sobelX3x3(cv::Mat& src, cv::Mat& dst);  // Computes the X gradient of the input image (Vertical Edge Detector)

int sobelY3x3(cv::Mat& src, cv::Mat& dst);  // Computes the Y gradient of the input image (Horizontal Edge Detector)

int magnitude(cv::Mat& sx, cv::Mat& sy, cv::Mat& dst);  // Generates a gradient magnitude image using euclidean distance

int blurQuantize(cv::Mat& src, cv::Mat& dst, int levels);  // Blurs and quantizes an image as per the given number of 'levels'

int cartoon(cv::Mat& src, cv::Mat& dst, int levels, int magThreshold);  // Cartoonizes an image

int pencilSketch(cv::Mat& src, cv::Mat& dst);  // Creates a pencil sketch of the input image


#endif