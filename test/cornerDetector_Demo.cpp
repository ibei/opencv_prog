#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

Mat MoveDetect(Mat background, Mat frame);


int main()
{
	VideoCapture video("/home/su/Desktop/opencv/test/image-video/VTest.avi");
	//VideoCapture video("hahah");
	if (!video.isOpened())
	{
		cout<<"wrong!"<<endl;
	}
	int frameCount= video.get(CV_CAP_PROP_FRAME_COUNT);
	//float FPS=video.get(CV_CAP_PROP_FPS);
	Mat background;
	Mat frame;
	for(int i= 0; i<frameCount;i++)
	{
		video>>frame;
		imshow("frame",frame);
	}
	
	
	
	
	
}
