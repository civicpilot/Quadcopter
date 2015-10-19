/*
 * I2C_Device.hpp
 *
 *  Created on: Jan 31, 2015
 *      Author: Chris
 */

#ifndef I2C_DEVICE_HPP_
#define I2C_DEVICE_HPP_

class I2C_Device
{
public:
	I2C_Device(unsigned int devAddress);
	~I2C_Device();

	void I2CA_Init (void);
	void readByte (unsigned int regAddr);
	void readBytes (unsigned int regAddr, unsigned int numBytes);
	void writeByte (unsigned int regAddr, unsigned int msg);
	void writeBytes (unsigned int regAddr, unsigned int numBytes, unsigned int *msg);

	unsigned short m_I2C_buffer[14];

private:
	unsigned long m_I2C_timeout;
	unsigned int m_devAddr;
};



#endif /* I2C_DEVICE_HPP_ */
