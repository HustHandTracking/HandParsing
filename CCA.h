#pragma once

#include <iostream>  
#include <string>  
#include <list>  
#include <vector>  
#include <map>  
#include <stack>  
#include <utility>

#include <opencv2\opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/highgui/highgui.hpp>  

#include <pxcsensemanager.h>
#include <pxcsession.h>
#include "util_render.h"


struct CCA
{
	float avg_d;
	int x;
	int y;
	int cca_cnt;
	int label;
};


void icvprCcaBySeedFill(PXCImage* depthIm, PXCCapture::Sample*sample, cv::Mat &img);
