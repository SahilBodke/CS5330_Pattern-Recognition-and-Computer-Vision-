/* Sahil Kiran Bodke
   Spring 2023: CS 5330 PRCV
   filter.cpp
*/
#define _USE_MATH_DEFINES
#include<opencv2/core.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>
#include<cmath>
#include<stdio.h>
using namespace std;


int greyscale(cv::Mat& src, cv::Mat& dst) {

    dst = cv::Mat::zeros(src.rows, src.cols, CV_8UC3);  // Allocate memory to the destination image

    for (int i = 0; i < src.rows; i++) {

        // source row pointer
        cv::Vec3b* rptrS = src.ptr<cv::Vec3b>(i);

        // destination row pointer
        cv::Vec3b* rptrD = dst.ptr<cv::Vec3b>(i);

        for (int j = 0; j < src.cols; j++) {

            // copying the 'green' color channel of the source image to the channel of destination image
            rptrD[j][0] = rptrS[j][1];
            rptrD[j][1] = rptrS[j][1];
            rptrD[j][2] = rptrS[j][1];

        }
    }

    return 0;
}

int blur5x5(cv::Mat& src, cv::Mat& dst) {

    int gauss[5] = { 1, 2, 4, 2, 1 };  // 1x5 separable gaussian filter

    dst = cv::Mat::zeros(src.rows, src.cols, CV_16SC3);  // Allocate memory to the destination image
    //src.copyTo(dst);
    cv::Mat temp = cv::Mat::zeros(src.size(), CV_16SC3);  // Allocate memory to an intermediate image (temp)


    // convolution 1
    for (int i = 0; i < src.rows; i++) {

        // source row pointer
        cv::Vec3b* rptr = src.ptr<cv::Vec3b>(i);

        // destination row pointer
        cv::Vec3s* dptr = temp.ptr<cv::Vec3s>(i);

        for (int j = 0; j < src.cols; j++) {

            // For each color channel of the current pixel
            for (int c = 0; c < 3; c++) {

                if (j == 0) {  // Handling edge case: at column 0

                    dptr[j][c] = (gauss[0] * rptr[j + 1][c] + gauss[1] * rptr[j][c] + gauss[2] * rptr[j][c] +
                        gauss[3] * rptr[j + 1][c] + gauss[4] * rptr[j + 2][c]) / 10;
                }

                else if (j == 1) {  // Handling edge case: at column 1

                    dptr[j][c] = (gauss[0] * rptr[j - 1][c] + gauss[1] * rptr[j - 1][c] + gauss[2] * rptr[j][c] +
                        gauss[3] * rptr[j + 1][c] + gauss[4] * rptr[j + 2][c]) / 10;
                }

                else if (j == src.cols - 2) {  // Handling edge case: at 2nd to last column 

                    dptr[j][c] = (gauss[0] * rptr[j - 2][c] + gauss[1] * rptr[j - 1][c] + gauss[2] * rptr[j][c] +
                        gauss[3] * rptr[j + 1][c] + gauss[4] * rptr[j + 1][c]) / 10;
                }

                else if (j == src.cols - 1) {  // Handling edge case: at last column 

                    dptr[j][c] = (gauss[0] * rptr[j - 2][c] + gauss[1] * rptr[j - 1][c] + gauss[2] * rptr[j][c] +
                        gauss[3] * rptr[j][c] + gauss[4] * rptr[j - 1][c]) / 10;
                }

                else {
                    dptr[j][c] = (gauss[0] * rptr[j - 2][c] + gauss[1] * rptr[j - 1][c] + gauss[2] * rptr[j][c] +
                        gauss[3] * rptr[j + 1][c] + gauss[4] * rptr[j + 2][c]) / 10;
                }
            }
        }

    }


    // convolution 2
    for (int i = 0; i < src.rows; i++) {

        // source row pointers
        cv::Vec3s* rptrm2 = temp.ptr<cv::Vec3s>(i - 2);
        cv::Vec3s* rptrm1 = temp.ptr<cv::Vec3s>(i - 1);
        cv::Vec3s* rptr = temp.ptr<cv::Vec3s>(i);
        cv::Vec3s* rptrp1 = temp.ptr<cv::Vec3s>(i + 1);
        cv::Vec3s* rptrp2 = temp.ptr<cv::Vec3s>(i + 2);

        // destination row pointer
        cv::Vec3s* dptr = dst.ptr<cv::Vec3s>(i);

        for (int j = 0; j < src.cols; j++) {

            // For each color channel of the current pixel
            for (int c = 0; c < 3; c++) {

                if (i == 0) {  // Handling edge case: at row 0

                    dptr[j][c] = (gauss[0] * rptrp1[j][c] + gauss[1] * rptr[j][c] + gauss[2] * rptr[j][c] +
                        gauss[3] * rptrp1[j][c] + gauss[4] * rptrp2[j][c]) / 10;
                }

                else if (i == 1) {  // Handling edge case: at row 1

                    dptr[j][c] = (gauss[0] * rptrp1[j][c] + gauss[1] * rptrp1[j][c] + gauss[2] * rptr[j][c] +
                        gauss[3] * rptrp1[j][c] + gauss[4] * rptrp2[j][c]) / 10;
                }

                else if (i == src.rows - 2) {  // Handling edge case: at 2nd to last row 

                    dptr[j][c] = (gauss[0] * rptrm2[j][c] + gauss[1] * rptrm1[j][c] + gauss[2] * rptr[j][c] +
                        gauss[3] * rptrp1[j][c] + gauss[4] * rptrp1[j][c]) / 10;
                }

                else if (i == src.rows - 1) {  // Handling edge case: at last row 

                    dptr[j][c] = (gauss[0] * rptrm2[j][c] + gauss[1] * rptrm1[j][c] + gauss[2] * rptr[j][c] +
                        gauss[3] * rptr[j][c] + gauss[4] * rptrm1[j][c]) / 10;
                }

                else {
                    dptr[j][c] = (gauss[0] * rptrm2[j][c] + gauss[1] * rptrm1[j][c] + gauss[2] * rptr[j][c] +
                        gauss[3] * rptrp1[j][c] + gauss[4] * rptrp2[j][c]) / 10;
                }
            }
        }

    }

    return 0;
}

