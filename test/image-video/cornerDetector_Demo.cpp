#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

Mat MoveDetect(Mat background, Mat frame);


int main()
{
	VideoCapture video("VTest3.avi");
	VideoWriter writer1("VTest_tipping_contour_result.avi",CV_FOURCC('F','L','V','1'), 25.0, Size(640,480));
	//VideoCapture video("hahah");
	if (!video.isOpened())
	{
		cout<<"wrong!"<<endl;
	}
	int frameCount= video.get(CV_CAP_PROP_FRAME_COUNT);
	cout<<"帧数:"<<frameCount<<endl;
	float FPS=video.get(CV_CAP_PROP_FPS);
	Mat background;
	Mat frame;
	Mat result;
	for(int i= 0; i<frameCount;i++)
	{
		video>>frame;
		imshow("frame",frame);
		moveWindow("frame",0,0);
		int framePosition=video.get(CV_CAP_PROP_POS_FRAMES);
		cout<<"framePosition:"<<framePosition<<endl;
		if (framePosition==1){
			background=frame.clone();}
		result = MoveDetect(background,frame);
		imshow("result",result);
		moveWindow("result",0,500);
		if (waitKey(1000.0/FPS)==27)
		{
			cout<<"ESC!"<<endl;
			break;}
		
	}
	//system('pause');
	return 0;
	
}
Mat MoveDetect(Mat background, Mat frame)
{
	Mat result=frame.clone();
	Mat Gray1,Gray2;
	cvtColor(background,Gray1,CV_BGR2GRAY);
	cvtColor(frame,Gray2,CV_BGR2GRAY);
	
	Mat dif;
	absdiff(Gray1,Gray2,dif);
	imshow("diff",dif);
	moveWindow("diff",700,0);
	
	//Threshold
	Mat dif_thre;
	threshold(dif,dif_thre,60,255,CV_THRESH_BINARY);
	imshow("dif_thre",dif_thre);
	moveWindow("dif_thre",700,500);
	Mat kernel_erode=getStructuringElement(MORPH_RECT,Size(3,3));
	
	Mat kernel_dilate=getStructuringElement(MORPH_RECT,Size(15,15));
	erode(dif_thre,dif_thre,kernel_erode);
	imshow("erode",dif_thre);
	moveWindow("erode",1300,0);
	dilate(dif_thre,dif_thre,kernel_dilate);
	imshow("dilate",dif_thre);
	moveWindow("dilate",1300,500);
	
	vector<vector<Point> > contours;
	findContours(dif_thre,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
	drawContours(result,contours,-1,Scalar(0,255,0),2);
	
	vector<Rect> boundRect(contours.size());
	for (int i=0; i<contours.size();i++)
	{
		boundRect[i]=boundingRect(contours[i]);
		rectangle(result,boundRect[i],Scalar(0,0,255),2);
		}
		
	
	
	
	
	
	
	return result;

}














