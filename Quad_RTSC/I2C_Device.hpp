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
	I2C_Device(Uint8 devAddress);
	~I2C_Device();

	void I2CA_Init (void);
	void readByte (Uint8 regAddr);
	void readBytes (Uint8 regAddr, Uint8 numBytes);
	void writeByte (Uint8 regAddr, Uint8 msg);
	void writeBytes (Uint8 regAddr, Uint8 numBytes, Uint8 *msg);

	Uint8 m_I2C_buffer[14];

private:
	unsigned long m_I2C_timeout;
	unsigned int m_devAddr;
};



#endif /* I2C_DEVICE_HPP_ */
