#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/calib3d.hpp>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
using namespace cv;

int main(int argv, char** argc)
{
	VideoCapture video(0);

	if (!video.isOpened())
	{
		cout<<"wrong!"<<endl;
	}
	Mat img,img_copy;
	while(1)
	{
		
		video>>img;
		img.copyTo(img_copy);
		vector<int> marker_id;
		vector<vector<Point2f> > marker_corner, reject_condi;
		aruco::DetectorParameters para;
	
		Ptr<aruco::Dictionary> dic = aruco::getPredefinedDictionary(aruco::DICT_4X4_50);
	
	
		//aruco::detectMarkers(img,dic,marker_corner,marker_id,para,reject_condi);
		aruco::detectMarkers(img,dic,marker_corner,marker_id);
	
		aruco::drawDetectedMarkers(img_copy,marker_corner,marker_id);
	
		imshow("out",img_copy);
		
		if(waitKey(1000/28)==27)
		{
			cout<<"Exit"<<endl;
			break;
			}
		}
	//Mat input_img,output_img;
	
	
	
	
	return 0;
	 
	}
	



