#include "ay_tools/ay_vision/include/ay_vision/color_detector.h"
#include "ay_tools/ay_vision/include/ay_vision/flow_finder.h"
#include "ay_tools/ay_vision/include/ay_vision/color_detector.h"
#include "ay_tools/ay_vision/include/ay_vision/vision_util.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>  // cvtColor
#include <opencv2/highgui/highgui.hpp>
#include <cstdio>
#include <cmath>
#include <cassert>
#include <vector>
#include <iostream>
#include <fstream>

/*

void TColorDetector::SetupColors(const std::vector<cv::Vec3b> &colors, const cv::Vec3s &col_radius)
{
  // Making a lookup table of 3 channels
  lookup_table_.create(256, 1, CV_8UC3);
  lookup_table_= cv::Scalar(0,0,0);

  for(std::vector<cv::Vec3b>::const_iterator citr(colors.begin()),clast(colors.end()); citr!=clast; ++citr)
  {
    for(int k(0); k<3; ++k)
    {
      int crad(std::abs(col_radius[k]));
      for(int i(-crad); i<=+crad; ++i)
      {
        int idx((*citr)[k]+i);
        if(col_radius[k]>=0)
        {
          if(idx<0 || 255<idx)  continue;
        }
        else
        {
          while(idx<0)  idx+= 256;
          idx= (idx%256);
        }
        lookup_table_.at<cv::Vec3b>(idx,0)[k]= 255;
      }
    }
  }
}
 */


cv::Mat TColorDetector::Detect(const cv::Mat &src_img) const
{
  assert(src_img.type()==CV_8UC3);

  cv::Mat color_img;
  if(using_blur_)
  {
    GaussianBlur(src_img, color_img, gaussian_kernel_size_, gaussian_sigma_x_, gaussian_sigma_y_);
    cv::cvtColor(color_img, color_img, color_code_);  // Color conversion with code
  }
  else
    cv::cvtColor(src_img, color_img, color_code_);  // Color conversion with code

  // Apply the lookup table (for each channel, the image is binarized)
  cv::LUT(color_img, lookup_table_, color_img);

  cv::Mat ch_imgs[3];
  cv::split(color_img, ch_imgs);

  // For each pixel, take "and" operation between all channels
  cv::Mat mask_img;
  cv::bitwise_and(ch_imgs[0], ch_imgs[1], mask_img);
  cv::bitwise_and(mask_img, ch_imgs[2], mask_img);

  if(dilations_erosions_>0)
  {
    cv::dilate(mask_img,mask_img,cv::Mat(),cv::Point(-1,-1), dilations_erosions_);
    cv::erode(mask_img,mask_img,cv::Mat(),cv::Point(-1,-1), dilations_erosions_);
  }

  return mask_img;
}

void TMultipleColorDetector::Setup(int num_detectors)
{
  col_detectors_.resize(num_detectors);
  detect_colors_.resize(num_detectors);
  nonzero_base_.resize(num_detectors);
  mask_imgs_.resize(num_detectors);
}


