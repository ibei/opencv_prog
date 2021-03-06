
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;
int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened()){  // check if we succeeded
		cout<<"haha"<<endl; 
        return -1;
	    }
        
    Mat edges;
    namedWindow("edges",1);
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        cvtColor(frame, edges, COLOR_BGR2GRAY);
        GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        Canny(edges, edges, 0, 30, 3);
        imshow("edges", edges);
        if(waitKey(30) >= 0) break;
    }
    //cout<<"haha12121212"<<endl;    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
