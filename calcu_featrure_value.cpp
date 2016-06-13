#include <opencv2\opencv.hpp>
#include <iostream>
#include "calcu_feature_value.h"


/*计算输入点的特征值,采样矩阵反映的是实际偏移距离，需先转换为偏移的坐标*/
int calcu_f_value(Mat *cur_frame, Mat *sample_metrix, int x, int y, Mat* p_value)
{
	int metrix_dim = (*sample_metrix).rows;
	int value_dim = (*p_value).cols;
	if (metrix_dim * metrix_dim != value_dim)
	{
		cout << "The dim of the metrix is not fit the dim of feature value" << endl;
		return -4;
	}

	int z = (*cur_frame).at<ushort>(x, y);
	int height = (*cur_frame).rows;
	int width = (*cur_frame).cols;

	for (int iRows = 0; iRows < metrix_dim; iRows ++)
	{
		for (int jCols = 0; jCols < metrix_dim; jCols++)
		{
			float Xdelta = (*sample_metrix).at<Vec2f>(iRows, jCols)[0];  //Xdelta 为竖直方向的偏移，故为 (*sample_metrix).at<Vec2f>(iRows, jCols)[1]
			Xdelta *= GRID_SIZE;
			int Xnew = x + (int)(0.5 + Xdelta * zNear / z); //四舍五入

			if (Xnew > height - 1)
			{
				Xnew = height - 1;
			}
			if (Xnew < 0)
			{
				Xnew = 0;
			}

			float Ydelta = (*sample_metrix).at<Vec2f>(iRows, jCols)[1]; //Ydelta 为水平方向的偏移
			Ydelta *= GRID_SIZE;
			int Ynew = y + (int)(0.5 + Ydelta * zNear / z);
			if (Ynew > width - 1)
			{
				Ynew = width - 1;
			}
			if (Ynew < 0)
			{
				Ynew = 0;
			}

			int value_tmp = z - (*cur_frame).at<ushort>(Xnew, Ynew);
			if (value_tmp > SIGMA)
			{
				value_tmp = SIGMA;
			}
			if (value_tmp < -SIGMA)
			{
				value_tmp = -SIGMA;
			}
			(*p_value).at<float>(0, iRows* metrix_dim + jCols) = value_tmp;
		}
	}
	return 1;
}