void TMultipleColorDetector::Detect(const cv::Mat &frame, int mode, bool verbose)
{
  std::vector<std::vector<cv::Point> > contours;
  double area(0.0);
  cv::Point2d center;
  cv::Rect bound;
  cv::Mat tmp_mask;
  data_ratio_.resize(Size());
  data_area_.resize(Size());
  data_center_xy_.resize(2*Size());
  data_bound_.resize(4*Size());
  nums_blocks_.resize(Size());
  blocks_area_.clear();
  blocks_center_xy_.clear();
  for(int i(0); i<Size(); ++i)
  {
    mask_imgs_[i]= col_detectors_[i].Detect(frame);

    mask_imgs_[i].copyTo(tmp_mask);
    // FindLargestContour(tmp_mask, &area, &center, &bound);
    {
      cv::findContours(tmp_mask,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
      int n_blocks= 0;
      if(contours.size()>0)
      {
        int ic_max(0);
        double a(0.0),a_max(0.0);
        cv::Point2d c(0,0), c_max(0,0);
        for(int ic(0),ic_end(contours.size()); ic<ic_end; ++ic)
        {
          a= cv::contourArea(contours[ic],false);
          if(a<block_area_min_)  continue;
          ++n_blocks;
          blocks_area_.push_back(a);
          cv::Moments mu= cv::moments(contours[ic]);
          c= cv::Point2d(mu.m10/mu.m00, mu.m01/mu.m00);
          blocks_center_xy_.push_back(c.x);
          blocks_center_xy_.push_back(c.y);
          if(a>a_max)  {ic_max= ic; a_max= a;  c_max= c;}
        }
        std::vector<cv::Point> &cnt(contours[ic_max]);
        area= a_max;
        center= c_max;
        bound= cv::boundingRect(cnt);
      }
      nums_blocks_[i]= n_blocks;
      if(n_blocks==0)
      {
        area= 0.0;
        center= cv::Point2d(0.0,0.0);
        bound= cv::Rect(0,0,0,0);
      }
    }


    // int nonzero= cv::countNonZero(mask_imgs_[i]), diff(0);
    int nonzero(area), diff(0);
    double ratio(0.0);
    switch(mode)
    {
    case 1:
      ratio= double(nonzero)/double(mask_imgs_[i].total());
      if(verbose)  std::cout<<"nonzero ("<<i<<"): \t"<<nonzero<<" / \t"<<mask_imgs_[i].total()
                      <<"  \t"<<ratio<<std::endl;
      break;
    case 2:
      if(nonzero_base_[i]==0)
      {
        nonzero_base_[i]= (nonzero>0 ? nonzero : 1);
        std::cerr<<"###Reset nonzero_base_: "<<nonzero_base_[i]<<std::endl;
      }
      diff= nonzero_base_[i]-nonzero;
      if(diff<0)  diff= 0;
      ratio= double(diff)/double(nonzero_base_[i]);
      if(verbose)  std::cout<<"diff ("<<i<<"): \t"<<diff<<"  \t"<<ratio<<std::endl;
      break;
    default:
      std::cerr<<"Invalid mode:"<<mode<<std::endl;
    }

    data_ratio_[i]= ratio;
    data_area_[i]= area;

    // int x_med(0), y_med(0);
    // GetMedian(mask_imgs_[i],x_med,y_med);
    // data_center_xy_[2*i+0]= x_med;
    // data_center_xy_[2*i+1]= y_med;

    data_center_xy_[2*i+0]= center.x;
    data_center_xy_[2*i+1]= center.y;
    data_bound_[4*i+0]= bound.x;
    data_bound_[4*i+1]= bound.y;
    data_bound_[4*i+2]= bound.width;
    data_bound_[4*i+3]= bound.height;
  }
}


void TMultipleColorDetector::Draw(cv::Mat &img_draw) const
{
  cv::Mat disp_img, disp_imgs[3];
  if(Size()>=2)
  {
    int rows(mask_imgs_[0].rows), cols(mask_imgs_[0].cols);
    disp_imgs[0]= cv::Mat::zeros(rows, cols, CV_8U);
    disp_imgs[1]= cv::Mat::zeros(rows, cols, CV_8U);
    disp_imgs[2]= cv::Mat::zeros(rows, cols, CV_8U);
    for(int i(0); i<Size(); ++i)
    {
      disp_imgs[0]+= double(DispColors(i)[0])/255.0 * mask_imgs_[i];
      disp_imgs[1]+= double(DispColors(i)[1])/255.0 * mask_imgs_[i];
      disp_imgs[2]+= double(DispColors(i)[2])/255.0 * mask_imgs_[i];
    }
    cv::merge(disp_imgs, 3, disp_img);
  }
  else
  {
    disp_imgs[0]= mask_imgs_[0];
    disp_imgs[1]= mask_imgs_[0];
    disp_imgs[2]= mask_imgs_[0];
    // img_draw= mask_imgs_[0];
    cv::merge(disp_imgs, 3, disp_img);
  }
  img_draw+= disp_img;

  cv::Point2d c(0.0,0.0);
  for(std::list<double>::const_iterator itr(blocks_center_xy_.begin()),itr_end(blocks_center_xy_.end());
      itr!=itr_end; ++itr)
  {
    c.x= *itr;  ++itr;  c.y= *itr;
    cv::circle(img_draw,c,3,0.5*cv::Scalar(DispColors(0)));
  }

  for(int i(0); i<Size(); ++i)
  {
    cv::circle(img_draw,Center(i),3,0.7*cv::Scalar(DispColors(i)));
    cv::rectangle(img_draw, Bound(i),0.7*cv::Scalar(DispColors(i)), 1);
  }
}

int main()
{
	
	VideoCapture video("detect_amout.avi");

	if (!video.isOpened())
	{
		cout<<"wrong!"<<endl;
	}
	int frameCount= video.get(CV_CAP_PROP_FRAME_COUNT);
	cout<<"帧数:"<<frameCount<<endl;
	
	float FPS=video.get(CV_CAP_PROP_FPS);
	Mat background;//for last frame
	Mat frame;//for frame
	Mat frame_ori;
	//Mat frame_final;
	Mat result;
	for(int i= 0; i<frameCount;i++)
	{
		
		
		video>>frame_ori;
		//cout<<frame_ori.imagedata<<endl;
		
		Mat frame=frame_ori(Rect(216,213,200,178));
		
		
		//frame_ori.copyTo(frame,mask);
		
		imshow("frame",TColorDetector::Detect(frame_ori));
		moveWindow("frame",0,0);
		
		

		moveWindow("result",0,500);
		
		if (waitKey(1000.0/FPS)==27)
		{
			cout<<"ESC!"<<endl;
			break;
		}
		//temp=frame.clone();
	}
	//system('pause');
	return 0;
	
}

