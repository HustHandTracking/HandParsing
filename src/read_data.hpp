#pragma once

#include <fstream>
#include <opencv2\opencv.hpp> // opencv general include file
#include <opencv2\ml\ml.hpp>// opencv machine learning include file
#include <stdio.h>
#include <iostream>
#include "gl_define.h"
using namespace std;
using namespace cv;


int read_data_from_dat(const char* samplePath, Mat data, Mat classes, struct rtreeparams* params, int Type);
int read_data_from_dat(const char* samplePath, Mat data, Mat classes, Mat coord, struct rtreeparams* params, int Type);
int read_data_from_dat(const char* samplePath, struct rtreeparams* c, int Type);
