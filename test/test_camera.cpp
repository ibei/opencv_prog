#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv)

{
	VideoCapture cap(0);
	if(!cap.isOpened())
		return false;
	
	cout<<"original size"<<cap.get(CV_CAP_PROP_FRAME_WIDTH)<<" "<<cap.get(CV_CAP_PROP_FRAME_HEIGHT)<<endl;
	cap.set(CV_CAP_PROP_FRAME_WIDTH,160);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT,120);
	cap.set(CV_CAP_PROP_FPS,10);
	cout<<"Modified size"<<cap.get(CV_CAP_PROP_FRAME_WIDTH)<<" "<<cap.get(CV_CAP_PROP_FRAME_HEIGHT)<<endl;
	
		cvNamedWindow("camera show", CV_WINDOW_AUTOSIZE);
	Mat frame;
	while(true)
	{
		cap>>frame;
		imshow("camera show",frame);
		if(waitKey(10)==27)
			break;
		
			
	}
	return 0;
}
