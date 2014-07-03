//============================================================================
// Name        : servoController.hpp
// Author      : Patrick
// Version     : 0.1
// Date Created: Sep 24, 2013
// Date Edited :
// Copyright   : Your copyright notice
// Description : This hpp file defines where straight is on the servo for the car. NEEDS TO BE TESTED
//============================================================================

#ifndef SERVOCONTROLLER_HPP_
#define SERVOCONTROLLER_HPP_

/*******************************************************************************
 * 					Constants
 *******************************************************************************/
#define STRAIGHT 100 	//This is not straight will need to be tested

/*******************************************************************************
 * 					Functions
 *******************************************************************************/
int setDirection(unsigned int direction, unsigned int pin,unsigned int place);

int turnLeft(unsigned int pin,unsigned int place);
int turnRight(unsigned int pin,unsigned int place);

#endif /* SERVOCONTROLLER_HPP_ */
