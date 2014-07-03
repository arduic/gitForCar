//============================================================================
// Name        : trackingTest.hpp
// Author      : Patrick
// Version     : 0.1
// Date Created: Sep 24, 2013
// Date Edited :
// Copyright   : Your copyright notice
// Description : Header file for the respective cpp file. The important parts are the constants. HMIN - VMAX are the thresholds which ideally would be calculated live rather than being constants but for now.
//				 The initals represent a base line for the image position. Currently these are completely wrong and will obviously need to be tweaked.
//				 Lastly are the K values which represent how much we care about X vs area for the scoring algorithm. They are nothing more than scales to eachother.
//============================================================================

#ifndef TRACKINGTEST_HPP_
#define TRACKINGTEST_HPP_

#include <cv.h>
using namespace cv;
/*******************************************************************************
 * 					Constants
 *******************************************************************************/
//Obviously these will need to be played with and if I was super cool I would probably apply this scoring concept to the limits thus evaluating where the threshold should be. Thing is, i'm lazy right now.
#define INITAL_X 640
#define INITAL_AREA 75000
#define HMIN 100
#define SMIN 38
#define VMIN 12
#define HMAX 120
#define SMAX 115
#define VMAX 128
#define KX 2
#define KAREA 1

//kp ki kd will not be defined as they are better used with tuning. the inital stuff is used incase target is lost.
//Scoring should also have a scale of sorts but that also needs to be tuned I think.
/*******************************************************************************
 * 					Functions
 *******************************************************************************/
double scoreObject(double x, double area, double previousX, double previousArea, double kx, double kArea);	//This will calculate how likely it is that this object is the target based on movement from previous location

//These will be called for both the X location and the Area, important calculate deltaTime immediately after sensor data. DO NOT DO IT INLINE WITH FUNCTION
double PIDArea(double ideal, double preVal, double curVal, double dt, double &integral, double kp, double ki, double kd);
double PIDTurning(double ideal, double preVal, double curVal, double dt, double kp, double kd);

int imageDetection(Mat image, double &x, double &area);

#endif /* TRACKINGTEST_HPP_ */
