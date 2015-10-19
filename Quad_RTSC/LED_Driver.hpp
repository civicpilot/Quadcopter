/*
 * LEDDriver.hpp
 *
 *  Created on: Feb 1, 2015
 *      Author: Chris
 */

#ifndef LEDDRIVER_HPP_
#define LEDDRIVER_HPP_

class LED_Driver {
public:
	LED_Driver();
	~LED_Driver();

public:
	void GPIO_Setup_LED(void);
	void flashLED(void);
};

#endif /* LEDDRIVER_HPP_ */