// Sobel X Filter
int sobelX3x3(cv::Mat& src, cv::Mat& dst) {

    dst = cv::Mat::zeros(src.size(), CV_16SC3);  // Allocate memory to the destination image
    cv::Mat temp = cv::Mat::zeros(src.size(), CV_16SC3);  // Allocate memory to an intermediate image (temp)

    // Implementing Sobel X as separable filters
    int sepFil1[3] = { -1,0,1 };
    int sepFil2[3] = { 1,2,1 };

    // convolution 1
    for (int i = 0; i < src.rows; i++) {

        // source row pointer
        cv::Vec3b* rptr = src.ptr<cv::Vec3b>(i);

        // destination row pointer
        cv::Vec3s* dptr = temp.ptr<cv::Vec3s>(i);

        for (int j = 0; j < src.cols; j++) {

            // for each channel of the current pixel
            for (int c = 0; c < 3; c++) {

                if (j == 0) {  // Handling edge case: at column 0

                    dptr[j][c] = (sepFil1[0] * rptr[j][c] + sepFil1[1] * rptr[j][c] + sepFil1[2] * rptr[j + 1][c]) / 1;
                }

                else if (j == src.cols - 1) {  // Handling edge case: at last column 

                    dptr[j][c] = (sepFil1[0] * rptr[j - 1][c] + sepFil1[1] * rptr[j][c] + sepFil1[2] * rptr[j][c]) / 1;
                }

                else {
                    dptr[j][c] = (sepFil1[0] * rptr[j - 1][c] + sepFil1[1] * rptr[j][c] + sepFil1[2] * rptr[j + 1][c]) / 1;  // Normalizing after convolution
                }
            }
        }
    }


    // convolution 2
    for (int i = 0; i < src.rows; i++) {

        // source row pointers
        cv::Vec3s* rptrm1 = temp.ptr<cv::Vec3s>(i - 1);
        cv::Vec3s* rptr = temp.ptr<cv::Vec3s>(i);
        cv::Vec3s* rptrp1 = temp.ptr<cv::Vec3s>(i + 1);

        // destination row pointer
        cv::Vec3s* dptr = dst.ptr<cv::Vec3s>(i);

        for (int j = 0; j < src.cols; j++) {

            // for each channel of the current pixel
            for (int c = 0; c < 3; c++) {

                if (i == 0) {  // Handling edge case: at row 0

                    dptr[j][c] = (sepFil2[0] * rptr[j][c] + sepFil2[1] * rptr[j][c] + sepFil2[2] * rptrp1[j][c]) / 4;
                }

                else if (i == src.rows - 1) {  // Handling edge case: at last row 

                    dptr[j][c] = (sepFil2[0] * rptrm1[j][c] + sepFil2[1] * rptr[j][c] + sepFil2[2] * rptr[j][c]) / 4;
                }

                else {
                    dptr[j][c] = (sepFil2[0] * rptrm1[j][c] + sepFil2[1] * rptr[j][c] + sepFil2[2] * rptrp1[j][c]) / 4;  // Normalizing after convolution
                }
            }
        }
    }
    return 0;
}

