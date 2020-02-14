#include "stdafx.h"
#include"test.h"
using namespace std;
using namespace cv;

int getvideo()
{
	VideoCapture capture("./images/1.avi");
	if (!capture.isOpened())
		cout << "fail to open" << endl;
	long totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);
	cout << "整个视频共" << totalFrameNumber << endl;
	long frameToStart = 300;
	capture.set(CV_CAP_PROP_POS_FRAMES, frameToStart);
	cout << "从第" << frameToStart << "帧开始读" << endl;
	int frameToStop = 400;
	if (frameToStart > frameToStop)
	{
		cout << "结束帧小于开始帧" << endl;
		return -1;
	}
	else
	{
		cout << "结束帧为：第" << frameToStop << "帧"<<endl;
	}
	double rate = capture.get(CV_CAP_PROP_FPS);
	cout << "帧率为：" << rate << endl;
	bool stop = false;
	Mat frame;
	
	int delay = 1000 / rate;
	long currentFrame = frameToStart;
	int kernelsize = 3;
	Mat kernel = Mat::ones(kernelsize, kernelsize, CV_32F) / (float)(kernelsize);
	while (!stop)
	{
		if (!capture.read(frame))
		{
			cout << "读取视频失败" << endl;
			return -1;
		}
		namedWindow("Extracted frame");
		imshow("Extracted frame", frame);

		filter2D(frame, frame, -1, kernel);

		namedWindow("afterfilter");
		imshow("afterfilter", frame);

		namedWindow("dst");
		Mat src1 = edgetest(frame);
		imshow("dst", src1);

		cout << "正在读取第：" << currentFrame << "帧" << endl;
		int c = waitKey(delay);
		currentFrame++;
		if ((char)c == 27 || currentFrame > frameToStop)
		{
			stop = true;
		}
		if (c >= 0)
		{
			waitKey(0);
			
		}
		
	}
	capture.release();
	waitKey(0);
	
	return 0;


}



int opencamera()
{
	VideoCapture capture(0);
	if (!capture.isOpened())
	{
		cout << "open camera faild. " << endl;
		return -1;
	}
	while (true)
	{
		Mat frame;
		capture >> frame;
		if (!frame.empty())
		{
			imshow("camera", frame);
			Mat src1 = edgetest(frame);
			imshow("dst", src1);
		}
		if (waitKey(30) > 0)
		{
			break;
		}
	}
	return 0;
}

Mat edgetest(Mat src)
{
	//Mat src = imread("./images/1.png");
	//Mat src = imread(imagespath);
	Mat src1 = src.clone();
	//imshow("src",src);
	Mat dst, edge, gray;
	dst.create(src1.size(), src1.type());
	cvtColor(src1,gray,COLOR_BGR2GRAY);
	blur(gray, edge, Size(3, 3));
	Canny(edge, edge, 3, 9, 3);
	//imshow("edgetest", edge);
	dst = Scalar::all(0);
	src1.copyTo(dst, edge);
	//imshow("效果图：", dst);
	//waitKey(0);
	return dst;
}