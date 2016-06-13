#include <opencv2\opencv.hpp> // opencv general include file
#include <opencv2\ml\ml.hpp>// opencv machine learning include file
#include <stdio.h>
#include <iostream>
#include <time.h>
#include "CCA.h"
#include <pxcsensemanager.h>
#include <pxcsession.h>
#include "util_render.h"

#include "gl_define.h"
#include "slic.h"
#include "predict.h"
#include <pxchandconfiguration.h>
#include <pxchanddata.h>
#include <pxchandmodule.h>

#define REALSENSE 

using namespace cv; // OpenCV API is in the C++ "cv" namespace
using namespace std;

/******************************************************************************/

int main( int argc, char** argv )
{
#ifdef REALSENSE
	UtilRender *renderColor = new UtilRender(L"COLOR_STREAM");
	UtilRender *renderDepth = new UtilRender(L"DEPTH_STREAM");

	PXCSenseManager *psm = 0;
	psm = PXCSenseManager::CreateInstance();
	if (!psm)
	{
		wprintf_s(L"Unabel to create the PXCSenseManager\n");
		return 1;
	}
   
	psm->EnableStream(PXCCapture::STREAM_TYPE_COLOR, WIDTH*2, HEIGHT*2);
	psm->EnableStream(PXCCapture::STREAM_TYPE_DEPTH, WIDTH*2, HEIGHT*2);

	//Hand Tracking 
	// Enable hand tracking
	psm->EnableHand();
	// Get an instance of PXCHandModule 

	PXCHandModule *handModule = psm->QueryHand();
	PXCHandConfiguration* handConfig = handModule->CreateActiveConfiguration();
	handConfig->EnableSegmentationImage(true);
	handConfig->SetTrackingMode(PXCHandData::TrackingModeType::TRACKING_MODE_FULL_HAND);
	handConfig->SetTrackingBounds(20.2, 120, 29.9, 22.4);
	handConfig->ApplyChanges();
	PXCHandData *handData = handModule->CreateOutput();

	if (psm->Init() != PXC_STATUS_NO_ERROR)
	{
		wprintf_s(L"Unable to Init the PXCSenseManager\n");
		return 2;
	}
	PXCImage  *depthIm;
	PXCImage::ImageData data;
	PXCImage::ImageInfo dinfo;

	//Init hand
	Parsing *hand = new Parsing;

	namedWindow("Predict");
	//namedWindow("Joint");
	Mat dst = Mat(480, 640, CV_8UC3);

	Mat img_result = Mat::zeros(HEIGHT,WIDTH, CV_8UC3);

	Mat img_raw = Mat(HEIGHT, WIDTH, CV_16UC1);
	Mat img = Mat(HEIGHT, WIDTH, CV_16UC1);
	//Mat img_joint = Mat(HEIGHT, WIDTH, CV_16UC1);

	while (1)
	{
		if (psm->AcquireFrame(true) < PXC_STATUS_NO_ERROR) break;

		PXCCapture::Sample *sample = psm->QuerySample(); 

		depthIm = sample->depth;

		depthIm->AcquireAccess(PXCImage::ACCESS_READ, &data);
		dinfo = sample->depth->QueryInfo();
		ushort *dpixels = (ushort*)data.planes[0];
		int dpitch = data.pitches[0] / sizeof(ushort);

		int irow = 0;
		int jcol = 0;

		for (int y = 0; y < (int)dinfo.height; y+=2)
		{
			if(y==0)
				irow = 0;

			for (int x = 0; x < (int)dinfo.width; x+=2)
			{
				if(x == 0)
					jcol = 0;

				ushort d = dpixels[y*dpitch + x];
				if (d < 100 || d > 400)
				{
					d = 0;
				}
				img_raw.at<ushort>(irow, jcol) = d;
				//img_joint.at<ushort>(irow, jcol) = 65535 - d;
				
				jcol++;
			}
			irow++;
		}
		depthIm->ReleaseAccess(&data);
		//Mat img2 = Mat(HEIGHT*2, WIDTH*2, CV_16UC1);
		//icvprCcaBySeedFill(depthIm, sample, img2);
		/*stringstream ss;
		string str;
		vector<int> c;
		c.push_back(CV_IMWRITE_PNG_COMPRESSION);
		c.push_back(0);
		ss << (i);
		str = ss.str();
		imwrite(str+"_d.png",img,c);*/

		//cv::resize(img2, img_raw, Size(), 0.5, 0.5, cv::INTER_LINEAR);

		medianBlur(img_raw, img, 3);
		hand->getImg(img_raw);
		hand->predic();


		//Mat joint= Mat::zeros(HEIGHT,WIDTH, CV_8UC1);
		hand->show_img(img_result);
		
		//hand->extract_joint(img_result);

		resize(img_result, dst, dst.size(), 0, 0, INTER_NEAREST);
		//cv::imshow("Predict", img_result);
		cv::imshow("Predict", dst);

		
		//imwrite(str+"_p.jpg",img_result);


		if (!renderDepth->RenderFrame(depthIm)) break;

		psm->ReleaseFrame();
		if (waitKey(1) > 0)
			break;
	}
	psm->Release();

#else

	cout << "preparing..." << endl;

	Parsing *hand = new Parsing;

	Mat cur_frame = imread("10000011_d.png", CV_LOAD_IMAGE_UNCHANGED);

	hand->getImg(cur_frame);

	clock_t start, end;
	double time = 0;
	start = clock();

	cout << "predicting..." << endl;
	hand->predic();

	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	cout << "time: " << time << endl;

	Mat img_result = Mat::zeros(HEIGHT,WIDTH, CV_8UC3);

	start = clock();
	hand->show_img(img_result);
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	
	cout << "showtime: " << time << endl;

	imshow("img_result", img_result);
	Mat gray = Mat(cur_frame.rows, cur_frame.cols, CV_16UC1);
		for (int i = 0; i < cur_frame.rows; i++)
		{
			for (int j = 0; j < cur_frame.cols; j++)
			{
				if (cur_frame.at<ushort>(i, j) < zFar - 500)
				{
					gray.at<ushort>(i, j) = 65535 - cur_frame.at<ushort>(i, j);
				}
				else
				{
					gray.at<ushort>(i, j) = 0;
				}
			}
		}
	imshow("depth", gray);
	imwrite("depth1.png", img_result);
	cvWaitKey(0);
#endif
		/*cout << "begin slic\n" << endl;
		SLIC slic;
		
		Mat gray = Mat(cur_frame.rows, cur_frame.cols, CV_16UC1);
		for (int i = 0; i < cur_frame.rows; i++)
		{
			for (int j = 0; j < cur_frame.cols; j++)
			{
				if (cur_frame.at<ushort>(i, j) < zFar - 500)
				{
					gray.at<ushort>(i, j) = 65535 - cur_frame.at<ushort>(i, j);
				}
				else
				{
					gray.at<ushort>(i, j) = 0;
				}
			}
		}
		
		slic.GenerateSuperpixels(gray, 1000, &pixel_prob,10);
		//slic.savelabels();
		Mat slic_result;
		slic_result = slic.GetImgWithContours(img_result,Scalar(30,30,30));
		imwrite("slic_result.png", slic_result);
		imshow("slic_result", slic_result);
		waitKey(0);*/
		
	return 0;
}
