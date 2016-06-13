#pragma once

#include <opencv2\opencv.hpp>
#include "gl_define.h"
#include "calcu_frame_feature.hpp"
#include "get_pixel_label.h"
class Parsing
{
public:
	Parsing();
	~Parsing();

	//void setData(Mat&);
	void getImg(Mat&);
	void predic(void);

	Mat joint;
	void extract_joint(Mat& img);
	void show_img(Mat& img);
private:
	//cv::Mat prob;

	double result;

	cv::Mat cur_frame;
	cv::Mat pixel_prob;
	cv::Mat result_label;
	//cv::Mat test_sample;

	FValue *fvalue;
	CvRTrees *rtrees;
	int *hand_jointx;
	int *hand_jointy;
	int *hand_joint_cnt;

	int xhis[CLASS_NUM - 1][HEIGHT];
	int yhis[CLASS_NUM - 1][WIDTH];
	Point j[CLASS_NUM];

	unsigned short *buffer;

};
