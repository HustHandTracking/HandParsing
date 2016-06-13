#ifndef CALCU_FRAME_FEATURE_H
#define CALCU_FRAME_FEATURE_H

#include <iostream>
#include <opencv2\opencv.hpp>
#define  INVALID_PIXEL  -1000
#define SIGMA 200 //mm
#define zNear 200
#define zFar 1200

#define GRID_SIZE 10  //mm

using namespace std;
using namespace cv;

class FValue
{
public:
	FValue();
	~FValue();

	void calcu_frame_feature(Mat *p_frame,CvRTrees*);

	int calcu_f_value(Mat *cur_frame, int& x, int& y, Mat& p_value, CvRTrees*);
	Mat fvalue;
private:
	int  M ;
	int DIM;
	Mat sample_metrix;

	int init_sample_metrix(void);
	void modify_sample_metrix(void);
};


#endif
