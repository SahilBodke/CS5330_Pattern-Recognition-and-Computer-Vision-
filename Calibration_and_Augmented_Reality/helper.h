/*
	Sahil Kiran Bodke
	Spring 2023 : CS5300 - Pattern Recognition and Computer Vision
	Project 4 : Calibration and Augmented Reality
*/

#pragma once

#include<opencv2/core.hpp>

int greyscale(cv::Mat& src, cv::Mat& dst);

std::vector<std::vector<cv::Point3d>> createLines(std::vector<std::vector<cv::Point3d>>& endPts);

std::vector<std::vector<cv::Point3d>> createShape(std::vector<std::vector<cv::Point3d>>& endPts, float start);
