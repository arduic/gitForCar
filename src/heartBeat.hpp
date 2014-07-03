/*
 * heartBeat.hpp
 *
 *  Created on: Sep 24, 2013
 *      Author: patrick
 */

#ifndef HEARTBEAT_HPP_
#define HEARTBEAT_HPP_

/*******************************************************************************
 * 					Constants
 *******************************************************************************/
#define ACCEPTABLE_TIME 3	//In seconds how long until it's considered a lost connection to the safety trigger
#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define MAX_BUF 64

/*******************************************************************************
 * 					Functions
 *******************************************************************************/
//void setupHeart();	//Setup the heartbeat for future use.
//void resetTime();	//Reset the count since last heartbeat
void emergencyShutDown(int x);	//Carefully! shutdown the car, maybe allow reset but probably will require hardware reset

void* loopHeart(void* arg);
void* loopKiller(void* arg);
int setupGPIOInterrupt(unsigned int* gpio);


#endif /* HEARTBEAT_HPP_ */
