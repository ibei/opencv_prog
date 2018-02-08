
#include<opencv2/opencv.hpp>

using namespace cv;

int main(){
	
	
	Mat img=imread("/home/su/Desktop/opencv/test/fig/huluwa.jpg");
	imshow("image",img);
	waitKey(0);
	return 0;
	
}


