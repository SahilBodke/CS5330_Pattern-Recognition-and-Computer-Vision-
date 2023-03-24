/*
	Sahil Kiran Bodke
	Spring 2023 : CS5300 - Pattern Recognition and Computer Vision
	Project 4 : Calibration and Augmented Reality
*/

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/dnn.hpp>
#include <iostream>
#include <stdio.h>
#include <opencv2/calib3d.hpp>
#include<fstream>
#include<sstream>
#include "csv_util.h"
#include "helper.h"

using namespace std;


int main(int argc, char* argv[]) {

	// Create a VideoCapture object pointer
	cv::VideoCapture* capdev;

	// Dynamically allocate memory to video capture object
	capdev = new cv::VideoCapture("http://192.168.38.110:4747/video");
	//capdev = new cv::VideoCapture(0);

	// Check if the video file was successfully opened
	if (!capdev->isOpened()) {
		printf("Unable to open the video\n");
		exit(-1);
	}
	// Get properties of the image - width and height
	cv::Size refS((int)capdev->get(cv::CAP_PROP_FRAME_WIDTH), (int)capdev->get(cv::CAP_PROP_FRAME_HEIGHT));
	printf("Input Image Size: %d %d\n", refS.width, refS.height);


	char key = ' ';
	char temp = ' ';

	cv::Mat frame;

	vector<vector<cv::Point3f>> pointList;  //Create a list for storing the point set of 3D world points
	vector<vector<cv::Point2f>> cornerList; //Create a list for storing the corner set of the 2D points

	cv::Mat distortionCoeffs;  //To store the distortion coefficients 
	cv::Mat cameraMatrix = cv::Mat::eye(3, 3, CV_64FC1);  //To store the camera calibration matrix

	cameraMatrix.at<double>(0, 2) = refS.width / 2;
	cameraMatrix.at<double>(1, 2) = refS.height / 2;

	//Camera calibration matrix before calibration
	cout << "Camera Matrix before calibration" << endl;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout << cameraMatrix.at<double>(i, j) << " ";
		}
		cout << endl;
	}
	cout << endl;

	int f = 0;

	vector<cv::Point2f> imgPoints;

	while (true) {

		// Read the incoming frame
		capdev->read(frame);
		if (frame.empty()) {
			cout << "Frame is empty" << endl;
			break;
		}

		temp = cv::waitKey(10);
		if (temp != -1) {
			key = temp;
		}

		cv::Mat grey, dst, projImg;
		frame.copyTo(projImg);


		vector<cv::Point2f> cornerSet;
		vector<cv::Point3f> pointSet;

		greyscale(frame, grey);

		//Check if corners are found
		bool patternFound = cv::findChessboardCorners(grey, cv::Size(9, 6), cornerSet, cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE
			+ cv::CALIB_CB_FAST_CHECK);

		if (patternFound) {
			cv::TermCriteria criteria(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, 30, 0.001);
			cv::cornerSubPix(grey, cornerSet, cv::Size(5, 5), cv::Size(-1, -1), criteria);  //Refine the detected corner locations

			cv::drawChessboardCorners(frame, cv::Size(9, 6), cornerSet, patternFound);  //Display the detected corners on the video stream

			cout << "Co-ordinates of the 1st corner: " << '(' << cornerSet[0].x << ', ' << ')' << endl;
			cout << "Number of Corners found: " << cornerSet.size() << endl;
		}

		if (key == 's') {

			static int k = 0;

			//Save the current frame
			cv::imwrite("img_" + to_string(k) + ".jpg", frame);
			k++;

			//Create the point list and corner list
			if (patternFound) {
				cornerList.push_back(cornerSet);

				for (int i = 0; i < 6; i++) {
					for (int j = 0; j < 9; j++) {

						cv::Point3f v = { float(j), float(-i), float(0) };
						pointSet.push_back(v);
					}
				}

				pointList.push_back(pointSet);
			}
			else {
				cout << "No Corners Detected" << endl;
			}

			//Tell the user how many more images are to be selected
			if (cornerList.size() < 5) {
				cout << "Select atleast " << 5 - cornerList.size() << " more images" << endl;
			}
			else {
				cout << "Adequate images selected!" << endl;
			}
			key = ' ';
		}

		// Run Calibration
		if (key == 'c') {

			double reprojectionError;

			if (cornerList.size() >= 5) {

				//cv::Mat distortionCoeffs;
				vector<cv::Mat> R, T;

				int flags = cv::CALIB_SAME_FOCAL_LENGTH;
				cv::TermCriteria criteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 30, 0.001);
				reprojectionError = cv::calibrateCamera(pointList, cornerList, cv::Size(frame.rows, frame.cols), cameraMatrix, distortionCoeffs, R, T, flags, criteria);
			
			}
			else {
				cout << "Not enough calibration images" << endl;
			}

			if (f == 0) {
				//Camera calibration matrix after calibration
				cout << "\nCamera Matrix after calibration" << endl;
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						cout << cameraMatrix.at<double>(i, j) << " ";
					}
					cout << endl;
				}
				f = 1;
			}

			//Distortion Coefficients after calibration
			cout << "\nDistortion Coefficients after calibration" << endl;
			for (int i = 0; i < distortionCoeffs.cols; i++) {
				cout << distortionCoeffs.at<double>(i, 0) << endl;
			}
			cout << endl;

			//Display reprojection error
			cout << "\nReprojection error: " << reprojectionError << endl;

			//Save the camera calibration matrix and distortion coefficients 
			char ans;
			cout << "Save the calibration matrix and distortation coefficients? (y/n)";
			cin >> ans;
			ans = (char)tolower(ans);

			if (ans == 'y') {

				// Write features to a csv file 
				ofstream file("intrinsicParams.csv", ofstream::app);
				if (!file.is_open()) {
					cerr << "Error opening file" << endl;
				}
				file << "Camera Matrix" << ",";
				for (int i = 0; i < cameraMatrix.rows;i++) {
					for (int j = 0; j < cameraMatrix.cols; j++) {
						file << cameraMatrix.at<double>(i, j) << ", ";
					}
				}
				file << "\n";
				file << "Distortion Coefficients" << ",";
				for (int i = 0; i < distortionCoeffs.cols; i++) {
					file << distortionCoeffs.at<double>(i, 0) << ", ";
				}
				file << "\n";
				file.close();
			}

			key = ' ';
		}



		if (key == 'p') {

			bool patternFound = cv::findChessboardCorners(grey, cv::Size(9, 6), cornerSet, cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE
				+ cv::CALIB_CB_FAST_CHECK);

			if (patternFound) {

				for (int i = 0; i < 6; i++) {
					for (int j = 0; j < 9; j++) {

						cv::Point3f v = { float(j), float(-i), float(0) };
						pointSet.push_back(v);
					}
				}

				cv::Mat rVec, tVec;
				cv::solvePnP(pointSet, cornerSet, cameraMatrix, distortionCoeffs, rVec, tVec); //Find 3D to 2D object pose

				//Print the rotation matrix
				cv::Mat rotMat;
				cv::Rodrigues(rVec, rotMat);

				//Print the rotation and translation vectors/matrix
				cout << "Rotation Matrix:\n" << rotMat << endl;
				cout << "Translation Vector:\n" << tVec << endl;

				//////////////   Task 5: Project the points on an image plane //////////////

				cv::projectPoints(pointSet, rotMat, tVec, cameraMatrix, distortionCoeffs, imgPoints);

				//Show the four outside corners of the chessboard
				cv::circle(projImg, imgPoints[0], 4, cv::Scalar(0, 0, 255), -1);
				cv::circle(projImg, imgPoints[8], 4, cv::Scalar(0, 0, 255), -1);
				cv::circle(projImg, imgPoints[45], 4, cv::Scalar(0, 0, 255), -1);
				cv::circle(projImg, imgPoints[53], 4, cv::Scalar(0, 0, 255), -1);

				//Display 3D axis at the origin of the chessboard
				cv::drawFrameAxes(projImg, cameraMatrix, distortionCoeffs, rVec, tVec, 2);

			}
			else {
				key = ' ';
			}
		}

		// Task 6
		if (key == 'd') {

			bool patternFound = cv::findChessboardCorners(grey, cv::Size(9, 6), cornerSet, cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE
				+ cv::CALIB_CB_FAST_CHECK);

			if (patternFound) {
				for (int i = 0; i < 6; i++) {
					for (int j = 0; j < 9; j++) {
						cv::Point3f v = { float(j), float(-i), float(0) };
						pointSet.push_back(v);
					}
				}

				cv::Mat rVec, tVec;
				cv::solvePnP(pointSet, cornerSet, cameraMatrix, distortionCoeffs, rVec, tVec);
				cv::Mat rotMat;
				cv::Rodrigues(rVec, rotMat);

				vector<vector<cv::Point3d>> endPtMat;
				vector<cv::Point2d> pts;
				vector<cv::Point2d> center;

				createLines(endPtMat); //Get the vector containing the end point coordinates of all the lines of the virtual object

				//Render the virtual object in the scene
				for (int i = 0; i < endPtMat.size(); i++) {
					cv::projectPoints(endPtMat[i], rotMat, tVec, cameraMatrix, distortionCoeffs, pts);
					cv::line(projImg, pts[0], pts[1], cv::Scalar(255, 0, 0), 2);
				}

			}
			else {
				key = ' ';
			}
		}

		////////////////    Extension    /////////////////

		if (key == 'e') {

			bool patternFound = cv::findChessboardCorners(grey, cv::Size(9, 6), cornerSet, cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE
				+ cv::CALIB_CB_FAST_CHECK);

			if (patternFound) {
				for (int i = 0; i < 6; i++) {
					for (int j = 0; j < 9; j++) {
						cv::Point3f v = { float(j), float(-i), float(0) };
						pointSet.push_back(v);
					}
				}

				cv::Mat rVec, tVec;
				cv::solvePnP(pointSet, cornerSet, cameraMatrix, distortionCoeffs, rVec, tVec);
				cv::Mat rotMat;
				cv::Rodrigues(rVec, rotMat);

				vector<vector<cv::Point3d>> endPtMat;
				vector<cv::Point2d> pts;
				vector<cv::Point2d> center;

				//Render the cube in the scene and animate it by moving it around in the scene
				static float start = 0.0;

				int temp = start / 26.0;
				if (temp) {
					start = 0.0;
					
				}
				float speed = 0.1;
				start += speed;

				createShape(endPtMat, start); 
				
				for (int i = 0; i < endPtMat.size(); i++) {
					cv::projectPoints(endPtMat[i], rotMat, tVec, cameraMatrix, distortionCoeffs, pts);
					cv::line(projImg, pts[0], pts[1], cv::Scalar(255, 0, 0), 2);
				}

			}
			else {
				key = ' ';
			}
	
		}
		

		//Display all the frames
		cv::imshow("Video Frame", frame);
		cv::imshow("Corners", frame);
		cv::imshow("CheckerBoard", grey);
		cv::imshow("Projected Points", projImg);

		if (key == 'q') {
			cv::destroyAllWindows();
			break;
		}

	}


	return 0;
}
