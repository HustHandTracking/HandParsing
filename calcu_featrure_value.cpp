#include <opencv2\opencv.hpp>
#include <iostream>
#include "calcu_feature_value.h"


/*��������������ֵ,��������ӳ����ʵ��ƫ�ƾ��룬����ת��Ϊƫ�Ƶ�����*/
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
			float Xdelta = (*sample_metrix).at<Vec2f>(iRows, jCols)[0];  //Xdelta Ϊ��ֱ�����ƫ�ƣ���Ϊ (*sample_metrix).at<Vec2f>(iRows, jCols)[1]
			Xdelta *= GRID_SIZE;
			int Xnew = x + (int)(0.5 + Xdelta * zNear / z); //��������

			if (Xnew > height - 1)
			{
				Xnew = height - 1;
			}
			if (Xnew < 0)
			{
				Xnew = 0;
			}

			float Ydelta = (*sample_metrix).at<Vec2f>(iRows, jCols)[1]; //Ydelta Ϊˮƽ�����ƫ��
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