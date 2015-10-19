/*
 * PWM_Driver.hpp
 *
 *  Created on: Feb 1, 2015
 *      Author: Chris
 */

#ifndef PWM_DRIVER_HPP_
#define PWM_DRIVER_HPP_

class PWM_Driver {
public:
	PWM_Driver();
	~PWM_Driver();

	void PWM_setup(void);
	void PWM_set(float *PWM);
};

#endif /* PWM_DRIVER_HPP_ */
