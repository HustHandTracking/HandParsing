#include"sample_metrix.h"

/*
    Input : Mar *metrix, M x M, M is odd.
           metrix 是一个2通道的Mat型数据，保存了采用矩阵 
	Output: None
    return:
	       -2 The sample metrix`s rows should be equal to cols
		   -3 The sample metrix`s size should be odd
		    1 done
*/

int init_sample_metrix(Mat *metrix)
{
	if ((*metrix).rows != (*metrix).cols)
	{
		cout << "The sample metrix`s rows should be equal to cols\n" << endl;
		return -2;
	}
	else if ((*metrix).rows % 2 != 1)
	{
		cout << "The sample metrix`s size should be odd\n" << endl;
		return -3;
	}

	int M = ((*metrix).rows - 1)/2;
	
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
				(*metrix).at<Vec2f>(iRows, jCols)[0] = tmp2.at<Vec2f>(iRows, jCols)[0];
				(*metrix).at<Vec2f>(iRows, jCols)[1] = tmp2.at<Vec2f>(iRows, jCols)[1];

			}

			//left down -+
			if ((iRows >= M  && iRows < 2 * M + 1) && (jCols >= 0 && jCols <= M))
			{
				(*metrix).at<Vec2f>(iRows, jCols)[0] = tmp3.at<Vec2f>(iRows - M, jCols)[0];
				(*metrix).at<Vec2f>(iRows, jCols)[1] = tmp3.at<Vec2f>(iRows - M, jCols)[1];
			}

			//right up
			if ((iRows >= 0 && iRows <= M) && (jCols >= M && jCols < 2 * M + 1))
			{
				(*metrix).at<Vec2f>(iRows, jCols)[0] = tmp1.at<Vec2f>(iRows, jCols - M)[0];
				(*metrix).at<Vec2f>(iRows, jCols)[1] = tmp1.at<Vec2f>(iRows, jCols - M)[1];

			}
			//right down
			if ((iRows >= M  && iRows < 2 * M + 1) && (jCols >= M && jCols < 2 * M + 1))
			{
				(*metrix).at<Vec2f>(iRows, jCols)[0] = tmp4.at<Vec2f>(iRows - M, jCols - M)[0];
				(*metrix).at<Vec2f>(iRows, jCols)[1] = tmp4.at<Vec2f>(iRows - M, jCols - M)[1];
			}
		}
	}
	return 1;
}
