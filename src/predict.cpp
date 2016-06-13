#include "predict.h" 
#include "read_data.hpp"

Parsing::Parsing()
{
	pixel_prob = cv::Mat::zeros(WIDTH*HEIGHT, CLASS_NUM, CV_64FC1);
	result_label = cv::Mat(HEIGHT, WIDTH, CV_8UC1, Scalar(BACKGROUND));
	result = 0;
	buffer = 0;
	fvalue = 0;
	
	hand_jointx = NULL;
	hand_jointy = NULL;
	hand_joint_cnt = NULL;

	fvalue = new FValue;
	rtrees = new CvRTrees;
	
	hand_jointx = new int[CLASS_NUM - 1];
	hand_jointy = new int[CLASS_NUM - 1];
	hand_joint_cnt = new int[CLASS_NUM - 1];

	for (int i = 0; i < CLASS_NUM - 1; i++)
	{
		hand_jointx[i] = 0;
		hand_jointy[i] = 0;
		hand_joint_cnt[i] = 0;
	}

	for (int i = 0; i < CLASS_NUM - 1; i++)
	{
		j[i].x = 0;
		j[i].y = 0;

		for (int j = 0; j < HEIGHT; j++)
		{
			xhis[i][j] = 0;
		}

		for (int k = 0; k < HEIGHT; k++)
		{
			yhis[i][k] = 0;
		}
	}

	joint = Mat::zeros(HEIGHT, WIDTH, CV_8UC1);
	
	cout << "Check these parameters first: " << endl;
	cout << "GRID_SIZE: " << GRID_SIZE << endl;
	cout << "SIGMA: " << SIGMA << endl;
	cout << "zNear: " << zNear << endl;
	cout << "zFar: " << zFar << endl;
	cout << "Load the treeModel_10.xml ..." << endl;

	rtrees->load("treeModel_10.xml");
}

Parsing::~Parsing()
{
	delete []fvalue;
	fvalue = 0;

	delete []buffer;
	buffer = 0;

	if (hand_jointx)
		delete[]hand_jointx;

	if (hand_jointy)
		delete[]hand_jointy;

	if (hand_joint_cnt)
		delete[]hand_joint_cnt;

//	delete[]rtrees;
	cvFree(&rtrees);
}

/*void Parsing::setData(Mat &img)
{
	int rows = img.rows;
	int cols = img.cols;
	
	pixel_prob = cv::Mat::zeros(rows*cols, CLASS_NUM, CV_64FC1);
	result_label = cv::Mat(rows, cols, CV_8UC1, Scalar(BACKGROUND));
}*/

void Parsing::getImg(Mat&img)
{
	cur_frame = img;
}

void Parsing::predic(void)
{
	memset(xhis, 0, (CLASS_NUM - 1)*HEIGHT*sizeof(int));
	memset(yhis, 0, (CLASS_NUM - 1)*WIDTH*sizeof(int));

#pragma omp parallel for
	for (int i = 0; i < WIDTH*HEIGHT; i++)
	{
		int pixel_row = i / WIDTH, pixel_col = i % WIDTH;

		if (cur_frame.at<ushort>(pixel_row, pixel_col) != 0) //the background pixel value is 0
		{
			if (pixel_row % 2 == 0 && pixel_col % 2 == 0)
			{
				Mat test_sample = fvalue->fvalue.row(i);
				Mat prob = pixel_prob.row(i);

				fvalue->calcu_f_value(&cur_frame, pixel_row, pixel_col, test_sample, rtrees);

				result = rtrees->my_predict(prob, test_sample, Mat());
				result_label.at<uchar>(pixel_row, pixel_col) = (uchar)result;
			}
			else if (pixel_col % 2 != 0)
			{
				result_label.at<uchar>(pixel_row, pixel_col) = result_label.at<uchar>(pixel_row, pixel_col - 1);
			}
			else if (pixel_row % 2 != 0)
			{
				result_label.at<uchar>(pixel_row, pixel_col) = result_label.at<uchar>(pixel_row - 1, pixel_col);
			}
			/*else
			{
				result_label.at<uchar>(pixel_row, pixel_col) = result_label.at<uchar>(pixel_row - 1, pixel_col - 1);
			}*/

			int coord = result_label.at<uchar>(pixel_row, pixel_col);
			xhis[(int)coord][pixel_row] ++;
			yhis[(int)coord][pixel_col] ++;
		}
		else
		{
			result_label.at<uchar>(pixel_row, pixel_col) = BACKGROUND;
		}
	}
}

