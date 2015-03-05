/*
*	Copyright (C)   Lyq root#lyq.me
*	File Name     : cartoonResizing
*	Creation Time : 2015-3-5 14:47:00 UTC+8
*	Environment   : Windows8.1-64bit VS2013 OpenCV2.4.9
*	Homepage      : http://www.lyq.me
*/

#ifndef OPENCV
#define OPENCV
#include <opencv2\opencv.hpp>
#include <opencv2\video\video.hpp>
using namespace cv;
#endif // !OPENCV

#ifndef STD
#define STD
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <algorithm>
using namespace std;
#define INF 2000000000
#define sqr(_x) ( (_x) * (_x) )
#endif // !STD

void help( void ); 
int openVideo( VideoCapture&, const char* );
void seamCarving( VideoCapture& );

int main( void ) {

	help();
	
	char *videoName = "shot1_240p.avi";
	VideoCapture cap;
	int state = openVideo( cap, videoName );
	if ( !state ) return -2;

	const int frameAmount = 30;
	seamCarving( cap );

	return 0;
}

void help( void ) {

	printf("===	Copyright (C)   Lyq root#lyq.me\n"
		"===	File Name : cartoonResizing\n"
		"===	Creation Time : 2015-3-5 14:47:00 UTC+8\n"
		"===	Environment : Windows8.1-64bit VS2013 OpenCV2.4.9\n"
		"===	Homepage : http ://www.lyq.me\n"
		"===	\n"
		"===	This program demostrated a simple method of cartoon resizing.\n"
		"===	It learns the pixels' Energy and calculates the min Curve.\n"
		"===	Reference: Improved Seam Carving for Video Retargeting\n");
}

int openVideo( VideoCapture &cap, const char *videoName ) {

	if ( !cap.open( videoName ) )
	{
		cout << "The videoName error!" << endl;
		return false;
	}

	return true;
}

void seamCarving( VideoCapture &cap ) {

	int framePos = 0;
	int frameCount = (int)cap.get( CV_CAP_PROP_FRAME_COUNT );
	const int frameAmount = 150;
	Mat frames[frameAmount], grad[frameAmount], spatialEnergy;
	Size frameSize( (int)cap.get( CV_CAP_PROP_FRAME_WIDTH ), (int)cap.get( CV_CAP_PROP_FRAME_HEIGHT ) );

	cout << frameSize.height << " " << frameSize.width << endl;

	while ( framePos < frameCount ) {
		
		if ( framePos == 0 ) {
			
			Mat inputFrame;
			cap.read( inputFrame );
			cvtColor( inputFrame, frames[0], CV_RGB2GRAY );
			medianBlur( frames[0], frames[0], 3 );
			framePos++;
			while ( framePos != frameAmount && framePos < frameCount ) {
				cap.read( inputFrame );
				cvtColor( inputFrame, frames[framePos], CV_RGB2GRAY );
				medianBlur( frames[framePos], frames[framePos], 3 );
				framePos++;
			}
			
			framePos = 0;

		} else {
			
			for ( int i = 1; i < frameAmount; i++ ) frames[i - 1] = frames[i];
			Mat inputFrame;
			cap.read( inputFrame );
			cvtColor( inputFrame, frames[0], CV_RGB2GRAY );
			framePos++;
		}
		
		cv::imshow( "test", frames[frameAmount - 1] );
		cv::waitKey( 40 );

		Mat sobelResult, xOrder, yOrder;
		for ( int i = 0; i < frameAmount; i++ ) {

			Sobel( frames[i], sobelResult, CV_16S, 1, 0 );
			convertScaleAbs( sobelResult, xOrder, 0.5 );
		}


	}


}