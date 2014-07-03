//============================================================================
// Name        : pwmController.cpp
// Author      : Patrick
// Version     : 0.1
// Date Created: Sep 24, 2013
// Date Edited :
// Copyright   : Your copyright notice
// Description : This controls PWM output for motors and servos. It will do the inital setup for all PWM's than export a single PWM.
// 				 From there they can set and read duty cycles and frequencies as well as supposedly turn off and on with a single command seperate from duty cycle.
//============================================================================

//Note to make this compile in eclipse you need to make it support C++11 using this. http://stackoverflow.com/questions/9131763/eclipse-cdt-c11-c0x-support
//Not all of this uses C++11 either (atol) but I can't find documentation on it's replacment.

//Ummmm so issue! Fact is the pwm files end in .14 .15 .16 in the order they are created. but having a 2d array that needs to be flipped through to find the proper relationship is just way to much for such a small issue.
//Thus i expect the user to pass the number 14,15,16 based on when they created the pin

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
 * 					Exports
 *******************************************************************************/
//Needs to be called one time so that exportPWM works. I'm not sure why this is done but here is were I found the commands.
//http://www.phys-x.org/rbots/index.php?option=com_content&view=article&id=106:lesson-3-beaglebone-black-pwm&catid=46:beaglebone-black&Itemid=81
int setupPWM(void){
	int len,fd;
	char buf[MAX_BUF];

	fd = open(SYSFS_PWM_DIR "/bone_capemgr.8/slots",O_WRONLY);	//Open the slots file for writing.
	if (fd < 0){	//If the file open command threw an error print out the error
		printf ("Error opening file: %s\n",strerror(errno));
		return -1;
	}
	len = snprintf(buf,sizeof(buf),"am33xx_pwm");
	write(fd,buf,len);
	close(fd);

	return 0;
}
//export the pin to be a PWM (should probably have an unexport but :P)
int exportPWM(unsigned int pin){
	int len;
	int pad=8;
	char buf[MAX_BUF];
	FILE * pFile;

	pFile = fopen(SYSFS_PWM_DIR "/bone_capemgr.8/slots","w");	//Open the slots for writing
	if (pFile==NULL){	//If the file open command threw an error print out the error
		printf ("Error opening file: %s\n",strerror(errno));
		return -1;
	}
	//Calculate what pin this is and what pad it's on	We add 1 to pad because if it's above 46 they mean pad 9;
	if(pin>46){
		pin-=46;
		pad++;
	}
	//Format the command to be written write it and than leave.
	len = snprintf(buf,sizeof(buf),"bone_pwm_P%d_%d",pad,pin);
	fwrite(buf,sizeof(char),len,pFile);
	fclose(pFile);

	return 0;
}

/*******************************************************************************
 * 					Frequency Control
 *******************************************************************************/

unsigned long setFrequency(unsigned long frequency,unsigned int pin,unsigned int place){
	int len;
	int pad=8;
	char buf[MAX_BUF];
	long nanoSeconds = floor(1000000000/frequency);	//Calculate the value to write that frequency into the PWM
	FILE * pFile;
	//Calculate what pin this is and what pad it's on
	if(pin>46){
		pin-=46;
		pad++;
	}
	//Format the directory to open
	len = snprintf(buf,sizeof(buf),SYSFS_PWM_DIR "/ocp.2/pwm_test_P%d_%d.%d/period",pad,pin,place);
	pFile = fopen(buf,"w");	//Open the file setup by the above command.
	if (pFile==NULL){	//If the file open command threw an error print out the error
		printf ("Error opening file: %s\n",strerror(errno));
		return -1;
	}
	//Write the amount of nano seconds that the pin will oscilate.
	len = snprintf(buf, sizeof(buf), "%ld", nanoSeconds);
	fwrite(buf,sizeof(char),len,pFile);
	fclose(pFile);
	return 0;
}
unsigned long getFrequency(unsigned int pin,unsigned int place){
	int len;
	int pad=8;
	char buf[MAX_BUF];
	FILE * pFile;

	//Calculate what pin this is and what pad it's on
	if(pin>46){
		pin-=46;
		pad++;
	}
	//Format the directory to open
	len = snprintf(buf,sizeof(buf),SYSFS_PWM_DIR "/ocp.2/pwm_test_P%d_%d.%d/period",pad,pin,place);

	pFile = fopen(buf,"r");	//Open the file setup by the above command.
	if (pFile==NULL){	//If the file open command threw an error print out the error
		printf ("Error opening file: %s\n",strerror(errno));
		return -1;
	}

	fgets(buf,10,pFile);
	fclose(pFile);
	return atol(buf);
}

