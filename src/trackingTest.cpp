//============================================================================
// Name        : trackingTest.cpp
// Author      : Patrick
// Version     : 0.2
// Date Created: Sep 24, 2013
// Date Edited :
// Copyright   : Your copyright notice
// Description : Objective of the final cpp file. Find all objects on an image, value which one is the best, and than use PID to predict the actual location of that object assuming it is the previous object.
//============================================================================
//Version changes. Fixed some serious bugs.

#include <cv.h>
#include <highgui.h>
//#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "trackingTest.hpp"
#include "/usr/local/include/opencv2/opencv.hpp"

using namespace std;
using namespace cv;
//So this score method returns 0 is perfect match higher the number the worse the object is. kx and karea are passed because eventually they might be dynamically calculated
double scoreObject(double x, double area, double previousX, double previousArea, double kx, double kArea){
	return abs(x-previousX)*kx+abs(area-previousArea)*kArea;
}
//This is just a general PID function nothing special except that integral is pass by reference
//Actually an important note. turning holds the exact OPPOSITE affect as to area when it comes to how to take action. As such we need this negative stuff.
double PIDArea(double ideal, double preVal, double curVal, double dt, double &integral, double kp, double ki, double kd){
    double proportion = (curVal-ideal)*kp;	//If this is negative we are losing target thus go faster thus return negative of this
    double deriv = ((curVal-preVal)/dt)*kd;	//If this is negative we are falling behind over time thus go faster thus return negative of this
    double ret;
    //if turning we don't use integral if turning we do use integral and reverse the logic of ideal and current because the effects are reversed (bigger means slow down where as higer (right) means go negative)
    integral += (curVal-ideal)*dt;	//If negative we have net fallen behind thus we use negative of this value
    ret = -proportion-deriv-(integral*ki);	//If the return is positive go faster. If it is negative go slower
    return ret;

}


double PIDTurning(double ideal, double preVal, double curVal, double dt, double kp, double kd){
	double ret;
    double proportion = (curVal-ideal)*kp;
    double deriv = ((curVal-preVal)/dt)*kd;
    ret = proportion+deriv;
    //Bound the return so that if the object is say to the right the car WILL NOT turn left. no matter the derivitaive
    if((abs(proportion)==proportion)!=(abs(ret)==ret)){
    	return 0;
    }else{
    	return ret;
    }
}



//Find the image, it returns a 0 on sucsses and returns the x and area by reference, this will return the best scored image.
int imageDetection(Mat image, double &x, double &area){
	//printf("%s\n","1");
	Mat HSV,threshold;
	//Mark every point that is within a certain HSV color range. (RGB sucks thus HSV)
	cvtColor(image,HSV,COLOR_BGR2HSV);
	inRange(HSV,Scalar(HMIN,SMIN,VMIN),Scalar(HMAX,SMAX,VMAX),threshold);
	//printf("%s\n","2");
	//Get rid of floating dots and expand ones that are close together
	Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
	//dilate with larger element so make sure object is easily linked together
	Mat dilateElement = getStructuringElement( MORPH_RECT,Size(8,8));
	erode(threshold,threshold,erodeElement);
	erode(threshold,threshold,erodeElement);
	dilate(threshold,threshold,dilateElement);
	dilate(threshold,threshold,dilateElement);
	//printf("%s\n","3");
	//Find the contours (objects) on this image
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(threshold,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
	if(contours.size()==0 && hierarchy.size()==0)
		return -2;
	//printf("%s\n","4");
	//Loop through all of the contours we have found (I may be able to remove the hiearchy that findContours made but I don't know how else to do this for loop yet)
	double score = 9000000;
	double theX,theArea;
	for (int index = 0; index >= 0; index = hierarchy[index][0]) {
		//Find the x position and area of the image
		//printf("%s\n","5");
		//Moments moment = moments((Mat)contours[index]);
		Moments moment = moments((Mat)contours[index],true);
		//Store the evaluation in a temp directory and compare if this score is LOWER (0 means target did not move) if it is update the possible position
		double tempArea = moment.m00;
		double tempX = moment.m10/moment.m00;
		//printf("%s\n","6");
		double temp = scoreObject(moment.m10/moment.m00,moment.m00,x,area,KX,KAREA);
		//double temp = scoreObject(tempX,tempArea,x,area,KX,KAREA);
		cout << tempX << "\t" << tempArea << "\t" << temp << endl;
		if(temp < score){
			theX = moment.m10/moment.m00;
			theArea = moment.m00;
			score = temp;
		}
		//printf("%s\n","7");

	}
	//printf("%s\n","8");
	//printf("x location: %f, area: %f",theX,theArea);
	//If we found at least 1 target update the previous locations to be the new ones and return 0 (no error)
	if (score != 9000000){
		x = theX;
		area = theArea;
		return 0;
	}else{	//If we found no valid targets than we return -1 and probably shut down
		return -1;
	}
}
