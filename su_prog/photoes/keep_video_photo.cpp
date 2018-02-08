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
	
	VideoCapture video("1122.avi");
	int frameCount= video.get(CV_CAP_PROP_FRAME_COUNT);
	float FPS=video.get(CV_CAP_PROP_FPS);
	Mat frame;
	
	for (int i=0;i<frameCount;i++)
	{
		video>>frame;
		//int j=0;
		imshow("frame",frame);
		cout<<i<<endl;
		if (i==100)
		{
			imwrite("Tomato.jpg",frame);
		}
		//imwrite(j+".bmp",frame);
		//j++;
		if (waitKey(1000.0/FPS)==27)
		{
			cout<<"ESC!"<<endl;
			break;
		}
		
	}
	return 0;
}
	
	

	
