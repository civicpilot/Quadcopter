/*
 * MPU_9150.hpp
 *
 *  Created on: Jan 31, 2015
 *      Author: Chris
 */

#ifndef MPU_9150_HPP_
#define MPU_9150_HPP_

#include "I2C_Device.hpp"
#include "MPU9150_defines.h"

#ifndef HEADER_H_
#include "header.h"
#endif


class Sensor_Drvr
{
public:
	Sensor_Drvr();
	~Sensor_Drvr();

	void 	Sensor_Setup		(void);
	void 	Sensor_testConnect	(void);
	Datagram    GetSensorData   (void);
	void    zeroDatagram        (void);
	void 	Sensor_Accel		(void);
	void 	Sensor_Mag			(void);
	void 	Sensor_Altimeter	(void);

	I2C_Device *m_mpu9150_i2c;
	I2C_Device *m_magno_i2c;
	I2C_Device *m_alti_i2c;

	unsigned int m_sensor_buffer[8];
	unsigned int m_alt_buffer[2];
	unsigned int m_mag_buffer[3];
	float 		 dtTimer;

public:
	Datagram dataGram;
	int sensors_connected[3];
};

#endif /* MPU_9150_HPP_ */
