//-------------------------------------------------------------------------------------------
/*! \file    color_detector_node.cpp
    \brief   Color detector node.
    \author  Akihiko Yamaguchi, xakiyam@gmail.com
    \version 0.1
    \date    Jun.05, 2014
*/
//-------------------------------------------------------------------------------------------
#include "ay_tools/ay_vision/include/ay_vision/color_detector.h"
#include "ay_tools/ay_vision/include/ay_vision/flow_finder.h"

#include "ay_tools/ay_vision/include/ay_vision/vision_util.h"

#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
//-------------------------------------------------------------------------------------------


namespace trick
{
const char *DefaultFileNames[]={
    "default_colors1.dat",
    "default_colors2.dat",
    "default_colors3.dat",
    "default_colors4.dat",
    "default_colors5.dat",
    "default_colors6.dat",
    "default_colors7.dat"};
const char *DefaultFlowFileName= "default_colors0.dat";
std::string ColorFilesBase= "";

int  CDIdx(0);
bool Running(true);
TMultipleColorDetector ColDetector;
TMultipleColorDetector FlowColDetector;  // Color detector for flow
TFlowFinder FlowFinder;
// TMovingObjectDetector MovDetector;

int NumDetectors(1);
int NRotate90(0);
TEasyVideoOut VideoOut;
//int VizMode(2);  // 0: camera only, 1: camera + detected, 2: 0.5*camera + detected, 3: 0.25*camera + detected, 4: detected only, 5: 0.5*camera + flow + flow-mask
//int FlowMaskMode(2);  // 0: none, 1: removing moving objects, 2: mask with colors
                      // DEPRECATED: FlowMaskMode==1 (so MovDetector is removed).

//std::vector<ay_vision_msgs::ColDetVizPrimitive> VizObjs;  // External visualization requests.
}
//-------------------------------------------------------------------------------------------
using namespace std;
// using namespace boost;
using namespace trick;
//-------------------------------------------------------------------------------------------
// #define print(var) PrintContainer((var), #var"= ")
// #define print(var) std::cout<<#var"= "<<(var)<<std::endl
//-------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------

// return if continue


// //-------------------------------------------------------------------------------------------

void MaskFlow(TFlowFinder &flow_finder, const cv::Mat &mask_img)
{
  std::list<TFlowElement> &flow(flow_finder.RefFlowElements());
  for(std::list<TFlowElement>::iterator itr(flow.begin()),itr_end(flow.end());
      itr!=itr_end;)
  {
    if(mask_img.at<unsigned char>(itr->Y,itr->X))
      ++itr;
    else
      itr= flow.erase(itr);
  }
}
 

//-------------------------------------------------------------------------------------------

int main(int argc, char**argv)
{

  int cap_width(640), cap_height(480);
  int mode(2);  // Ratio computation mode (1 or 2, 1:deprecated).
  //int pub_mode(0);  // 0: original, 1: only ColDetSensor message, 2: both
  double block_area_min(10.0);
  std::string vout_base("/tmp/vout");
  
  cv::VideoCapture cap("detect_amout.avi"); // open the default camera  .....................  modified :  default video
  if(!cap.isOpened())  // check if we succeeded
  {
    std::cerr<<"Cannot open: "<<"camera"<<std::endl;
    return -1;
  }
  std::cerr<<"Camera opened"<<std::endl;

  cap.set(CV_CAP_PROP_FRAME_WIDTH, cap_width);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT, cap_height);

  VideoOut.SetfilePrefix(vout_base);

  ColDetector.Setup(NumDetectors);
  for(int i(0); i<NumDetectors; ++i)
    ColDetector.LoadColors(i, ColorFilesBase+DefaultFileNames[i]);
  ColDetector.SetBlockAreaMin(block_area_min);
  FlowColDetector.Setup(1);
  FlowColDetector.LoadColors(0, ColorFilesBase+DefaultFlowFileName);
  CDIdx= 0;

  FlowFinder.SetOptFlowWinSize(cv::Size(3,3));
  FlowFinder.SetOptFlowSpdThreshold(5.0);
  FlowFinder.SetErodeDilate(1);
  FlowFinder.SetAmountRange(/*min=*/3.0, /*max=*/3000.0);
  FlowFinder.SetSpeedRange(/*min=*/3.0, /*max=*/-1.0);


/*  existed originally
  std::vector<ros::Publisher> ratio_pubs(NumDetectors);
  std::vector<ros::Publisher> mxy_pubs(NumDetectors);
  ros::Publisher flow_pub;
  ros::Publisher sensor_pub;
*/
  // pub_mode: // 0: original, 1: only ColDetSensor message, 2: both
  

  cv::namedWindow("color_detector",1);
  cv::Mat frame, disp_img, flow_mask_img;
  ColDetector.SetCameraWindow(frame);
  FlowColDetector.SetCameraWindow(frame);




  int frameCount= cap.get(CV_CAP_PROP_FRAME_COUNT);

  for(int f(0);frameCount;++f)
  {

      cap >> frame; // get a new frame from camera
    
      ColDetector.Detect(frame, mode, /*verbose=*/false);
      ColDetector.Draw(disp_img);
 

      

      // Remove detected flows of moving objects (e.g. robot hand)
      // if(FlowMaskMode==1)  RemoveMovingObjectFlow(FlowFinder, MovDetector);
     
     

      // Compute average flow


    

  } 




  return 0;

}
//-------------------------------------------------------------------------------------------