void Parsing::show_img(Mat& img)
{
	int rows = img.rows, cols = img.cols;
//#pragma omp parallel for
	for (int k = 0; k < rows*cols; k++)
	{
		int i = k / cols;
		int j = k % cols;

		int l = result_label.at<uchar>(i, cols - 1 - j);
		if (l == PALM)
		{
			img.at<Vec3b>(i, j)[0] = 255;
			img.at<Vec3b>(i, j)[1] = 255;
			img.at<Vec3b>(i, j)[2] = 255;
		}
		else if (l == THUMB_1||l == THUMB_2)
		{
			img.at<Vec3b>(i, j)[0] = 92;
			img.at<Vec3b>(i, j)[1] = 70;
			img.at<Vec3b>(i, j)[2] = 226;
		}
		/*else if (l == THUMB_2)
		{
			img.at<Vec3b>(i, j)[0] = 59;
			img.at<Vec3b>(i, j)[1] = 51;
			img.at<Vec3b>(i, j)[2] = 107;
		}*/
		else if (l == INDEX_1 || l == INDEX_2)
		{
			img.at<Vec3b>(i, j)[0] = 61;
			img.at<Vec3b>(i, j)[1] = 145;
			img.at<Vec3b>(i, j)[2] = 109;

		}
		/*else if (l == INDEX_2)
		{
			img.at<Vec3b>(i, j)[0] = 86;
			img.at<Vec3b>(i, j)[1] = 71;
			img.at<Vec3b>(i, j)[2] = 89;

		}*/
		else if (l == MIDDLE_1 || l == MIDDLE_2)
		{
			img.at<Vec3b>(i, j)[0] = 240;
			img.at<Vec3b>(i, j)[1] = 13;
			img.at<Vec3b>(i, j)[2] = 30;

		}
		/*else if (l == MIDDLE_2)
		{
			img.at<Vec3b>(i, j)[0] = 135;
			img.at<Vec3b>(i, j)[1] = 87;
			img.at<Vec3b>(i, j)[2] = 76;
		}*/
		else if (l == RING_1 || l == RING_2)
		{
			img.at<Vec3b>(i, j)[0] = 148;
			img.at<Vec3b>(i, j)[1] = 125;
			img.at<Vec3b>(i, j)[2] = 191;
		}
		/*else if (l == RING_2)
		{
			img.at<Vec3b>(i, j)[0] = 54;
			img.at<Vec3b>(i, j)[1] = 232;
			img.at<Vec3b>(i, j)[2] = 245;
		}*/
		else if (l == PINGKY_1 || l == PINGKY_2)
		{
			img.at<Vec3b>(i, j)[0] = 250;
			img.at<Vec3b>(i, j)[1] = 229;
			img.at<Vec3b>(i, j)[2] = 153;
		}
		/*else if (l == PINGKY_2)
		{
			img.at<Vec3b>(i, j)[0] = 153;
			img.at<Vec3b>(i, j)[1] = 107;
			img.at<Vec3b>(i, j)[2] = 94;
		}*/
		else
		{
			img.at<Vec3b>(i, j)[0] = 0;
			img.at<Vec3b>(i, j)[1] = 0;
			img.at<Vec3b>(i, j)[2] = 0;
		}
	}
}
