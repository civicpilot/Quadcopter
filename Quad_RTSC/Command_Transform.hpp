/*
 * Motor_Controller.hpp
 *
 *  Created on: Feb 1, 2015
 *      Author: Chris
 */

#ifndef MOTOR_CONTROLLER_HPP_
#define MOTOR_CONTROLLER_HPP_

#include "PWM_Driver.hpp"
#include "LED_Driver.hpp"

class Command_Transform {
public:
	Command_Transform();
	~Command_Transform();

	void CommandMixer(float *motorCmd, float *pidCmd, unsigned int flightMode);

private:
	PWM_Driver *m_pwm_driver;
	LED_Driver *m_led_driver;
};

#endif /* MOTOR_CONTROLLER_HPP_ */
