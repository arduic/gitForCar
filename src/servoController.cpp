//============================================================================
// Name        : servoController.cpp
// Author      : Patrick
// Version     : 0.1
// Date Created: Sep 24, 2013
// Date Edited :
// Copyright   : Your copyright notice
// Description : Objective of the final cpp file. Use the predefined functions from pwmController to have an easy way to turn right and left while maintaing thresholds for valid input.
//============================================================================

#include "servoController.hpp"
#include "pwmController.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <math.h>

using namespace std;

/*******************************************************************************
 * 					direction controls
 *******************************************************************************/
int setDirection(unsigned int direction, unsigned int pin,unsigned int place){
	direction = direction > 0 ? direction : 0;
	direction = direction < 255 ? direction : 255;
	setDutyCycle(direction,pin,place);
	return 0;
}

/*******************************************************************************
 * 					turn controls
 *******************************************************************************/
int turnLeft(unsigned int pin,unsigned int place){
	int duty = getDutyCycle(pin,place);
	duty = duty > 0 ? duty : 1;
	duty = duty < 255 ? duty : 255;
	if(duty<STRAIGHT)
		setDutyCycle(getDutyCycle(pin,place)-1,pin,place);
	else
		setDutyCycle(STRAIGHT-1,pin,place);
	return 0;
}
int turnRight(unsigned int pin,unsigned int place){
	int duty = getDutyCycle(pin,place);
	duty = duty > 0 ? duty : 0;
	duty = duty < 255 ? duty : 254;
	if(duty>STRAIGHT)
		setDutyCycle(getDutyCycle(pin,place)+1,pin,place);
	else
		setDutyCycle(STRAIGHT+1,pin,place);
	return 0;
}