/*******************************************************************************
 * 					Duty Cycle Control
 *******************************************************************************/

int setDutyCycle(unsigned int duty,unsigned int pin,unsigned int place){
	int len;
	int pad=8;
	char buf[MAX_BUF];
	long nanoSeconds = floor(getFrequency(pin,place)*((double)duty/255.0));	//Calculate the value to write that duty cycle for the PWM
	FILE * pFile;

	//Calculate what pin this is and what pad it's on
	if(pin>46){
		pin-=46;
		pad++;
	}
	//Format the directory to open
	len = snprintf(buf,sizeof(buf),SYSFS_PWM_DIR "/ocp.2/pwm_test_P%d_%d.%d/duty",pad,pin,place);

	pFile = fopen(buf,"w");
	if (pFile==NULL){	//If the file open command threw an error print out the error
		printf ("Error opening file: %s\n",strerror(errno));
		return -1;
	}

	len = snprintf(buf, sizeof(buf), "%ld", nanoSeconds);
	fwrite(buf,sizeof(char),len,pFile);
	fclose(pFile);
	return 0;
}
int getDutyCycle(unsigned int pin,unsigned int place){
	int len;
	int pad=8;
	int temp = pin;	//We need temp because pin is modified but we call getFrequency.
	char buf[MAX_BUF];
	FILE * pFile;

	//Calculate what pin this is and what pad it's on
	if(pin>46){
		pin-=46;
		pad++;
	}
	//Format the directory to open
	len = snprintf(buf,sizeof(buf),SYSFS_PWM_DIR "/ocp.2/pwm_test_P%d_%d.%d/duty",pad,pin,place);

	pFile = fopen(buf,"w");
	if (pFile==NULL){	//If the file open command threw an error print out the error
		printf ("Error opening file: %s\n",strerror(errno));
		return -1;
	}
	fgets(buf,10,pFile);
	fclose(pFile);
	return floor(getFrequency(temp,place)/atol(buf));	//Calculate the duty cycle from the file but put it in a scale of 0-255 There could be some mathematical horror where it infinitely trys to accelerate but is maintaining speed. not sure.
}

/*******************************************************************************
 * 					OnOff Control
 *******************************************************************************/
//So there is this wonderful file called run in the PWM directory which I BELIEVE (need to test) will allow me to say hey I know duty cycle is this but don't actually output anything unless this is 1.
int startStop(unsigned int state,unsigned int pin,unsigned int place){
	if(state>1){
		printf("%s\n","State can only be 0 or 1");
		return -2;
	}
	int len;
	int pad=8;
	char buf[MAX_BUF];
	FILE * pFile;
	//Calculate what pin this is and what pad it's on
	if(pin>46){
		pin-=46;
		pad++;
	}
	//Format the directory to open
	len = snprintf(buf,sizeof(buf),SYSFS_PWM_DIR "/ocp.2/pwm_test_P%d_%d.%d/run",pad,pin,place);
	pFile = fopen(buf,"w");	//WRITE LATER
	if (pFile==NULL){	//If the file open command threw an error print out the error
		printf ("Error opening file: %s\n",strerror(errno));
		return -1;
	}
	len = snprintf(buf,sizeof(buf),"%d",state);
	fwrite(buf,sizeof(char),len,pFile);
	fclose(pFile);
	return 0;
}
