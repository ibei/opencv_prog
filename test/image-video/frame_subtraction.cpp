#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

Mat MoveDetect(Mat background, Mat frame);
Mat dif_thre;



int main()
{
	//VideoWriter writer1("VTest_tipping_contour_result.avi",CV_FOURCC('F','L','V','1'), 25.0, Size(640,480));
	//VideoWriter writer2("VTest_tipping_dilate_result.avi",CV_FOURCC('F','L','V','1'), 25.0, Size(640,480));
	VideoCapture video("VTest_tipping.avi");
	//VideoCapture video("VTest1.avi");
	//VideoCapture video("hahah");
	if (!video.isOpened())
	{
		cout<<"wrong!"<<endl;
	}
	int frameCount= video.get(CV_CAP_PROP_FRAME_COUNT);
	cout<<"帧数:"<<frameCount<<endl;
	float FPS=video.get(CV_CAP_PROP_FPS);
	Mat temp;//for last frame
	Mat frame;//for frame
	Mat result;
	for(int i= 0; i<frameCount;i++)
	{
		video>>frame;
		imshow("frame",frame);
		moveWindow("frame",0,0);
		if(i==0)
		{
			result=MoveDetect(frame,frame);
		
		}
		else //got a value for temp
		{
			result=MoveDetect(temp,frame);
			
		}
		imshow("result",result);
		
		imshow("dilate",dif_thre);
		//writer2<<dif_thre;
		//writer1<<result;

		moveWindow("result",0,500);
		
		if (waitKey(1000.0/FPS)==27)
		{
			cout<<"ESC!"<<endl;
			break;
		}
		temp=frame.clone();
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
	
	vector<Rect> boundRect(contours.size());
	for (int i=0; i<contours.size();i++)
	{
		
		boundRect[i]=boundingRect(contours[i]);
		//if (boundRect[i].height>boundRect[i].width)
		//{
		rectangle(result,boundRect[i],Scalar(0,0,255),2);
			//cout<<"object number: "<<(++i)<<endl;
			
			//}
		
		
		}
	
	return result;

}














