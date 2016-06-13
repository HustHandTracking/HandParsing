#include "calcu_frame_feature.hpp"
#include "gl_define.h"
#include <cmath>
#include <omp.h>
/*
p_featrue (rows*cols) x ((2*M+1)*(2*M+1))
*/


FValue::FValue()
{
	M = 10;
	DIM = 2 * M + 1;
	sample_metrix = Mat(DIM, DIM, CV_32FC2);
	init_sample_metrix();
	modify_sample_metrix();
	fvalue = Mat::zeros(WIDTH*HEIGHT, DIM*DIM , CV_32FC1);
}

FValue::~FValue()
{
}


/*
    Input : Mar *metrix, M x M, M is odd.
           metrix 是一个2通道的Mat型数据，保存了采用矩阵 
	Output: None
    return:
	       -2 The sample metrix`s rows should be equal to cols
		   -3 The sample metrix`s size should be odd
		    1 done
*/

int FValue::init_sample_metrix(void)
{
	if (sample_metrix.rows != sample_metrix.cols)
	{
		cout << "The sample metrix`s rows should be equal to cols\n" << endl;
		return -2;
	}
	else if (sample_metrix.rows % 2 != 1)
	{
		cout << "The sample metrix`s size should be odd\n" << endl;
		return -3;
	}

	int M = (sample_metrix.rows - 1)/2;
	
	Mat tmp1 = Mat::zeros(M + 1, M + 1, CV_32FC2);
	Mat tmp2 = Mat::zeros(M + 1, M + 1, CV_32FC2);
	Mat tmp3 = Mat::zeros(M + 1, M + 1, CV_32FC2);
	Mat tmp4 = Mat::zeros(M + 1, M + 1, CV_32FC2);

	for (int iRows = 0; iRows < M + 1; iRows++)
	{
		for (int jCols = 0; jCols < M + 1; jCols++)
		{
			float xd = (float)(M * (1 - sqrt((M - iRows) / (float)M)));
			float yd = (float)(M * (1 - sqrt((M - jCols) / (float)M)));
			 
			//right up -+
			tmp1.at<Vec2f>(M - iRows, jCols)[0] = -xd;
			tmp1.at<Vec2f>(M - iRows, jCols)[1] =  yd;

			//left up --
			tmp2.at<Vec2f>(M - iRows, M - jCols)[0] = -xd;
			tmp2.at<Vec2f>(M - iRows, M - jCols)[1] = -yd;

            //left down +-
			tmp3.at<Vec2f>(iRows, M - jCols)[0] =  xd;
			tmp3.at<Vec2f>(iRows, M - jCols)[1] = -yd;

			//right down ++
			tmp4.at<Vec2f>(iRows, jCols)[0] = xd;
			tmp4.at<Vec2f>(iRows, jCols)[1] = yd;
		}
	}


	for (int iRows = 0; iRows < 2 * M + 1; iRows ++)
	{
		for (int jCols = 0; jCols < 2 * M + 1; jCols ++)
		{ 
			//left up --
			if ((iRows >= 0 && iRows <= M) && (jCols >= 0 && jCols <= M))
			{
				sample_metrix.at<Vec2f>(iRows, jCols)[0] = tmp2.at<Vec2f>(iRows, jCols)[0];
				sample_metrix.at<Vec2f>(iRows, jCols)[1] = tmp2.at<Vec2f>(iRows, jCols)[1];
			}

			//left down -+
			if ((iRows >= M  && iRows < 2 * M + 1) && (jCols >= 0 && jCols <= M))
			{
				sample_metrix.at<Vec2f>(iRows, jCols)[0] = tmp3.at<Vec2f>(iRows - M, jCols)[0];
				sample_metrix.at<Vec2f>(iRows, jCols)[1] = tmp3.at<Vec2f>(iRows - M, jCols)[1];
			}

			//right up
			if ((iRows >= 0 && iRows <= M) && (jCols >= M && jCols < 2 * M + 1))
			{
				sample_metrix.at<Vec2f>(iRows, jCols)[0] = tmp1.at<Vec2f>(iRows, jCols - M)[0];
				sample_metrix.at<Vec2f>(iRows, jCols)[1] = tmp1.at<Vec2f>(iRows, jCols - M)[1];

			}
			//right down
			if ((iRows >= M  && iRows < 2 * M + 1) && (jCols >= M && jCols < 2 * M + 1))
			{
				sample_metrix.at<Vec2f>(iRows, jCols)[0] = tmp4.at<Vec2f>(iRows - M, jCols - M)[0];
				sample_metrix.at<Vec2f>(iRows, jCols)[1] = tmp4.at<Vec2f>(iRows - M, jCols - M)[1];
			}
		}
	}
	return 1;
}

