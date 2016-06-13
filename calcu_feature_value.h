#ifndef CALCU_FEATURE_VALUE_H_
#define CALCU_FEATURE_VALUE_H_

#include "camera_info.h"
#include "sample_metrix.h"

#define SIGMA 300 //mm
//#define GRID_SIZE 15  //mm

//#define M 10

using namespace cv;
using namespace std;
int calcu_f_value(Mat *cur_frame, Mat *sample_metrix, int x, int y, Mat* p_value);

#endif
