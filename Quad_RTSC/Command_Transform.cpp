/*
 * Command_Transform.cpp
 *
 *  Created on: Feb 1, 2015
 *      Author: Chris
 *
 *  Description: This class will accept a command array [4] and a pid array [4], and then
 *  			 mix the signals together to form a resulting quad movement.
 */

#include "Command_Transform.hpp"

#define SATURATION	56250
#define START_DUTYCYCLE 3200
#define COMMAND_OFFSET 3500

Command_Transform::Command_Transform() {
	// TODO Auto-generated constructor stub
	m_pwm_driver = new PWM_Driver();
	m_led_driver = new LED_Driver();
}

Command_Transform::~Command_Transform() {
	// TODO Auto-generated destructor stub
	delete m_pwm_driver;
	delete m_led_driver;
}
// Should be called from a TSK
void Command_Transform::CommandMixer(float *motorCmd, float *pidCmd, unsigned int flightMode)
{
	// PID controllers (roll, pitch, yaw, altitude) or (0,1,2,3)
	float PWM[4] = {0};

	// X mode
	if (flightMode == 0){
//		if (motorCmd[4] != 0 || motorCmd[5] != 0 || motorCmd[6] != 0) {
			//						   thrust,		  roll,		  pitch,	      yaw
			PWM[2] = START_DUTYCYCLE + motorCmd[0] - pidCmd[0] - pidCmd[1];// + pidCmd[2];	// Motor 1
			PWM[0] = START_DUTYCYCLE + motorCmd[0] - pidCmd[0] + pidCmd[1];// - pidCmd[2];	// Motor 2
			PWM[1] = START_DUTYCYCLE + motorCmd[0] + pidCmd[0] + pidCmd[1];// + pidCmd[2];	// Motor 3
			PWM[3] = START_DUTYCYCLE + motorCmd[0] + pidCmd[0] - pidCmd[1];// - pidCmd[2];	// Motor 4
//		}
//		else {
//			//						        thrust,		   roll,		   pitch,	       yaw
//			PWM[2] = START_DUTYCYCLE + 3500*(motorCmd[0] - motorCmd[1] - motorCmd[2] + motorCmd[3]);	// Motor 1
//			PWM[0] = START_DUTYCYCLE + 3500*(motorCmd[0] - motorCmd[1] + motorCmd[2] - motorCmd[3]);	// Motor 2
//			PWM[1] = START_DUTYCYCLE + 3500*(motorCmd[0] + motorCmd[1] + motorCmd[2] + motorCmd[3]);	// Motor 3
//			PWM[3] = START_DUTYCYCLE + 3500*(motorCmd[0] + motorCmd[1] - motorCmd[2] - motorCmd[3]);	// Motor 4
//		}
	}

	// + mode
	if (flightMode == 1){
		if (motorCmd[4] != 0 || motorCmd[5] != 0 || motorCmd[6] != 0) {
			//						        thrust,		  roll,		  pitch,	      yaw
			PWM[2] = START_DUTYCYCLE + COMMAND_OFFSET*(motorCmd[0] - 0           - pidCmd[1]/2 + pidCmd[2]/4);	// Motor 1
			PWM[0] = START_DUTYCYCLE + COMMAND_OFFSET*(motorCmd[0] - pidCmd[0]/2 + 0           - pidCmd[2]/4);	// Motor 2
			PWM[1] = START_DUTYCYCLE + COMMAND_OFFSET*(motorCmd[0] + 0           + pidCmd[1]/2 + pidCmd[2]/4);	// Motor 3
			PWM[3] = START_DUTYCYCLE + COMMAND_OFFSET*(motorCmd[0] + pidCmd[0]/2 + 0           - pidCmd[2]/4);	// Motor 4
		}
	}

	// Used to determine PID for Pitch axis
	if (flightMode == 2){
		//						        thrust,		  roll,		    pitch,	      yaw
		PWM[2] = START_DUTYCYCLE + COMMAND_OFFSET*(motorCmd[0] - 0           - motorCmd[2] + 0          );	// Motor 1
		PWM[0] = START_DUTYCYCLE + COMMAND_OFFSET*(motorCmd[0] - 0           + motorCmd[2] - 0          );	// Motor 2
		PWM[1] = START_DUTYCYCLE + COMMAND_OFFSET*(motorCmd[0] + 0           + motorCmd[2] + 0          );	// Motor 3
		PWM[3] = START_DUTYCYCLE + COMMAND_OFFSET*(motorCmd[0] + 0           - motorCmd[2] - 0          );	// Motor 4


	}

	// Used to determine PID for Roll axis
	if (flightMode == 3){
		//						        thrust,		  roll,		    pitch,	      yaw
		PWM[2] = START_DUTYCYCLE + COMMAND_OFFSET*(motorCmd[0] - motorCmd[1] - 0           + 0          );	// Motor 1
		PWM[0] = START_DUTYCYCLE + COMMAND_OFFSET*(motorCmd[0] - motorCmd[1] + 0           - 0          );	// Motor 2
		PWM[1] = START_DUTYCYCLE + COMMAND_OFFSET*(motorCmd[0] + motorCmd[1] + 0           + 0          );	// Motor 3
		PWM[3] = START_DUTYCYCLE + COMMAND_OFFSET*(motorCmd[0] + motorCmd[1] - 0           - 0          );	// Motor 4


	}

	// Ensure commands do not become saturated by clipping at max and min values
	for(short i = 0; i < 4; i++)
		if (PWM[i] > SATURATION) {
			PWM[i] = SATURATION-1;
		} else if (PWM[i] < START_DUTYCYCLE) {
			PWM[i] = START_DUTYCYCLE;
		}

//	m_led_driver->flashLED();
	m_pwm_driver->PWM_set(PWM);
}

