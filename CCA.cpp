//Connected Component Analysis/Labeling By Seed-Filling Algorithm   
							   //  Author:  www.icvpr.com    
							   //  Blog  :  http://blog.csdn.net/icvpr   
#include "CCA.h"
#include "gl_define.h"
void icvprCcaBySeedFill(PXCImage* depthIm,PXCCapture::Sample*sample, cv::Mat &img)
{
	// connected component analysis (4-component)  
	// use seed filling algorithm  
	// 1. begin with a foreground pixel and push its foreground neighbors into a stack;  
	// 2. pop the top pixel on the stack and label it with the same label until the stack is empty  
	//   
	// foreground pixel: _binImg(x,y) = 1  
	// background pixel: _binImg(x,y) = 0  

	PXCImage::ImageData data;
	PXCImage::ImageInfo dinfo;

	cv::Mat _lableImg =cv::Mat(HEIGHT*2,WIDTH*2,CV_32SC1,cv::Scalar(0));
	if (depthIm->AcquireAccess(PXCImage::ACCESS_READ, &data) == PXC_STATUS_NO_ERROR)
	{
		dinfo = sample->depth->QueryInfo();
		ushort *dpixels = (ushort*)data.planes[0];
		int dpitch = data.pitches[0] / sizeof(ushort);


		/*for (int y = 0; y < (int)dinfo.height; y++)
		{
			for (int x = 0; x < (int)dinfo.width; x++)
			{
				ushort d = dpixels[y*dpitch + x];
			}
		}*/



		int label = 1;  // start by 2  

		int rows = (int)dinfo.height - 1;
		int cols = (int)dinfo.width - 1;

		CCA NearestCCA;
		NearestCCA.avg_d = 10000;
		NearestCCA.x = 0;
		NearestCCA.y = 0;
		NearestCCA.label = 0;

		for (int i = 1; i < rows - 1; i++)
		{
			for (int j = 1; j < cols - 1; j++)
			{
				ushort d = dpixels[i * dpitch + j];

				CCA cca;
				cca.cca_cnt = 0;
				cca.avg_d = 0;
				cca.x = 0;
				cca.y = 0;

				if (d >= 202 && d <= 700 && _lableImg.at<int>(i, j) == 0)
				{
					std::stack<std::pair<int, int>> neighborPixels;
					neighborPixels.push(std::pair<int, int>(i, j));     // pixel position: <i,j>  
					++label;  // begin with a new label  
					while (!neighborPixels.empty())
					{
						// get the top pixel on the stack and label it with the same label  
						std::pair<int, int> curPixel = neighborPixels.top();
						int curX = curPixel.first;
						int curY = curPixel.second;
						if (curX < rows - 1 && curX > 1 && curY > 1 && curY < cols - 1)
						{
							_lableImg.at<int>(curX, curY) = label;

							// pop the top pixel  
							neighborPixels.pop();
							cca.cca_cnt++;
							cca.avg_d += dpixels[curX*dpitch + curY];
							cca.x += curX; cca.y += curY;

							// push the 4-neighbors (foreground pixels)  

							int value = dpixels[curX*dpitch + curY - 1];
							if (_lableImg.at<int>(curX, curY - 1) == 0 && (value >= 200 && value <= 700))
								//if (_lableImg.at<int>(curX, curY - 1) == 1)
							{// left pixel  
								neighborPixels.push(std::pair<int, int>(curX, curY - 1));
							}
							value = dpixels[curX*dpitch + curY + 1];
							if (_lableImg.at<int>(curX, curY + 1) == 0 && (value >= 200 && value <= 700))
								//if (_lableImg.at<int>(curX, curY + 1) == 1)
							{// right pixel  
								neighborPixels.push(std::pair<int, int>(curX, curY + 1));
							}
							value = dpixels[(curX - 1)*dpitch + curY];
							if (_lableImg.at<int>(curX - 1, curY) == 0 && (value >= 200 && value <= 700))
								//if (_lableImg.at<int>(curX - 1, curY) == 1)
							{// up pixel  
								neighborPixels.push(std::pair<int, int>(curX - 1, curY));
							}
							value = dpixels[(curX + 1)*dpitch + curY];
							if (_lableImg.at<int>(curX + 1, curY) == 0 && (value >= 200 && value <= 700))
								//if (_lableImg.at<int>(curX + 1, curY) == 1)
							{// down pixel  
								neighborPixels.push(std::pair<int, int>(curX + 1, curY));
							}
						}
						else
						{
							neighborPixels.pop();
						}
					}//end while

					if (cca.cca_cnt > 3000)
					{
						cca.avg_d /= cca.cca_cnt;
						cca.label = label;
						if (cca.avg_d < NearestCCA.avg_d)
						{
							NearestCCA.avg_d = cca.avg_d;
							NearestCCA.x = cca.x;
							NearestCCA.y = cca.y;
							NearestCCA.label = cca.label;
						}
					}
				}
			}
		}
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				if (_lableImg.at<int>(i, j) != NearestCCA.label)
				{
					dpixels[i * dpitch + j] = 0;
					img.at<ushort>(i,j) = 0;
				}
				else
				{
					img.at<ushort>(i,j) = dpixels[i * dpitch + j];
				}
			}
		}
		depthIm->ReleaseAccess(&data);
	}
}