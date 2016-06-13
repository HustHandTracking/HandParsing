/*
   get pixel label from relative RGB PNG image
   the label range 0~10
   the classification must be Mat(rows,cols,CV_8UC1,Scalar(13));
*/

#include "get_pixel_label.h"

void get_pixel_label(Mat& img, Mat& classification)
{
	int rows = img.rows;
	int cols = img.cols;
   
	
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			int B = img.at<Vec3b>(i, j)[0];
			int G = img.at<Vec3b>(i, j)[1];
			int R = img.at<Vec3b>(i, j)[2];

			if (B == PALM_RGB && G == PALM_RGB && R == PALM_RGB)
			{
				classification.at<uchar>(i, j) = PALM;
			}
			else if (R > THUMB_PIP_R / 2 && R <= (THUMB_PIP_R + THUMB_DIP_R) / 2)
			{
				classification.at<uchar>(i, j) = THUMB_1;
			}
			else if (R >  (THUMB_PIP_R + THUMB_DIP_R) / 2)
			{
				classification.at<uchar>(i, j) = THUMB_2;
			}
			else if (G >INDEX_MCP_DIP_G / 2  &&   G <= (INDEX_MCP_DIP_G + INDEX_DIP_G)/2)
			{
				classification.at<uchar>(i, j) = INDEX_1;
			}
			else if (G > (INDEX_MCP_DIP_G + INDEX_DIP_G)/2 && G <= (INDEX_DIP_G + MIDDLE_MCP_PIP_G)/2)
			{
				classification.at<uchar>(i, j) = INDEX_2;
			}
			else if (G > (INDEX_DIP_G + MIDDLE_MCP_PIP_G)/2 && G <= (MIDDLE_MCP_PIP_G + MIDDLE_DIP_G)/2)
			{
				classification.at<uchar>(i, j) = MIDDLE_1;
			}
			else if (G > (MIDDLE_MCP_PIP_G + MIDDLE_DIP_G)/2)
			{
				classification.at<uchar>(i, j) = MIDDLE_2;
			}
			else if (B >RING_MCP_PIP_B / 2 &&  B <= (RING_MCP_PIP_B+RING_DIP_B)/2)
			{
				classification.at<uchar>(i, j) = RING_1;
			}
			else if (B > (RING_MCP_PIP_B+RING_DIP_B)/2 && B < (RING_DIP_B + PINKY_MCP_PIP_B)/2)
			{
				classification.at<uchar>(i, j) = RING_2;
			}
			else if (B >  (RING_DIP_B + PINKY_MCP_PIP_B)/2 && B < (PINKY_MCP_PIP_B + PINGKY_DIP_B)/2)
			{
				classification.at<uchar>(i, j) = PINGKY_1;
			}
			else if (B > (PINKY_MCP_PIP_B + PINGKY_DIP_B)/2)
			{
				classification.at<uchar>(i, j) = PINGKY_2;
			}
			else
			{
				classification.at<uchar>(i, j) = BACKGROUND;
			}
		}
	}
	
}
