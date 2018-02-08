#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace cv;

int main()
{
	//VideoCapture capture(0);
	VideoCapture capture(0);
	
	VideoWriter writer("marker.avi",CV_FOURCC('F','L','V','1'), 25.0, Size(640,480));
	Mat frame;
	
	while(capture.isOpened())
	{
		capture >> frame;
		//frame>>writer;
		writer<<frame;
		//namedWindow("test",left);
		imshow("test",frame);
		moveWindow("test",0,0);
		if(waitKey(20)==27)
		{
			break;}
	}
	
}
