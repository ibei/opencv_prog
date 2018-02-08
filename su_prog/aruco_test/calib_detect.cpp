#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/calib3d.hpp>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;
const float calibrationSquareDimension=0.0225f;
const float arucoSquareDimension=0.0064f;
const Size chessboardDimensions= Size(6,8); 

void createArucoMarkers()
{
	Mat marker;
	Ptr<aruco::Dictionary> dic = aruco::getPredefinedDictionary(aruco::DICT_4X4_50);
	//aruco::Dictionary dic=aruco::getPredefinedDictionary(aruco::DICT_6X6_250);
	//aruco::drawMarker(dic,23,200,marker,1);
	
	for(int i=0;i<50;i++)
	{
		aruco::drawMarker(dic, i, 500, marker,1);
		ostringstream convert;
		string imageName= "4X4Marker_";
		convert<<imageName<<i<<".jpg";
		imwrite(convert.str(),marker);
		}
	
	 
	}
	
	
void creatKnownBoardPosition(Size boardSize,float squareEdgeLength,vector<Point3f>& corners)
{
	for(int i=0; i<boardSize.height;i++)
	{
		for(int j=0;j<boardSize.width;j++)
		{
			corners.push_back(Point3f(j * squareEdgeLength, i*squareEdgeLength,0.0f));
			
			}
		}
	}
	
void getChessboardCorners(vector<Mat> images,vector<vector<Point2f> >& allFoundCorners, bool showResults=false)
{
	for(vector<Mat>::iterator iter=images.begin(); iter !=images.end();iter++)
	{
		vector<Point2f> pointBuf;
		bool found=findChessboardCorners(*iter,Size(6,8),pointBuf,CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE);
		if(found)
		{
			allFoundCorners.push_back(pointBuf);
			}
		if(showResults)
		{
			drawChessboardCorners(*iter,Size(6,8),pointBuf,found);
			imshow("Looking for Corners", *iter);
			waitKey(0);
			}	
		}
	}
	
void cameraCalibration(vector<Mat> calibrationImages, Size boardSize, float squareEdgeLength, Mat& cameraMatrix, Mat&  distanceCoe)
{
	vector<vector<Point2f> > checkboardImageSpacePoints;
	getChessboardCorners(calibrationImages, checkboardImageSpacePoints,false);
	vector<vector<Point3f> > worldSpaceCornerPoints(1);
	creatKnownBoardPosition(boardSize,squareEdgeLength,worldSpaceCornerPoints[0]);
	worldSpaceCornerPoints.resize(checkboardImageSpacePoints.size(), worldSpaceCornerPoints[0]);
	vector<Mat> rVectors,tVectors;
	distanceCoe=Mat::zeros(8,1,CV_64F);
	calibrateCamera(worldSpaceCornerPoints,checkboardImageSpacePoints,boardSize,cameraMatrix,distanceCoe,rVectors,tVectors);
	
	
	}

bool saveCameraCalibration(string name, Mat cameraMatrix, Mat distanceCoe)
{
	ofstream outStream(name.c_str());
	if(outStream)
	{
		uint16_t rows=cameraMatrix.rows;
		uint16_t columns=cameraMatrix.cols;
		outStream<< rows<<endl;
		outStream << columns<<endl;
		for (int r=0; r<rows; r++)
		{
			for(int c=0;c<columns;c++)
			{
				
				double value=cameraMatrix.at<double>(r,c);
				outStream<<value<<endl;
				}
			
			}
			rows=distanceCoe.rows;
			columns=distanceCoe.cols;
			
			outStream<< rows<<endl;
			outStream << columns<<endl;
			for (int r=0; r<rows; r++)
			{
				for(int c=0;c<columns;c++)
				{
				
					double value=cameraMatrix.at<double>(r,c);
					outStream<<value<<endl;
					}
			
				}
				outStream.close();
				return true;
		}
		return false;
	}
int startWebcamMonitoring(const Mat& cameraMatrix, const Mat& distanceCoe, float arucoSquareDimensions)
{
	Mat frame;
	vector<int> markerIds;
	vector<vector<Point2f> > markerCorners,rejectedCandidates;
	aruco::DetectorParameters parameters;
	Ptr<aruco::Dictionary> markerDic=aruco::getPredefinedDictionary(aruco::DICT_4X4_50);
	VideoCapture vid(0);
	if(!vid.isOpened())
	{
		return -1;
		}
	namedWindow("Webcam",CV_WINDOW_AUTOSIZE);
	vector<Vec3d> rotationVectors,transVectors;
	//vector<Mat> rotationVectors,transVectors;
	while(true)
	{
		if(!vid.read(frame))
			break;
		aruco::detectMarkers(frame,markerDic,markerCorners,markerIds);
		aruco::estimatePoseSingleMarkers(markerCorners,arucoSquareDimension,cameraMatrix,distanceCoe, rotationVectors,transVectors);
		//aruco::drawAxis(frame,cameraMatrix,distanceCoe,)
		for (int i=0;i< markerIds.size();i++)
		{
			aruco::drawAxis(frame,cameraMatrix,distanceCoe,rotationVectors[i],transVectors[i],0.1f);
			cout<<"RotationVec: "<< rotationVectors[i]<<"    TransVec:  "<<transVectors[i]<<endl;
			
			}
		imshow("Webcam",frame);
		if(waitKey(30)>=0) break;
		}
	return 1;
	}