// Sobel Y Filter
int sobelY3x3(cv::Mat& src, cv::Mat& dst) {

    dst = cv::Mat::zeros(src.size(), CV_16SC3);  // Allocate memory to the destination image
    cv::Mat temp = cv::Mat::zeros(src.size(), CV_16SC3);  // Allocate memory to an intermediate image (temp)

    // Implementing Sobel Y as separable filters
    int sepFil1[3] = { 1,2,1 };
    int sepFil2[3] = { 1,0,-1 };

    // convolution 1
    for (int i = 0; i < src.rows; i++) {

        // source row pointer
        cv::Vec3b* rptr = src.ptr<cv::Vec3b>(i);

        // destination row pointer
        cv::Vec3s* dptr = temp.ptr<cv::Vec3s>(i);

        for (int j = 0; j < src.cols; j++) {

            // for each channel of the current pixel
            for (int c = 0; c < 3; c++) {

                if (j == 0) {  // Handling edge case: at column 0

                    dptr[j][c] = (sepFil1[0] * rptr[j][c] + sepFil1[1] * rptr[j][c] + sepFil1[2] * rptr[j + 1][c]) / 4;
                }

                else if (j == src.cols - 1) {  // Handling edge case: at last column

                    dptr[j][c] = (sepFil1[0] * rptr[j - 1][c] + sepFil1[1] * rptr[j][c] + sepFil1[2] * rptr[j][c]) / 4;
                }

                else {
                    dptr[j][c] = (sepFil1[0] * rptr[j - 1][c] + sepFil1[1] * rptr[j][c] + sepFil1[2] * rptr[j + 1][c]) / 4;  // Normalizing after convolution
                }
            }
        }
    }

    // convolution 2
    for (int i = 0; i < src.rows; i++) {

        // source row pointers
        cv::Vec3s* rptrm1 = temp.ptr<cv::Vec3s>(i - 1);
        cv::Vec3s* rptr = temp.ptr<cv::Vec3s>(i);
        cv::Vec3s* rptrp1 = temp.ptr<cv::Vec3s>(i + 1);

        // destination row pointer
        cv::Vec3s* dptr = dst.ptr<cv::Vec3s>(i);

        for (int j = 0; j < src.cols; j++) {

            // for each channel of the current pixel
            for (int c = 0; c < 3; c++) {

                if (i == 0) {  // Handling edge case: at row 0

                    dptr[j][c] = (sepFil2[0] * rptr[j][c] + sepFil2[1] * rptr[j][c] + sepFil2[2] * rptrp1[j][c]) / 1;
                }

                else if (i == src.rows - 1) {  // Handling edge case: at last row

                    dptr[j][c] = (sepFil2[0] * rptrm1[j][c] + sepFil2[1] * rptr[j][c] + sepFil2[2] * rptr[j][c]) / 1;
                }

                else {
                    dptr[j][c] = (sepFil2[0] * rptrm1[j][c] + sepFil2[1] * rptr[j][c] + sepFil2[2] * rptrp1[j][c]) / 1;  // Normalizing after convolution
                }
            }
        }
    }
    return 0;
}

// Gradient Magnitude
int magnitude(cv::Mat& sx, cv::Mat& sy, cv::Mat& dst) {

    dst = cv::Mat::zeros(sx.size(), CV_16SC3);  // Allocate memory to destination image

    for (int i = 0; i < sx.rows; i++) {

        // Sobel row pointers
        cv::Vec3s* rptrSx = sx.ptr<cv::Vec3s>(i);
        cv::Vec3s* rptrSy = sy.ptr<cv::Vec3s>(i);

        // destination row pointer
        cv::Vec3s* dptr = dst.ptr<cv::Vec3s>(i);

        for (int j = 0; j < sx.cols; j++) {

            // for each color channel of the current pixel
            for (int c = 0; c < 3; c++) {

                dptr[j][c] = sqrt(rptrSx[j][c] * rptrSx[j][c] + rptrSy[j][c] * rptrSy[j][c]);
            }
        }
    }

    return 0;
}

