/*
 * LEDDriver.cpp
 *
 *  Created on: Feb 1, 2015
 *      Author: Chris
 *
 *  Drives LEDs GPIO 8, 16, 17, and 18
 */

#include "header.h"
#include "LED_Driver.hpp"

LED_Driver::LED_Driver() {
	// TODO Auto-generated constructor stub
	GPIO_Setup_LED();
}

LED_Driver::~LED_Driver() {
	// TODO Auto-generated destructor stub
}


void LED_Driver::GPIO_Setup_LED(void)
{
	EALLOW;

	// Enable GPIO outputs on GPIO8 - GPIO11, set high
	GpioDataRegs.GPASET.bit.GPIO8 = 1; // Load output latch
	GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 0; // GPIO8 = GPIO8
	GpioCtrlRegs.GPADIR.bit.GPIO8 = 1; // GPIO8 = output

	GpioDataRegs.GPASET.bit.GPIO16 = 1; // Load output latch
	GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 0; // GPIO9 = GPIO9
	GpioCtrlRegs.GPADIR.bit.GPIO16 = 1; // GPIO9 = output

	GpioDataRegs.GPASET.bit.GPIO17 = 1; // Load output latch
	GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 0; // GPIO10 = GPIO10
	GpioCtrlRegs.GPADIR.bit.GPIO17 = 1; // GPIO10 = output

	GpioDataRegs.GPASET.bit.GPIO18 = 1; // Load output latch
	GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 0; // GPIO10 = GPIO10
	GpioCtrlRegs.GPADIR.bit.GPIO18 = 1; // GPIO10 = output
	EDIS;
}

void LED_Driver::flashLED()
{
//	static unsigned int count = 0;
//
//	if (count >= (5150 - pulse_width * 5050)) {
		GpioDataRegs.GPATOGGLE.bit.GPIO8 = 1;
		GpioDataRegs.GPATOGGLE.bit.GPIO16 = 1;
		GpioDataRegs.GPATOGGLE.bit.GPIO17 = 1;
		GpioDataRegs.GPATOGGLE.bit.GPIO18 = 1;
//		count = 0;
//	}
//	++count;
}
