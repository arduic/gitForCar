//============================================================================
// Name        : pwmController.hpp
// Author      : Patrick
// Version     : 0.1
// Date Created: Sep 24, 2013
// Date Edited :
// Copyright   : Your copyright notice
// Description : The constant setting the directory is 100% crucial and accurate currently. Otherwise writting to PWM files is impossible.
//============================================================================

#ifndef PWMCONTROLLER_HPP_
#define PWMCONTROLLER_HPP_

/*******************************************************************************
 * 					Constants
 *******************************************************************************/
#define SYSFS_PWM_DIR "/sys/devices"	//Directory for the pwm's Also the buffer size, 64 is arbitrary
#define MAX_BUF 64


/*******************************************************************************
 * 					Functions
 *******************************************************************************/
int setupPWM(void);	//Needs to be called one time so that exportPWM works.
int exportPWM(unsigned int pin);	//export the pin to be a PWM (should probably have an unexport but :P)

unsigned long setFrequency(unsigned long frequency,unsigned int pin,unsigned int place);	//Will set the frequency of the pin to be driven by PWM. Frequency is rated in hZ
unsigned long getFrequency(unsigned int pin,unsigned int place);	//Return the frequency of the pin in hZ unless error

int setDutyCycle(unsigned int duty,unsigned int pin,unsigned int place);	//Set the duty cycle in percentage 0-255
int getDutyCycle(unsigned int pin,unsigned int place);	//Return the duty cycle 0-255 unless error

int startStop(unsigned int state, unsigned int pin,unsigned int place);	//Should shutoff the PWM no matter the duty cycle.

#endif /* PWMCONTROLLER_HPP_ */
