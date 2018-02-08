#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/aruco.hpp"
#include <vector>

using namespace cv;
using namespace std;

int main (int argc, char** argv)
{
        VideoCapture cap;

        if(!cap.open(0)){
                return 0;
        }
        for(;;){
                Mat inputImage;
                cap >> inputImage;
                vector< int > markerIds;
                vector< vector<Point2f> > markerCorners, rejectedCandidates;
                aruco::DetectorParameters parameters;
                Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::DICT_6X6_250);
                aruco::detectMarkers(inputImage, dictionary, markerCorners, markerIds, parameters, rejectedCandidates);
                Mat outputImage;
                aruco::drawDetectedMarkers(outputImage, markerCorners, markerIds);
                if(inputImage.empty()) break;
                imshow("Webcam", outputImage);
                if(waitKey(1) >= 0) break;
        }

        return 0;
}
