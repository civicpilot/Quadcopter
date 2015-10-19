/*
 * I2C_Device.cpp
 *
 *  Created on: Jan 31, 2015
 *      Author: Chris
 */
#include "header.h"
#include "I2C_Device.hpp"

I2C_Device::I2C_Device(unsigned int devAddress)
{
	I2CA_Init();
	m_devAddr = devAddress;
	m_I2C_timeout = 0;
	for (int i = 0; i < 14; i++)
		m_I2C_buffer[i] = 0;
}

I2C_Device::~I2C_Device(void)
{
	delete [] m_I2C_buffer;
}

void I2C_Device::I2CA_Init(void)
{
	EALLOW;

	GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;    // Enable pull-up for GPIO28 (SDAA)
	GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;    // Enable pull-up for GPIO29 (SCLA)

	GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;  // Asynch input GPIO28 (SDAA)
	GpioCtrlRegs.GPAQSEL2.bit.GPIO29 = 3;  // Asynch input GPIO29 (SCLA)

	GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 2;   // Configure GPIO28 for SDAA operation
	GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 2;   // Configure GPIO29 for SCLA operation

	EDIS;

   I2caRegs.I2CSAR = 0x00;     		  // Slave address - EEPROM control code
   I2caRegs.I2CPSC.all = 6;           // Prescaler - 1 Mhz on module clk from 90MHz CPU
   I2caRegs.I2CCLKL = 10;             // 1MHz /4 = 250kHz
   I2caRegs.I2CCLKH = 5;              // 1MHz /3 = 333.3kHz
   I2caRegs.I2CIER.all = 0x3E;        // Enable SCD,XRDY,RRDY,ARDY,NACK interrupts
   I2caRegs.I2CMDR.bit.IRS = 1;       // Take I2C out of reset, Stop I2C when suspended
   I2caRegs.I2CFFTX.all = 0x0000;     // Disable FIFO mode and TXFIFO
   I2caRegs.I2CFFRX.all = 0x0000;     // Disable RXFIFO, clear RXFFINT
}

void I2C_Device::writeBytes(unsigned int regAddr, unsigned int numBytes, unsigned int *msg)
{
	I2caRegs.I2CSAR = m_devAddr;   			// Set slave address
	I2caRegs.I2CCNT = 1+numBytes;           // Set count to register address + buffer
	I2caRegs.I2CDXR = regAddr;     			// Send register address

	I2caRegs.I2CMDR.bit.TRX = 1;    		// Set to Transmit mode
	I2caRegs.I2CMDR.bit.MST = 1;    		// Set to Master mode
	I2caRegs.I2CMDR.bit.FREE = 1;   		// Run in FREE mode
	I2caRegs.I2CMDR.bit.STP = 1;    		// Stop when internal I2C_timeout becomes 0
	I2caRegs.I2CMDR.bit.STT = 1;    		// Send the start bit, transmission will follow
	while(I2caRegs.I2CSTR.bit.XRDY == 0 && m_I2C_timeout < 100000){m_I2C_timeout++;}; // Wait for buffer send to be ready
	//I2caRegs.I2CDXR = (devAddr<<1);  		// Send device write (W)
	m_I2C_timeout = 0;
	for(unsigned int i = 0; i < numBytes; i++)
	   {
		   while(I2caRegs.I2CSTR.bit.XRDY == 0 && m_I2C_timeout < 100000) {
			   m_I2C_timeout++;
		   }     // Wait for buffer send to be ready
		   I2caRegs.I2CDXR = msg[i];
	   }

//	DELAY_US(50);
	for (int a = 0; a < 10000; a++);

	m_I2C_timeout = 0;
}

void I2C_Device::writeByte(unsigned int regAddr, unsigned int msg0)
{
	unsigned int msg[1];
	msg[0] =  msg0;
	writeBytes(regAddr, 1, msg);
}


void I2C_Device::readBytes(unsigned int regAddr, unsigned int numBytes)
{
	I2caRegs.I2CSAR = m_devAddr;   		// Set slave address
	I2caRegs.I2CCNT = 1;            	// Set count to register address
	I2caRegs.I2CDXR = regAddr;     		// Send register address
	I2caRegs.I2CMDR.bit.TRX = 1;   		// Set to Transmit mode
	I2caRegs.I2CMDR.bit.MST = 1;   		// Set to Master mode
	I2caRegs.I2CMDR.bit.FREE = 1;  		// Run in FREE mode
	I2caRegs.I2CMDR.bit.STP = 0;    	// Stop when internal I2C_timeout becomes 0
	I2caRegs.I2CMDR.bit.STT = 1;    	// Send the start bit, transmission will follow
	while(I2caRegs.I2CSTR.bit.XRDY == 0 && m_I2C_timeout < 100000){m_I2C_timeout++;};     // Wait for buffer ready
	if (m_I2C_timeout > 0)
	{
		System_printf("TimeoutOccured\n");
		m_I2C_timeout = 0;
	}
	I2caRegs.I2CDXR = (m_devAddr<<1)+1;  			// Send device read (R)
	I2caRegs.I2CCNT = numBytes;                 // read numBytes from device
	I2caRegs.I2CMDR.bit.TRX = 0;                // Set to Receive mode
	I2caRegs.I2CMDR.bit.MST = 1;                // Set to Master mode
	I2caRegs.I2CMDR.bit.FREE = 1;               // Run in FREE mode
	I2caRegs.I2CMDR.bit.STP = 1;                // Stop when internal I2C_timeout becomes 0
	I2caRegs.I2CMDR.bit.STT = 1;                // Repeated start, Reception will follow
	if (numBytes > 14) numBytes = 14;
	for(unsigned int i = 0; i < numBytes; i++)
	   {
		   while(I2caRegs.I2CSTR.bit.RRDY == 0 && m_I2C_timeout < 100000)
		   {
			   m_I2C_timeout++;
		   }     //I2CDRR not ready to read?
		   m_I2C_buffer[i] = I2caRegs.I2CDRR;
	   }

//	DELAY_US(50);
	for (int a = 0; a < 10000; a++);

	m_I2C_timeout = 0;
}

void I2C_Device::readByte(unsigned int regAddr)
{
	readBytes(regAddr, 1);
}




