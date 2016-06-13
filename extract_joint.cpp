#include "predict.h"

void Parsing::extract_joint(Mat& img)
{
	int cntx = 0;
	int lx = 0;
	int mlx = 0;
	int wx = 0;
	int mwx = 0;
	int mcntx = 0;
	int sx = 0;
	int th = 0;

	int cnty = 0;
	int ly = 0;
	int mly = 0;
	int wy = 0;
	int mwy = 0;
	int mcnty = 0;
	int sy = 0;

	for (int i = 0; i < CLASS_NUM - 1; i++)
	{
	cntx = 0;
	lx = 0;
	mlx = 0;
	wx = 0;
	mwx = 0;
	mcntx = 0;
	sx = 0;
	th = 0;
	cnty = 0;
	ly = 0;
	mly = 0;
	wy = 0;
	mwy = 0;
	mcnty = 0;
	sy = 0;
		for (int x = 0; x < HEIGHT; x++)
		{
			if (th < xhis[i][x])
			{
				th = xhis[i][x];
			}
		}

		th /= 2;
		for (int x = 0; x < HEIGHT; x++)
		{
			if (sx == 0)
			{
				if (xhis[i][x] > th)
				{
					cntx += x*xhis[i][x];
					wx += xhis[i][x];
					lx++;
					sx = 1;
				}
			}
			else if (sx == 1)
			{
				if (xhis[i][x] <= th)
				{
					sx = 0;
					if (mlx < lx)
					{
						mlx = lx;
						mcntx = cntx;
						mwx = wx;
						cntx = 0;
						lx = 0;
						wx = 0;
					}
				}
				else
				{
					cntx += x*xhis[i][x];
					wx += xhis[i][x];
					lx++;
				}
			}
		}

		for (int y = 0; y < WIDTH; y++)
		{
			if (th < yhis[i][y])
			{
				th = yhis[i][y];
			}
		}
		th /= 2;
		for (int y = 0; y < WIDTH; y++)
		{
			if (sy == 0)
			{
				if (yhis[i][y] > th)
				{
					cnty += y*yhis[i][y];
					wy += yhis[i][y];
					ly++;
					sy = 1;
				}
			}
			else if (sy == 1)
			{
				if (yhis[i][y] <= th)
				{
					sy = 0;
					if (mly < ly)
					{
						mly = ly;
						mcnty = cnty;
						mwy = wy;
						cnty = 0;
						ly = 0;
						wy = 0;
					}
				}
				else
				{
					cnty += y*yhis[i][y];
					wy += yhis[i][y];
					ly++;
				}
			}

		}

		if (mwx == 0)
		{
			mwx = 1;
		}
		if (mwy == 0)
		{
			mwy = 1;
		}
		j[i].y = (int)mcntx / mwx;
		j[i].x = WIDTH - 1 - (int)mcnty / mwy;

		cv::circle(img, j[i], 5, Scalar(139,250,86));
		//cv::circle(img, j[i], 5, Scalar(65530));
	}

	cv::line(img, j[0], j[1], Scalar(139,250,86), 2);
	cv::line(img, j[1], j[2], Scalar(139,250,86), 2);


	cv::line(img, j[0], j[3], Scalar(139,250,86), 2);
	cv::line(img, j[3], j[4], Scalar(139,250,86), 2);

	cv::line(img, j[0], j[5], Scalar(139,250,86), 2);
	cv::line(img, j[5], j[6], Scalar(139,250,86), 2);

	cv:: line(img, j[0], j[7], Scalar(139,250,86), 2);
	cv::line(img, j[7], j[8], Scalar(139,250,86), 2);

	cv::line(img, j[0], j[9], Scalar(139,250,86), 2);
	cv::line(img, j[9], j[10], Scalar(139,250,86), 2);

	/*cv::line(img, j[0], j[1], Scalar(65530), 2);
	cv::line(img, j[1], j[2], Scalar(65530), 2);


	cv::line(img, j[0], j[3], Scalar(65530), 2);
	cv::line(img, j[3], j[4], Scalar(65530), 2);

	cv::line(img, j[0], j[5], Scalar(65530), 2);
	cv::line(img, j[5], j[6], Scalar(65530), 2);

	cv:: line(img, j[0], j[7], Scalar(65530), 2);
	cv::line(img, j[7], j[8], Scalar(65530), 2);

	cv::line(img, j[0], j[9], Scalar(65530), 2);
	cv::line(img, j[9], j[10], Scalar(65530), 2);
*/

}