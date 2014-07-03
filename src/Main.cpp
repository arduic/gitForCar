//============================================================================
// Name        : Main.cpp
// Author      : Patrick
// Version     : 0.1
// Date Created: Sep 24, 2013
// Date Edited :
// Copyright   : Your copyright notice
// Description : Main loop to follow a person
//				 This needs to first run a setup, than find object, evaluate best one, estimate where it is.
//				 Estimate how much to turn and accelerate.
//				 Listen to the heart on seperate pthreads.
//============================================================================
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <iostream>
#include "pwmController.hpp"
#include "servoController.hpp"
#include "heartBeat.hpp"
#include <pthread.h>
#include <errno.h>
#include <ctype.h>
#include <cv.h>
#include <highgui.h>
#include "trackingTest.hpp"
#include "/usr/local/include/opencv2/opencv.hpp"

//List of things to implement in order
//heartBeat, PWM, Servo, tracking, main

//So what main will do.
//in a while(1)
//find objects, evaluate for best object, if no valid objects stap!, else use PID to estimate where it is.
//Evaluate where the object is to turn the car.
//Evaluate how far the object is to control speed of the car.
//With the interrupts for heartBeat shutdown the car if we ever lose long range coms, also if we see the stop radio command STOP
using namespace std;
using namespace cv;

//Global variable accessed by killer thread to set when the program should start.
int startCap = 0;

int main( int argc, const char* argv[] ) {

	double x = INITAL_X;	//Where object "should" start
	double prev_x = x;
	double area = INITAL_AREA;	//Size object "should" start as
	double prev_area = area;
	double areaInt = 0.0;	//integral value for area (initally it is zero)
	double PID_area;
	double PID_x;
	struct timespec start,end;

	VideoCapture cameraImage(0);
	cameraImage.set(CV_CAP_PROP_FRAME_WIDTH,1280);
	cameraImage.set(CV_CAP_PROP_FRAME_HEIGHT,720);
    if(!cameraImage.isOpened()){
    	printf("%s\n","Failed to connect to the camera.");
    	return -1;
    }
    printf("%s\n","Camera is opened!");
	Mat image;



	//This is where I need to initalize the PWM and servo motors (untested on RC car)



	cameraImage >> image;
	clock_gettime(0, &start);
	if(image.empty()){
		printf("%s\n","No image");
		return -2;
	}
	imageDetection(image,x,area);
	PIDTurning(INITAL_X,prev_x,x,0.0,0.7,0.3);
	PIDArea(INITAL_AREA,prev_area,area,0.0,areaInt,0.7,0.2,0.3);

	while(1){
		cameraImage >> image;
		clock_gettime(0, &end);
		double difference = (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec)/1000000000.0;
		start = end;
		if(image.empty()){
			printf("%s\n","No image");
			return -2;
		}
		prev_x=x;
		prev_area=area;
		imageDetection(image,x,area);
		PID_x=PIDTurning(INITAL_X,prev_x,x,difference,0.7,0.3);
		PID_area=PIDArea(INITAL_AREA,prev_area,area,difference,areaInt,0.7,0.2,0.3);
		//This is where I would set the Servo and the PWM value
		printf("Alright the turning PID was %f and the Area PID was %f",PID_x,PID_area);
	}

	//imwrite("test.png",image);


	/*double x = 640;
	double area = 75000;
	double ZERO = 0.0;
	double areaInt = 0.0;
	Mat image = imread(argv[1],1);
	imageDetection(image,x,area);
	printf("calculated x: %f area: %f\n",x,area);
	printf("turning offset from center: %f\n",PIDFunction(640,640,x,0.25,ZERO,0.7,0,0.3,true));
	printf("speed value unscaled: %f\n\n",PIDFunction(11000,11000,area,0.25,areaInt,0.7,0.2,0.3,false));*/


	/*
	double x = 1043;
	double area = 28789;
	struct timespec start,end;
	clock_gettime(0, &start);	//ECLIPSE!!! apparently, even though CLOCK_REALTIME will build and run I tested this. Eclipse still thinks it's an error and will flag it. Thus use the value of CLOCK_REALTIME to shut it up
	int trials = 1000;
	VideoCapture cameraImage(0);
	cameraImage.set(CV_CAP_PROP_FRAME_WIDTH,1920);
	cameraImage.set(CV_CAP_PROP_FRAME_HEIGHT,1080);
    if(!cameraImage.isOpened()){
    	printf("%s\n","Failed to connect to the camera.");
    	return -1;
    }
    cout << cameraImage.get(CV_CAP_PROP_FORMAT) << endl;
    printf("%s\n","Alright let's get it started in here");
	Mat image;
	for(int i=0; i<trials; i++){
		cameraImage >> image;
		imageDetection(image,x,area);
	}

	clock_gettime(0,&end);
	double difference = (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec)/1000000000.0;
	double fps = (1.0/difference)*trials;
	cout << "It took " << difference << " seconds to process " << trials << " frames" << endl;
	cout << "my fps is: " << fps << endl;
	*/


	/*if(setupPWM()){
		return -1;
	}
	cout << "help1" << endl;
	if(exportPWM(13)){
		return -1;
	}
	cout << "help2" << endl;
	sleep(1);		//this is in seconds no milliseconds. Windows it's mill linux uses this and usleep
	if(setFrequency(50,13,14)){
		return -1;
	}
	cout << "help3" << endl;
	if(setDutyCycle(127,13,14)){
		return -1;
	}
	cout << "help4" << endl;
	cin.ignore();
	if(startStop(1,13,14)){
		return -1;
	}
	cout << "help5" << endl;
	return 0;*/


	//Alright we are going to create 2 threads but we still need to set priority using the schedule setting for pthread later on.
	//pthread_t heart, killer;
	/*if(pthread_create( &heart, NULL,loopHeart,(void*) 15)!=0){
		printf("AHH FAILURE");
		return -1;
	}
	if(pthread_create( &killer, NULL,loopKiller,(void*) 3)!=0){
		printf("CANTASTROPHE");
		return -1;
	}*/


	/*
	 *   //Here would be a sample initalization and frame grab.
    double x=640;
    double ZERO = 0.0;
    double prevX = 640;
    double area = 11000;
    double prevArea = 11000;
    double areaInt = 0;
    double temp;
    Mat image;
    image = imread(argv[1],1);
    imageDetection(image,x,area);
    printf("calculated x: %f area: %f\n",x,area);
    printf("%f\n",PIDFunction(640,prevX,x,0.25,ZERO,0.7,0,0.3,true));
    printf("%f\n",PIDFunction(11000,prevArea,area,0.25,areaInt,0.7,0.2,0.3,false));
    prevX=x;
    prevArea=area;
    */

	return 0;

}
