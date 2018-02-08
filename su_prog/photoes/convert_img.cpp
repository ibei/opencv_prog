#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <stdio.h>

using namespace std;
using namespace cv;

Mat MoveDetect(Mat background, Mat frame);
Mat dif_thre;
double area=0;
Mat final_frame;



int main()
{
	while (1)
	{
	
	Mat h,s,v,img,img_hsv;
	img=imread("red.jpg");
	//int frameCount= video.get(CV_CAP_PROP_FRAME_COUNT);
	//float FPS=video.get(CV_CAP_PROP_FPS);
	//Mat frame;
	//cout<<img.size()<<endl;
	vector<cv::Mat> hsv_vec;
	cvtColor(img,img_hsv,CV_BGR2HSV);
	split(img_hsv,hsv_vec);
	h=hsv_vec[0];
	s=hsv_vec[1];
	v=hsv_vec[2];
	imshow("img-h",h);
	imshow("img-s",s);
	imshow("img-v",v);
	cout<<"h:  "<<(int)h.at<uchar>(230,375)<<"      s:  "<<(int)s.at<uchar>(230,375)<<"       v:   "<<(int)v.at<uchar>(230,375)<<endl;
	
	if (waitKey(1000.0)==27)
		{
			cout<<"ESC!"<<endl;
			break;
		}
	
	}
	
	return 0;
}
	
	

	
