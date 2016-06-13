#ifndef GET_PIXEL_LABEL_H
#define GET_PIXEL_LABEL_H

#include <opencv2\opencv.hpp>
using namespace std;
using namespace cv;

#define PALM_RGB 255

#define THUMB_PIP_R 102
#define THUMB_DIP_R 204

#define INDEX_MCP_DIP_G 51
#define INDEX_DIP_G     102

#define MIDDLE_MCP_PIP_G 153
#define MIDDLE_DIP_G     204

#define RING_MCP_PIP_B 51
#define RING_DIP_B     102

#define PINKY_MCP_PIP_B 153
#define PINGKY_DIP_B    204

#define PALM 0
#define THUMB_1 1
#define THUMB_2 2
#define INDEX_1 3
#define INDEX_2 4
#define MIDDLE_1 5
#define MIDDLE_2 6
#define RING_1   7
#define RING_2   8
#define PINGKY_1 9
#define PINGKY_2 10
#define BACKGROUND 11


void get_pixel_label(Mat& img, Mat& classification);

#endif
