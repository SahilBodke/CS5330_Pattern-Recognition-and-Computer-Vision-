/*
    Sahil Kiran Bodke
    Spring 2023 : CS5300 - Pattern Recognition and Computer Vision
    Project 4 : Calibration and Augmented Reality
*/


#define _USE_MATH_DEFINES
#include<opencv2/core.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>
#include<cmath>
#include<cstdlib>
#include<set>
#include<stdio.h>
#include<numeric>
#include<fstream>
#include<sstream>
#include "helper.h"
using namespace std;



int greyscale(cv::Mat& src, cv::Mat& dst) {

    /*
        This function takes an cv::Mat color image as input and returns a single channel greyscale version of the image as output
        Input: cv::Mat color image
        Output: Single channel Greyscale image
    */

    dst = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);  // Allocate memory to the destination image
    for (int i = 0; i < src.rows; i++) {
        // source row pointer
        cv::Vec3b* rptrS = src.ptr<cv::Vec3b>(i);
        // destination row pointer
        cv::Vec<uchar, 1>* rptrD = dst.ptr<cv::Vec<uchar, 1>>(i);
        for (int j = 0; j < src.cols; j++) {
            rptrD[j] = 0.114 * rptrS[j][0] + 0.587 * rptrS[j][1] + 0.299 * rptrS[j][2];
        }
    }

    return 0;
}


std::vector<std::vector<cv::Point3d>> createLines(std::vector<std::vector<cv::Point3d>> &endPts) {


    /*
        This function takes an empty vector of vector of 3d points and returns the same vector containing a vector of coordinates
        of the end points of line segments that make up a virtual shape of a Flag
        Input: std::vector<std::vector<cv::Point3d>> empty vector of vectors
        Output: std::vector<std::vector<cv::Point3d>> vector of collection of end points of different line segments
    */



    float start = 0.0;

    endPts.push_back({ cv::Point3d(0.0, 0.0, start), cv::Point3d(0.0, 0.0, start+5) });

    // In X-direction
    endPts.push_back({ cv::Point3d(0.0, 0.0, start + 5), cv::Point3d(0.5, 0.0, start + 5) });
    endPts.push_back({ cv::Point3d(0.5, 0.0, start + 5), cv::Point3d(0.5, 0.0, start) });
    endPts.push_back({ cv::Point3d(0.5, 0.0, start + 5), cv::Point3d(0.5, -0.5, start + 5) });

    // In Y-direction
    endPts.push_back({ cv::Point3d(0.0, 0.0, start + 5), cv::Point3d(0.0, -0.5, start + 5) });
    endPts.push_back({ cv::Point3d(0.0, -0.5, start + 5), cv::Point3d(0.0, -0.5, start) });
    endPts.push_back({ cv::Point3d(0.0, -0.5, start + 5), cv::Point3d(0.5, -0.5, start + 5) });

    endPts.push_back({ cv::Point3d(0.5, -0.5, start + 5), cv::Point3d(0.5, -0.5, start) });



    endPts.push_back({ cv::Point3d(0.5, 0.0, start + 5), cv::Point3d(3.5, 0.0, start + 5) });
    endPts.push_back({ cv::Point3d(0.5, -0.5, start + 5), cv::Point3d(3.5, -0.5, start + 5) });

    endPts.push_back({ cv::Point3d(3.5, 0.0, start + 5), cv::Point3d(3.5, 0.0, start + 3) });
    endPts.push_back({ cv::Point3d(3.5, -0.5, start + 5), cv::Point3d(3.5, -0.5, start + 3) });

    endPts.push_back({ cv::Point3d(3.5, 0.0, start + 3), cv::Point3d(0.5, 0.0, start + 3) });
    endPts.push_back({ cv::Point3d(3.5, -0.5, start + 3), cv::Point3d(0.5, -0.5, start + 3) });


    return endPts;
}


std::vector<std::vector<cv::Point3d>> createShape(std::vector<std::vector<cv::Point3d>>& endPts, float start) {


    /*
        This function takes an empty vector of vector of 3d points and returns the same vector containing a vector of coordinates
        of the end points of line segments that make up a virtual shape of a Cube. It has dynamically defined coordinates depending on the
        calling value of 'start' in the 'main.cpp' to create an animation effect
        Input: std::vector<std::vector<cv::Point3d>> empty vector of vectors
        Output: std::vector<std::vector<cv::Point3d>> vector of collection of end points of different line segments
    */


    float x = 0;
    float y = 0;

    float base = 0.0;
    float ht = 2.0;

    if (start < 8) {
        x = start;
        y = 0;
        base = x/3;
    }
    else if (start >= 8 && start < 13) {
        x = 8;
        y = 8 - start;
        base = y/3;
    }
    else if (start >= 13 && start < 21) {
        x = 21 - start;
        y = -5;
        base = -x/3;
    }
    else if (start >= 21 && start < 26) {
        x = 0;
        y = start - 26;
        base = -y/3;
    }

    endPts.push_back({ cv::Point3d(x, y, base),cv::Point3d(x, y, base + ht) });
    endPts.push_back({ cv::Point3d(x + 1, y, base),cv::Point3d(x + 1, y, base + ht) });
    endPts.push_back({ cv::Point3d(x, y + 1, base),cv::Point3d(x, y + 1, base + ht) });
    endPts.push_back({ cv::Point3d(x + 1, y + 1, base),cv::Point3d(x + 1, y + 1, base + ht) });

    endPts.push_back({ cv::Point3d(x, y, base + ht), cv::Point3d(x + 1, y, base + ht) });
    endPts.push_back({ cv::Point3d(x, y, base + ht), cv::Point3d(x, y + 1, base + ht) });
    endPts.push_back({ cv::Point3d(x, y + 1, base + ht), cv::Point3d(x + 1, y + 1, base + ht) });
    endPts.push_back({ cv::Point3d(x + 1, y, base + ht), cv::Point3d(x + 1, y + 1, base + ht) });

    endPts.push_back({ cv::Point3d(x, y, base), cv::Point3d(x + 1, y, base) });
    endPts.push_back({ cv::Point3d(x, y, base), cv::Point3d(x, y + 1, base) });
    endPts.push_back({ cv::Point3d(x, y + 1, base), cv::Point3d(x + 1, y + 1, base) });
    endPts.push_back({ cv::Point3d(x + 1, y, base), cv::Point3d(x + 1, y + 1, base) });

    return endPts;
}