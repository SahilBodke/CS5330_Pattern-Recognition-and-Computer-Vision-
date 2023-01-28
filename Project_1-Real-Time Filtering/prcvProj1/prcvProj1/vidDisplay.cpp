/* Sahil Kiran Bodke
   Spring 2023: CS 5330 PRCV
   Project 1: Real-Time Filtering
*/

#include<opencv2/core.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>
#include<stdio.h>
#include "filter.h"
using namespace std;



int blurring = 0;  // Global Variable for Extension Task

void onChange(int val, void* userdata) {
    blurring = val;
}


int main(int argc, char* argv[]) {

    // Create a VideoCapture object pointer
    cv::VideoCapture* capdev;

    // Dynamically allocate memory to video capture object
    capdev = new cv::VideoCapture(0);


    // Check if the video file was successfully opened
    if (!capdev->isOpened()) {
        printf("Unable to open the video\n");
        exit(-1);
    }

    // Get properties of the image - width and height
    cv::Size refS((int)capdev->get(cv::CAP_PROP_FRAME_WIDTH), (int)capdev->get(cv::CAP_PROP_FRAME_HEIGHT));

    printf("Input Image Size: %d %d\n", refS.width, refS.height);

    cv::namedWindow("Video", 1);
    cv::Mat frame;
    int k = 0, i = 0;

    char key = ' ';
    char temp = ' ';



    while (true) {

        // Read the incoming frame
        capdev->read(frame);

        if (frame.empty()) {
            cout << "Frame is empty" << endl;
            break;
        }

        //cv::resize(frame, frame, cv::Size(800, 450), 0, 0); // Resize the incoming frame


        temp = cv::waitKey(10);  // Check if there is a keystroke
        if (temp != -1) {
            key = temp;
        }

        // Convert video from BGR to grayscale
        if (key == 'g') {


            cv::Mat dst;
            cv::cvtColor(frame, dst, cv::COLOR_BGR2GRAY);
            cv::imshow("Greyscale Video", dst);
        }

        //Show original video 
        if (key == 'u') {


            cv::imshow("Video", frame);
        }

        // Save the frame
        if (key == 's') {
            string savePath = "C:\\Users\\sahil\\MS-Academics\\Spring_2023\\CS_5330-PRCV\\Codes\\Project_1-Real-Time Filtering\\vidDisplay\\savedOutput\\";
            cv::imwrite(savePath + "savedImg_" + to_string(i + 1) + ".jpg", frame);
            i++;
            key = ' ';
        }

        if (key == 'h') {

            cv::Mat dst;
            greyscale(frame, dst);
            cv::imshow("Alternative Greyscale", dst);

        }

        if (key == 'b') {

            cv::Mat blurImg;
            blur5x5(frame, blurImg);

            // blur5x5 returns 16-bit signed char but imshow requires 8-bit unsigned char [0..255]
            cv::Mat displayBlur;

            // Convert blurImg to [0..255] BGR in order to display it 
            cv::convertScaleAbs(blurImg, displayBlur);

            cv::imshow("Blurred Frame", displayBlur);
        }

        if (key == 'x') {

            cv::Mat sobelX;
            sobelX3x3(frame, sobelX);

            cv::Mat dispSobelX;

            // Convert dispSobelX to [0..255] BGR in order to display it 
            cv::convertScaleAbs(sobelX, dispSobelX, 2);

            cv::imshow("Sobel_X Frame", dispSobelX);
        }

        if (key == 'y') {

            cv::Mat sobelY;
            sobelY3x3(frame, sobelY);

            cv::Mat dispSobelY;

            // Convert dispSobelY to [0..255] BGR in order to display it 
            cv::convertScaleAbs(sobelY, dispSobelY, 2);

            cv::imshow("Sobel_Y Frame", dispSobelY);
        }

        if (key == 'm') {

            cv::Mat magn, sx, sy;
            sobelX3x3(frame, sx);
            sobelY3x3(frame, sy);
            magnitude(sx, sy, magn);

            cv::Mat dispMag;

            // Convert magn to [0..255] BGR in order to display it 
            cv::convertScaleAbs(magn, dispMag, 4);

            cv::imshow("Magnitude Frame", dispMag);
        }

        // Blurring and Quantizing image
        if (key == 'l') {

            cv::Mat dst;
            blurQuantize(frame, dst, 15);

            cv::imshow("Blurred and Quantized", dst);
        }

        // Cartoonize image
        if (key == 'c') {

            cv::Mat dst;
            cartoon(frame, dst, 15, 15);

            cv::imshow("Cartoonized Image", dst);
        }


        // Pencil Sketch image
        if (key == 'p') {

            cv::Mat dst;
            pencilSketch(frame, dst);

            cv::imshow("Pencil Sketch", dst);
        }

        // Return the function for exiting the code
        if (key == 'q') {
            cv::destroyAllWindows();
            break;
        }




        // ///////////////////    EXTENSION    ///////////////////////

        /*
        This is the extension part of the project where the functions learned during the core tasks are explored in-depth
        and implemented in a creative manner to create customized effects
        */


        if (key == 'e') {

            static int flag = 1; // Flag to have only one trackbar for all the iterations


            cv::namedWindow("BlurExtension", cv::WINDOW_AUTOSIZE);   // Create a window for Trackbar

            if (flag) {
                cv::createTrackbar("Value", "BlurExtension", NULL, 5, onChange);  // Create a Trackbar with name 'Value
                flag = 0;
            }

            cv::Mat dst;  // Destination image
            cv::Mat disp; // Image for storing 8-bit unsigned character image 

            cv::setTrackbarPos("Value", "BlurExtension", blurring);  // Set the initial value of trackbar

            // Apply Gaussian Blur
            int sigma = 1 * blurring;
            int kernelSize = 2 * sigma + 1;  // Kernel size based on the value of sigma 
            cv::GaussianBlur(frame, dst, cv::Size(kernelSize, kernelSize), sigma, sigma);

            cv::convertScaleAbs(dst, disp);   // Convert dst to [0..255] BGR in order to display it 
            cv::imshow("DstExtension", disp);  // Display the final output
        }

        cv::imshow("Video", frame);   // Display the original video frame

    }

    delete capdev;   // Deallocate the dynamically assigned memory  

    return 0;
}