// Blur and Quantize image
int blurQuantize(cv::Mat& src, cv::Mat& dst, int levels) {

    dst = cv::Mat::zeros(src.size(), CV_8UC3);  // Allocate memory to destination image

    blur5x5(src, dst);   // 1. Blur the image
    cv::convertScaleAbs(dst, dst);  // convert dst to 8-bit unsigned character image (BGR [0..255])

    // 2. Quantizing the image
    int b = 255 / levels;  // Determine number of buckets

    for (int i = 0; i < src.rows; i++) {

        // destination row pointer
        cv::Vec3b* rptr = dst.ptr<cv::Vec3b>(i);

        for (int j = 0; j < src.cols; j++) {

            // for each channel of the current pixel
            for (int c = 0; c < 3; c++) {

                int xt = rptr[j][c] / b;
                int xf = xt * b;
                rptr[j][c] = xf;
            }
        }
    }

    return 0;
}

// Cartoonize Image
int cartoon(cv::Mat& src, cv::Mat& dst, int levels, int magThreshold) {

    cv::Mat sx, sy, magn;

    sobelX3x3(src, sx);  // Get a gradient X image
    sobelY3x3(src, sy);  // Get a gradient Y image

    magnitude(sx, sy, magn);  // Get a gradient magnitude image
    cv::convertScaleAbs(magn, magn);  // Convert 'magn' to 8-bit unsigned char

    blurQuantize(src, dst, levels);  // Blur and quantize the 'src' image


    for (int i = 0; i < dst.rows; i++) {

        cv::Vec3b* rptrD = dst.ptr<cv::Vec3b>(i);  // destination row pointer
        cv::Vec3b* rptrM = magn.ptr<cv::Vec3b>(i);  // magnitude gradient image (magn) row pointer 

        for (int j = 0; j < dst.cols; j++) {

            // for color channel[0] of the current pixel, set pixels with magnitude gradients > magThreshold to 0
            if (rptrM[j][0] > magThreshold) {
                rptrD[j][0] = 0;
            }

            // for color channel[1] of the current pixel, set pixels with magnitude gradients > magThreshold to 0
            if (rptrM[j][1] > magThreshold) {
                rptrD[j][1] = 0;
            }

            // for color channel[2] of the current pixel, set pixels with magnitude gradients > magThreshold to 0
            if (rptrM[j][2] > magThreshold) {
                rptrD[j][2] = 0;
            }
        }
    }

    return 0;
}

// Function to invert an image. Used as a helper function for 'pencilSketch'
int invertHelper(cv::Mat& src, cv::Mat& dst) {

    dst = cv::Mat::zeros(src.size(), CV_8UC3);  // Allocate memory to destination image

    for (int i = 0; i < src.rows; i++) {

        // source row pointer
        cv::Vec3b* rptrS = src.ptr<cv::Vec3b>(i);

        // destination row pointer
        cv::Vec3b* rptrD = dst.ptr<cv::Vec3b>(i);

        for (int j = 0; j < src.cols; j++) {

            // Invert each channel of the current pixel
            rptrD[j][0] = 255 - rptrS[j][0];
            rptrD[j][1] = 255 - rptrS[j][1];
            rptrD[j][2] = 255 - rptrS[j][2];
        }
    }

    return 0;
}

// Pencil Sketch
int pencilSketch(cv::Mat& src, cv::Mat& dst) {

    dst = cv::Mat::zeros(src.size(), CV_8UC3);  // Allocate memory to destination image
    cv::Mat grey, inv, blur;

    greyscale(src, grey);  // Convert src image to greyscale
    invertHelper(grey, inv);  // Invert the greyscale image
    blur5x5(inv, blur);  // Blur the inverted image

    cv::convertScaleAbs(blur, blur);  // Convert 'blur' image to 8-bit unsigned char

    invertHelper(blur, inv);  // Invert the blurred image
    cv::divide(grey, inv, dst, 256.0);  // Blend the greyscale (grey) and inverted (inv) images

    return 0;

}