void FValue::modify_sample_metrix(void)
{
	int row = sample_metrix.rows;
	int col = sample_metrix.cols;
	
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			float Xdelta = sample_metrix.at<Vec2f>(i, j)[0];  //Xdelta 为竖直方向的偏移，故为 (*sample_metrix).at<Vec2f>(iRows, jCols)[1]
			Xdelta *= GRID_SIZE;
			sample_metrix.at<Vec2f>(i,j)[0] =  Xdelta * zNear;

			float Ydelta = sample_metrix.at<Vec2f>(i, j)[1];  //Xdelta 为竖直方向的偏移，故为 (*sample_metrix).at<Vec2f>(iRows, jCols)[1]
			Ydelta *= GRID_SIZE;
			sample_metrix.at<Vec2f>(i,j)[1] =  Ydelta * zNear;
		}
	}
}

/*计算输入点的特征值,采样矩阵反映的是实际偏移距离，需先转换为偏移的坐标*/
int FValue:: calcu_f_value(Mat *cur_frame, int& x, int& y, Mat& p_value,CvRTrees* rtree)
{
	int metrix_dim = sample_metrix.rows;
	int value_dim = (p_value).cols;
	if (metrix_dim * metrix_dim != value_dim)
	{
		cout << "The dim of the metrix is not fit the dim of feature value" << endl;
		return -4;
	}

	int z = (*cur_frame).at<ushort>(x, y);
	int height = (*cur_frame).rows;
	int width = (*cur_frame).cols;


    #pragma omp parallel for
	for (int dim = 0; dim < metrix_dim*metrix_dim; dim++)
	{
		int iRows = dim / metrix_dim;
		int jCols = dim % metrix_dim;
		//if (rtree->get_FeatureNum(dim))
		{
			float Xdelta = sample_metrix.at<Vec2f>(iRows, jCols)[0];  //Xdelta 为竖直方向的偏移，故为 (*sample_metrix).at<Vec2f>(iRows, jCols)[1]
			int Xnew = x + (int)(0.5 + Xdelta / z); //四舍五入

			if (Xnew > height - 1)
			{
				Xnew = height - 1;
			}
			if (Xnew < 0)
			{
				Xnew = 0;
			}

			float Ydelta = sample_metrix.at<Vec2f>(iRows, jCols)[1]; //Ydelta 为水平方向的偏移
			int Ynew = y + (int)(0.5 + Ydelta / z);

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
				value_tmp = SIGMA;
			}
			(p_value).at<float>(0, dim) = (float)value_tmp;
		}
	}
	return 1;
}

void FValue::calcu_frame_feature(Mat *p_frame, CvRTrees* rtree)
{
	int rows = (*p_frame).rows;
	int cols = (*p_frame).cols;

	Mat per_pixel_value;

	for (int iRows = 0; iRows < rows; iRows++)
	{
		for (int jCols = 0; jCols < cols; jCols++)
		{
			//int pixel_depth =fvalue.at<ushort>(iRows,jCols);
			per_pixel_value = fvalue.row(iRows * cols+ jCols);

			{
				calcu_f_value(p_frame, iRows, jCols, per_pixel_value, rtree);
			}
			/*if (abs(pixel_depth - zFar) > 100)
			{ 
				calcu_f_value(p_frame, iRows, jCols, &per_pixel_value,rtree);
			}
			else
			{
				per_pixel_value.at<float>(0, 0) = 0;
			}*/
		}
	}
}
