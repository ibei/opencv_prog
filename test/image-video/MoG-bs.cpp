#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<iostream>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/video/background_segm.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/videoio.hpp>
#include<opencv2/imgproc.hpp>


using namespace cv;
using namespace std;
//using namespace BackgroundSubtractorMOG;


int main()
{
	VideoCapture capture("VTest_tapping.avi");
	if( !capture.isOpened() )
	{
		cout <<"failed!" << endl;
		return -1;
	}
	//获取整个帧数
	long totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);
	cout<<"total:"<<totalFrameNumber<<"帧"<<endl;

	//设置开始帧()
	long frameToStart = 200;
	capture.set( CV_CAP_PROP_POS_FRAMES,frameToStart);
	cout<<"从第"<<frameToStart<<"帧开始读"<<endl;

	//设置结束帧
	int frameToStop = 650;

	if(frameToStop < frameToStart)
	{
		cout<<"结束帧小于开始帧，程序错误，即将退出！"<<endl;
		return -1;
	}
	else
	{
		cout<<"结束帧为：第"<<frameToStop<<"帧"<<endl;
	}

	double rate = capture.get(CV_CAP_PROP_FPS);
	int delay = 1000/rate;

	Mat frame;
	//前景图片
	Mat foreground;
	BackgroundSubtractorMOG mog;
	bool stop(false);
	//currentFrame是在循环体中控制读取到指定的帧后循环结束的变量
	long currentFrame = frameToStart;
	while( !stop )
	{
		if( !capture.read(frame) )
		{
			cout<<"从视频中读取图像失败或者读完整个视频"<<endl;
			return -2;
		}
		cvtColor(frame,frame,CV_RGB2GRAY);
		imshow("输入视频",frame);
		//参数为：输入图像、输出图像、学习速率
		mog(frame,foreground,0.01);


		imshow("前景",foreground);

		//按ESC键退出，按其他键会停止在当前帧

		int c = waitKey(delay);

		if ( (char)c == 27 || currentFrame >= frameToStop)
		{
			stop = true;
		}
		if ( c >= 0)
		{
			waitKey(0);
		}
		currentFrame++;

	}

	waitKey(0);
}


