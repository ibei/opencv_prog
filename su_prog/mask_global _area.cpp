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
	
	//VideoCapture video("detect_amout.avi");
	VideoCapture video("446.avi");
	Mat mask;
	
	//Rect r1(216,213,144,178);
	//mask=Mat::zeros(480,640,CV_8UC1);
	//mask(r1).setTo(255);
	//cout<<mask.size()<<endl;
	
	
	if (!video.isOpened())
	{
		cout<<"wrong!"<<endl;
	}
	int frameCount= video.get(CV_CAP_PROP_FRAME_COUNT);
	cout<<"帧数:"<<frameCount<<endl;
	float FPS=video.get(CV_CAP_PROP_FPS);
	Mat background;//for last frame
	Mat frame;//for frame
	Mat frame_ori;
	//Mat frame_final;
	Mat result;
	for(int i= 0; i<frameCount;i++)
	{
		
		
		video>>frame_ori;
		//cout<<frame_ori.imagedata<<endl;
		Mat frame=frame_ori(Rect(200,355,30,20));
		
		
		//frame_ori.copyTo(frame,mask);
		
		imshow("frame",frame);
		moveWindow("frame",0,0);
		if(i==0)
		{
			background=frame.clone();
			result=MoveDetect(background,frame);
			}
		
		result=MoveDetect(background,frame);
		//frame_ori.copyTo(result,frame_final);
		result.copyTo(frame,frame);
		
		imshow("result",frame_ori);
		
		imshow("dilate",dif_thre);
		//writer2<<dif_thre;
		//writer1<<result;
		cout<<i<<endl;
		

		moveWindow("result",0,500);
		
		if (waitKey(1000.0/FPS)==27)
		{
			cout<<"ESC!"<<endl;
			break;
		}
		//temp=frame.clone();
	}
	//system('pause');
	return 0;
	
}
Mat MoveDetect(Mat temp, Mat frame)
{
	//int j=0;
	Mat result=frame.clone();
	Mat Gray1,Gray2;
	cvtColor(temp,Gray1,CV_BGR2GRAY);
	cvtColor(frame,Gray2,CV_BGR2GRAY);
	
	Mat dif;
	absdiff(Gray1,Gray2,dif);
	imshow("dif",dif);
	moveWindow("dif",700,0);
	
	
	//Threshold
	//Mat dif_thre;
	threshold(dif,dif_thre,60,255,CV_THRESH_BINARY);
	imshow("dif_thre",dif_thre);
	moveWindow("dif_thre",700,500);
	Mat kernel_erode=getStructuringElement(MORPH_RECT,Size(3,3));
	
	Mat kernel_dilate=getStructuringElement(MORPH_RECT,Size(15,15));
	erode(dif_thre,dif_thre,kernel_erode);
	imshow("erode",dif_thre);
	moveWindow("erode",1300,0);
	dilate(dif_thre,dif_thre,kernel_dilate);
	//imshow("dilate",dif_thre);
	moveWindow("dilate",1300,500);
	//writer2<<dilate;
	
	
	//fixed statement
	vector<vector<Point> > contours;
	
	findContours(dif_thre,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
	//cout<<contours.size()<<endl;
	drawContours(result,contours,-1,Scalar(0,255,0),2);
	//if(contours.size()==1)
	
	//vector<Rect> boundRect(contours.size());
	for (int i=0; i<contours.size();i++)
	{
		area+=contourArea(contours[i],true);
	}
	if(area!=0)	


	cout<<"Area is :   "<<area<<endl;
	area=0;
	//{
		
		//boundRect[i]=boundingRect(contours[i]);
		//if (boundRect[i].height>boundRect[i].width)
		//{
		//rectangle(result,boundRect[i],Scalar(0,0,255),2);
		
			//cout<<"object number: "<<(++i)<<endl;
		//area+=contourArea(contours[i],true);
		//cout<<"总面积为:  "<<area<<endl;
		
		
		//}
	
	return result;

}














