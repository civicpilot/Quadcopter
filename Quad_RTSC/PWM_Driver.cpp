/*
 * PWM_Driver.cpp
 *
 *  Created on: Feb 1, 2015
 *      Author: Chris
 *
 *  Drives PWMs 3, 4, 7, and 8
 */

#include "header.h"
#include "PWM_Driver.hpp"

#define MOTOR_START_UP 80000
#define PERIOD		   56250

PWM_Driver::PWM_Driver() {
	// TODO Auto-generated constructor stub
	PWM_setup();
}

void PWM_Driver::PWM_setup(void)
{

	// Delay so that Motors can be initialized
	DELAY_US(MOTOR_START_UP);

	// Setup PWMs for the ESCs
	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;

	// Setup TBCLK
	EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count up
	EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count up
	EPwm7Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count up
	EPwm8Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count up
	EPwm3Regs.TBPRD = PERIOD; // Set timer period
	EPwm4Regs.TBPRD = PERIOD; // Set timer period
	EPwm7Regs.TBPRD = PERIOD; // Set timer period
	EPwm8Regs.TBPRD = PERIOD; // Set timer period
	EPwm3Regs.TBCTL.bit.PHSEN = TB_DISABLE; // Disable phase loading
	EPwm4Regs.TBCTL.bit.PHSEN = TB_DISABLE; // Disable phase loading
	EPwm7Regs.TBCTL.bit.PHSEN = TB_DISABLE; // Disable phase loading
	EPwm8Regs.TBCTL.bit.PHSEN = TB_DISABLE; // Disable phase loading
	EPwm3Regs.TBPHS.half.TBPHS = 0x0000; // Phase is 0
	EPwm4Regs.TBPHS.half.TBPHS = 0x0000; // Phase is 0
	EPwm7Regs.TBPHS.half.TBPHS = 0x0000; // Phase is 0
	EPwm8Regs.TBPHS.half.TBPHS = 0x0000; // Phase is 0
	EPwm3Regs.TBCTR = 0x0000; // Clear counter
	EPwm4Regs.TBCTR = 0x0000; // Clear counter
	EPwm7Regs.TBCTR = 0x0000; // Clear counter
	EPwm8Regs.TBCTR = 0x0000; // Clear counter
	EPwm3Regs.TBCTL.bit.HSPCLKDIV = 0x4; // Clock ratio to SYSCLKOUT
	EPwm4Regs.TBCTL.bit.HSPCLKDIV = 0x4; // Clock ratio to SYSCLKOUT
	EPwm7Regs.TBCTL.bit.HSPCLKDIV = 0x4; // Clock ratio to SYSCLKOUT
	EPwm8Regs.TBCTL.bit.HSPCLKDIV = 0x4; // Clock ratio to SYSCLKOUT
	EPwm3Regs.TBCTL.bit.CLKDIV = TB_DIV4;
	EPwm4Regs.TBCTL.bit.CLKDIV = TB_DIV4;
	EPwm7Regs.TBCTL.bit.CLKDIV = TB_DIV4;
	EPwm8Regs.TBCTL.bit.CLKDIV = TB_DIV4;

	// Setup shadow register load on ZERO
	EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	EPwm4Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm4Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	EPwm7Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm7Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	EPwm8Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm8Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;

	// Set Compare values
	EPwm3Regs.CMPA.half.CMPA = 0; // Set compare A value
	EPwm4Regs.CMPA.half.CMPA = 0; // Set compare A value
	EPwm7Regs.CMPA.half.CMPA = 0; // Set compare A value
	EPwm8Regs.CMPA.half.CMPA = 0; // Set compare A value

	// Set actions
	EPwm3Regs.AQCTLA.bit.ZRO = AQ_SET; // Set PWM1A on Zero
	EPwm3Regs.AQCTLA.bit.CAU = AQ_CLEAR; // Clear PWM1A on event A, up count
	EPwm4Regs.AQCTLA.bit.ZRO = AQ_SET; // Set PWM1A on Zero
	EPwm4Regs.AQCTLA.bit.CAU = AQ_CLEAR; // Clear PWM1A on event A, up count
	EPwm7Regs.AQCTLA.bit.ZRO = AQ_SET; // Set PWM1A on Zero
	EPwm7Regs.AQCTLA.bit.CAU = AQ_CLEAR; // Clear PWM1A on event A, up count
	EPwm8Regs.AQCTLA.bit.ZRO = AQ_SET; // Set PWM1A on Zero
	EPwm8Regs.AQCTLA.bit.CAU = AQ_CLEAR; // Clear PWM1A on event A, up count

	// Interrupt where we will change the Compare Values
	EPwm3Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO; // Select INT on Zero event
	EPwm3Regs.ETSEL.bit.INTEN = 1; // Enable INT
	EPwm3Regs.ETPS.bit.INTPRD = ET_1ST; // Generate INT on 1st event

	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
	EDIS;
}
// This is called from within the CommandTransform
void PWM_Driver::PWM_set(float *PWM)
{
	EPwm7Regs.CMPA.half.CMPA = PWM[2]; // Motor 1
	EPwm8Regs.CMPA.half.CMPA = PWM[0]; // Motor 2
	EPwm3Regs.CMPA.half.CMPA = PWM[1]; // Motor 3
	EPwm4Regs.CMPA.half.CMPA = PWM[3]; // Motor 4
}