bool loadCameraCalibration(string name, Mat& cameraMatrix, Mat& distanceCoe)
{
	ifstream inStream(name.c_str());
	if (inStream)
	{
		uint16_t rows;
		uint16_t columns;
		inStream>>rows;
		inStream>>columns;
		cameraMatrix=Mat(Size(columns,rows),CV_64F);
		for(int r=0;r<rows;r++)
		{
			for(int c=0; c<columns; c++)
			{
				double read=0.0f;
				inStream>>read;
				cameraMatrix.at<double>(r,c)=read;
				cout<< cameraMatrix.at<double>(r,c) << "\n";
				}
			}
			//distance coe
		inStream >> rows;
		inStream >> columns;
		distanceCoe= Mat::zeros(rows,columns,CV_64F);
		for(int r=0;r<rows;r++)
		{
			for(int c=0; c<columns; c++)
			{
				double read=0.0f;
				inStream>>read;
				distanceCoe.at<double>(r,c)=read;
				cout<< distanceCoe.at<double>(r,c) << "\n";
				}
			}
		inStream.close();
		return true;
		
		}
	return false;
	}

void cameraCalibrationProcess(Mat& cameraMatrix, Mat& distanceCoe)
{
	Mat frame;
	Mat drawToFrame;
	
	vector<Mat> saveImages;
	vector<vector<Point2f> > markerCorners,rejectedCandidates;
	VideoCapture video(0);
	if(!video.isOpened())
	{
		return ;
		}
	int framesPerSecond=20;
	namedWindow("Webcam",CV_WINDOW_AUTOSIZE);
	while(true)
	{
		if(!video.read(frame))
			break;
		vector<Vec2f> foundPoints;
		bool found=false;
		found=findChessboardCorners(frame,chessboardDimensions,foundPoints,CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE);
		frame.copyTo(drawToFrame);
		drawChessboardCorners(drawToFrame,chessboardDimensions,foundPoints,found);
		if(found)
			imshow("Webcam",drawToFrame);
		else
			imshow("Webcam",frame);
		char character=waitKey(1000/framesPerSecond);
		switch(character)
		{
			case ' ':
				//save image
				if (found)
				{
					Mat temp;
					frame.copyTo(temp);
					saveImages.push_back(temp);
					}
				break;
			case 13:
				//start calib
				if(saveImages.size()>15)
				{
					cameraCalibration(saveImages,chessboardDimensions,calibrationSquareDimension,cameraMatrix,distanceCoe);
					saveCameraCalibration("cameraCalibration",cameraMatrix,distanceCoe);
				}
				break;
			case 27:
				//exit
				return ;
				break;
				
			}
		
		}
	
	}
	


int main(int argv, char** argc)
{
	//Mat cameraMatrix=Mat::eye(3,3,CV_64F);
	//Mat distanceCoe;
	
	Mat cameraMatrix,distanceCoe;
	//cameraCalibrationProcess(cameraMatrix,distanceCoe);
	loadCameraCalibration("cameraCalibration",cameraMatrix,distanceCoe);
	startWebcamMonitoring(cameraMatrix,distanceCoe,arucoSquareDimension);
	
	/*
	VideoCapture video;
	video.open(0);
	//Mat cameraMatrix,Coe;
	aruco::Dictionary dic=aruco::getPredefinedDictionary(aruco::DICT_4X4_50);
	while(video.grab())
	{
		Mat image,imageCopy;
		video.retrieve(image);
		image.copyTo(imageCopy);
		vector<int> ids;
		vector<vector<Point2f> > corners;
		aruco::detectMarkers(image,dic,corners,ids);
		if(ids.size()>0)
		{
			aruco::drawDetectedMarkers(imageCopy,corners,ids);
			vector<Mat> rvecs,tvecs;
			aruco::estimatePoseSingleMarkers(corners,0.016,cameraMatrix,distanceCoe,rvecs,tvecs);
			for (int i=0;i< ids.size();i++)
			{
				aruco::drawAxis(imageCopy,cameraMatrix,distanceCoe,rvecs[i],tvecs[i],0.1f);
			}
			imshow("Webcam",imageCopy);
			if(waitKey(30)>=0) break;
			
			}
		}
		*/
	
	return 0;
	
	}
