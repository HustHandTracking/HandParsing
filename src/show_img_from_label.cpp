#include "show_img_from_label.h"
#include "get_pixel_label.h"
#include "predict.h"

void show_img(Mat& img, Mat& label)
{
	int rows = img.rows, cols = img.cols;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			int l = label.at<uchar>(i, j);
			if (l == PALM)
			{
				img.at<Vec3b>(i, j)[0] = PALM_RGB;
				img.at<Vec3b>(i, j)[1] = PALM_RGB;
				img.at<Vec3b>(i, j)[2] = PALM_RGB;
			}
			else if (l == THUMB_1)
			{
				img.at<Vec3b>(i, j)[0] = 0;
				img.at<Vec3b>(i, j)[1] = 0;
				img.at<Vec3b>(i, j)[2] = THUMB_PIP_R;

			}
			else if (l == THUMB_2)
			{
				img.at<Vec3b>(i, j)[0] = 0;
				img.at<Vec3b>(i, j)[1] = 0;
				img.at<Vec3b>(i, j)[2] = THUMB_DIP_R;
			}
			else if (l == INDEX_1)
			{
				img.at<Vec3b>(i, j)[0] = 0;
				img.at<Vec3b>(i, j)[1] = INDEX_MCP_DIP_G;
				img.at<Vec3b>(i, j)[2] = 100;

			}
			else if (l == INDEX_2)
			{
				img.at<Vec3b>(i, j)[0] = 3;
				img.at<Vec3b>(i, j)[1] = INDEX_DIP_G;
				img.at<Vec3b>(i, j)[2] = 150;

			}
			else if (l == MIDDLE_1)
			{
				img.at<Vec3b>(i, j)[0] = 0;
				img.at<Vec3b>(i, j)[1] = MIDDLE_MCP_PIP_G;
				img.at<Vec3b>(i, j)[2] = 0;

			}
			else if (l == MIDDLE_2)
			{
				img.at<Vec3b>(i, j)[0] = 200;
				img.at<Vec3b>(i, j)[1] = MIDDLE_DIP_G;
				img.at<Vec3b>(i, j)[2] = 0;
			}
			else if (l == RING_1)
			{
				img.at<Vec3b>(i, j)[0] = RING_MCP_PIP_B;
				img.at<Vec3b>(i, j)[1] = 50;
				img.at<Vec3b>(i, j)[2] = 10;
			}
			else if (l == RING_2)
			{
				img.at<Vec3b>(i, j)[0] = RING_DIP_B;
				img.at<Vec3b>(i, j)[1] = 200;
				img.at<Vec3b>(i, j)[2] = 30;
			}
			else if (l == PINGKY_1)
			{
				img.at<Vec3b>(i, j)[0] = PINKY_MCP_PIP_B;
				img.at<Vec3b>(i, j)[1] = 80;
				img.at<Vec3b>(i, j)[2] = 30;
			}
			else if (l == PINGKY_2)
			{
				img.at<Vec3b>(i, j)[0] = PINGKY_DIP_B;
				img.at<Vec3b>(i, j)[1] = 50;
				img.at<Vec3b>(i, j)[2] = 200;
			}
			else
			{
				img.at<Vec3b>(i, j)[0] = 0;
				img.at<Vec3b>(i, j)[1] = 0;
				img.at<Vec3b>(i, j)[2] = 0;
			}
		}
	}

}
