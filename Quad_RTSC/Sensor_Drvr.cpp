/*
 * MPU_9150.cpp
 *
 *  Created on: Jan 31, 2015
 *      Author: Chris
 */
#include "header.h"
#include "Sensor_Drvr.hpp"

Sensor_Drvr::Sensor_Drvr(void)
{
	m_mpu9150_i2c = new I2C_Device(MPU9150_ADDR);
	m_magno_i2c = new I2C_Device(0x0C);
	m_alti_i2c = new I2C_Device(0x60);
	zeroDatagram();
	Sensor_Setup();
	Sensor_testConnect();		// Check sensor communication on I2C
}

Sensor_Drvr::~Sensor_Drvr(void)
{
	delete m_mpu9150_i2c;
	delete m_magno_i2c;
	delete m_alti_i2c;
}

void Sensor_Drvr::Sensor_Setup(void) {

	//// Set up Clock Source and Gyro/Accel Ranges

	//	DLPF_CFG	Accelerometer (Fs = 1kHz)		Gyroscope
	//				Bandwidth (Hz)	Delay (ms)		Bandwidth (Hz)	Delay (ms)	Fs (kHz)
	//	0			260				0				256				0.98		8
	//	1			184				2.0				188				1.9			1
	//	2			94				3.0				98				2.8			1
	//	3			44				4.9				42				4.8			1
	//	4			21				8.5				20				8.3			1
	//	5			10				13.8			10				13.4		1
	//	6			5				19.0			5				18.6		1
	//	7			RESERVED		RESERVED		RESERVED		RESERVED	8

	/// Accel range
	//	AFS_SEL			Full Scale Range		LSB Sensitivity
	//	0				±2g						16384 LSB/mg
	//	1				±4g						8192 LSB/mg
	//	2				±8g						4096 LSB/mg
	//	3				±16g					2048 LSB/mg

	//	ACCEL_HPF		Filter Mode		Cut-off Frequency
	//	0				Reset			None
	//	1				On				5Hz
	//	2				On				2.5Hz
	//	3				On				1.25Hz
	//	4				On				0.63Hz
	//	7				Hold			None

	/// Gyro range
	//	FS_SEL		Full Scale Range		LSB Sensitivity
	//	0			± 250 °/s				131 LSB/°/s
	//	1			± 500 °/s				65.5 LSB/°/s
	//	2			± 1000 °/s				32.8 LSB/°/s
	//	3			± 2000 °/s				16.4 LSB/°/s

	m_mpu9150_i2c->writeByte(MPU9150_PWR_MGMT_1, 0x01); // Set clock source and clear sleep bit
	m_mpu9150_i2c->writeByte(MPU9150_CONFIG, 0x00);// Enable DLPF at above settings (No lowpass)
//	m_mpu9150_i2c->writeByte(MPU9150_SMPLRT_DIV, 0x09); // Set the clock rate to 100 Hz : Sample Rate = Gyroscope Output Rate(1kHz) / (1 + SMPLRT_DIV)
	m_mpu9150_i2c->writeByte(MPU9150_SMPLRT_DIV, 0x07); // Set the clock rate to 1000 Hz
//	m_mpu9150_i2c->writeByte(MPU9150_SMPLRT_DIV, 0x05); // Set the clock rate to 200 Hz
	m_mpu9150_i2c->writeByte(MPU9150_GYRO_CONFIG, 0x00);	// Set Full Scale Gyro Range at above settings
	m_mpu9150_i2c->writeByte(MPU9150_ACCEL_CONFIG, 0x00);// Set Full Scale Accel Range plus Accel HPF at above settings (No highpass)
	m_mpu9150_i2c->writeByte(MPU9150_INT_ENABLE, 0xE1);// Enable Free Fall, Motion detect, Zero Motion, and Data Rdy Interrupts E1
	m_mpu9150_i2c->writeByte(MPU9150_I2C_MST_CTRL, 0x00); // Disable Slave FIFO writes
	m_mpu9150_i2c->writeByte(MPU9150_INT_PIN_CFG, 0x02);	// Place MPU into Pass-Through mode to access Compass
	m_mpu9150_i2c->writeByte(MPU9150_USER_CTRL, 0x00);


	//// end Accel

	//// Mag Setup

	m_magno_i2c->writeByte(0x0A, 0x01);		// Single measurement mode

	//// end Mag

	//// Altimeter Setup

	m_alti_i2c->writeByte(0x26, 0xB8); 		// Set to Altimeter with an OSR = 128
	m_alti_i2c->writeByte(0x13, 0x07); 		// Enable Data Flags in PT_DATA_CFG
	m_alti_i2c->writeByte(0x26, 0xB9);		// Set sensor to Active

	//// end Altimeter

}
// NOTE: Should be called from a SWI
void Sensor_Drvr::Sensor_Accel(void) {

	Uint8 accel_dataRdy = 0;
	Uint8 interrupts = 0;

	// Check if Data Ready bit is enabled
	m_mpu9150_i2c->readByte(MPU9150_INT_STATUS);
	interrupts = m_mpu9150_i2c->m_I2C_buffer[0];
	accel_dataRdy = (interrupts && 0x01);

	if (accel_dataRdy != 0) {
		// Single Measurement Mode
//		dtTimer = ((float) (0xFFFFFFFF - CpuTimer0Regs.TIM.all)) / 90000000;// Measure the time between reads
		m_mpu9150_i2c->readBytes(MPU9150_ACCEL_XOUT_H, 14);

//		// Restart the timer register
//		CpuTimer0Regs.TCR.bit.TSS = 1;	// Stop timer
//		CpuTimer0Regs.TCR.bit.TRB = 1;	// Reload timer
//		CpuTimer0Regs.TCR.bit.TSS = 0;	// Restart timer

		// Store the raw acceleration values
		m_sensor_buffer[0] = (m_mpu9150_i2c->m_I2C_buffer[0] << 8 | m_mpu9150_i2c->m_I2C_buffer[1]);
		m_sensor_buffer[1] = (m_mpu9150_i2c->m_I2C_buffer[2] << 8 | m_mpu9150_i2c->m_I2C_buffer[3]);
		m_sensor_buffer[2] = (m_mpu9150_i2c->m_I2C_buffer[4] << 8 | m_mpu9150_i2c->m_I2C_buffer[5]);

		// Temperature in degrees C = (TEMP_OUT Register Value as a signed quantity)/340 + 35
		m_sensor_buffer[3] = (m_mpu9150_i2c->m_I2C_buffer[6] << 8 | m_mpu9150_i2c->m_I2C_buffer[7]);

		// Store the raw gyro values
		m_sensor_buffer[4] = (m_mpu9150_i2c->m_I2C_buffer[8] << 8 | m_mpu9150_i2c->m_I2C_buffer[9]);
		m_sensor_buffer[5] = (m_mpu9150_i2c->m_I2C_buffer[10] << 8 | m_mpu9150_i2c->m_I2C_buffer[11]);
		m_sensor_buffer[6] = (m_mpu9150_i2c->m_I2C_buffer[12] << 8 | m_mpu9150_i2c->m_I2C_buffer[13]);

		// Keep track of how many readings
//		m_sensor_buffer[7] = *(int *) dataGram.index + 1;

		// Store the sensor values as formatted floats
		dataGram.sensorData.Ax = *(int *) &m_sensor_buffer[0];
		dataGram.sensorData.Ax = (*(float *) &dataGram.sensorData.Ax) / ACCEL_SENS0;
		dataGram.sensorData.Ay = *(int *) &m_sensor_buffer[1];
		dataGram.sensorData.Ay = (*(float *) &dataGram.sensorData.Ay) / ACCEL_SENS0;
		dataGram.sensorData.Az = *(int *) &m_sensor_buffer[2];
		dataGram.sensorData.Az = (*(float *) &dataGram.sensorData.Az) / ACCEL_SENS0;

		dataGram.sensorData.T[0] = *(int *) &m_sensor_buffer[3];
		dataGram.sensorData.T[0] = ((*(float *) &dataGram.sensorData.T[0] / 340) + 35) * 1.8 + 32;// in degrees F

		dataGram.sensorData.Gx = *(int *) &m_sensor_buffer[4];
		dataGram.sensorData.Gx = *(float *) &dataGram.sensorData.Gx / GYRO_SENS0;
		dataGram.sensorData.Gy = *(int *) &m_sensor_buffer[5] / GYRO_SENS0;
		dataGram.sensorData.Gy = *(float *) &dataGram.sensorData.Gy / GYRO_SENS0;
		dataGram.sensorData.Gz = *(int *) &m_sensor_buffer[6] / GYRO_SENS0;
		dataGram.sensorData.Gz = *(float *) &dataGram.sensorData.Gz / GYRO_SENS0;

		dataGram.dT = dtTimer;
		dataGram.time = dataGram.time + dtTimer;
	}
} // end of Sensor_Accel
// NOTE: Should be called from a SWI
void Sensor_Drvr::Sensor_Mag(void) {
	// Single Measurement Mode

	float norm;

	// Read data ready register bit to check if new data available
	m_magno_i2c->readByte(0x02);

	if (m_magno_i2c->m_I2C_buffer[0] == 0x01) {
		// Read the raw values.
		m_magno_i2c->readBytes(0x03, 6);

		// Store the raw magnetometer values
		m_mag_buffer[0] = (m_magno_i2c->m_I2C_buffer[1] << 8 | m_magno_i2c->m_I2C_buffer[0]);
		m_mag_buffer[1] = (m_magno_i2c->m_I2C_buffer[3] << 8 | m_magno_i2c->m_I2C_buffer[2]);
		m_mag_buffer[2] = (m_magno_i2c->m_I2C_buffer[5] << 8 | m_magno_i2c->m_I2C_buffer[4]);

		// Repeat single measurement mode
		m_magno_i2c->writeByte(0x0A, 0x01);

		// Check if data is bad from overflow, if not then store as float data
		m_magno_i2c->readByte(0x09);

		if (m_magno_i2c->m_I2C_buffer[0] != 0x40) {
			dataGram.sensorData.Mx = *(int *) &m_mag_buffer[0];
			dataGram.sensorData.Mx = *(float *) &dataGram.sensorData.Mx * 0.3;

			dataGram.sensorData.My = *(int *) &m_mag_buffer[1];
			dataGram.sensorData.My = *(float *) &dataGram.sensorData.My * 0.3;

			dataGram.sensorData.Mz = *(int *) &m_mag_buffer[2];
			dataGram.sensorData.Mz = *(float *) &dataGram.sensorData.Mz * 0.3;

			// Normalize data
			norm = sqrt(dataGram.sensorData.Mx*dataGram.sensorData.Mx + dataGram.sensorData.My*dataGram.sensorData.My + dataGram.sensorData.Mz*dataGram.sensorData.Mz);
			dataGram.sensorData.Mx = dataGram.sensorData.Mx/norm;
			dataGram.sensorData.Mx = dataGram.sensorData.My/norm;
			dataGram.sensorData.Mx = dataGram.sensorData.Mz/norm;
		}

	}

} // end of Sensor_Mag
// NOTE: Should be called from a SWI
void Sensor_Drvr::Sensor_Altimeter(void) {
	unsigned int temp = 0;
	float temp1 = 0;
	unsigned int temp2 = 0;
	float temp3 = 0;

	m_alti_i2c->readByte(0x00);
	if (m_alti_i2c->m_I2C_buffer[0] & 0x08) {
		/* Read OUT_P 3 bytes and OUT_T 3 bytes, the LSB of each is the fractional part */
		/* This also clears the Data Rdy Interrupt */
		m_alti_i2c->readBytes(0x01, 5);

		// Store Altitude data; fractional part is LSB
		m_alt_buffer[0] = (m_alti_i2c->m_I2C_buffer[0] << 8 | m_alti_i2c->m_I2C_buffer[1]);
		temp = m_alti_i2c->m_I2C_buffer[2] >> 4;
		temp1 = (float) temp;

		// Store Temp data; fractional part is LSB
		m_alt_buffer[1] = m_alti_i2c->m_I2C_buffer[3];
		temp2 = m_alti_i2c->m_I2C_buffer[4] >> 4;
		temp3 = (float) temp2;

		dataGram.sensorData.A = *(int *) &m_alt_buffer[0];
		dataGram.sensorData.A = *(float *) &dataGram.sensorData.A + (temp1 / 16);

		dataGram.sensorData.T[1] = *(int *) &m_alt_buffer[1];
		dataGram.sensorData.T[1] = (*(float *) &dataGram.sensorData.T[1] + (temp3 / 16)) * 1.8 + 32; // Convert to deg F by (deg C)*1.8 + 32

	}

}

void Sensor_Drvr::Sensor_testConnect(void) {

	// Accelerometer
	m_mpu9150_i2c->readByte(MPU9150_WHO_AM_I);
	if (m_mpu9150_i2c->m_I2C_buffer[0] == MPU9150_ADDR)
		sensors_connected[0] = 1;

	// Magnetometer
	m_magno_i2c->readByte(0x00);
	if (m_magno_i2c->m_I2C_buffer[0] == 0x48)
		sensors_connected[1] = 1;

	// Altimeter
	m_alti_i2c->readByte(0x0C);
	if (m_alti_i2c->m_I2C_buffer[0] == 0xC4)
		sensors_connected[2] = 1;
}

Datagram Sensor_Drvr::GetSensorData(void) {

	// Measure the time between reads
	dtTimer = ((float) (0xFFFFFFFF - CpuTimer0Regs.TIM.all)) / 90000000;

	// Get data
	Sensor_Accel();	Sensor_Altimeter();	Sensor_Mag();
	dataGram.index = dataGram.index + 1;

	// Restart the timer register
	CpuTimer0Regs.TCR.bit.TSS = 1;	// Stop timer
	CpuTimer0Regs.TCR.bit.TRB = 1;	// Reload timer
	CpuTimer0Regs.TCR.bit.TSS = 0;	// Restart timer

	return dataGram;
}

void Sensor_Drvr::zeroDatagram(void){

	dataGram.sensorData.Ax = 0;	dataGram.sensorData.Ay = 0;	dataGram.sensorData.Az = 0;
	dataGram.sensorData.Gx = 0;	dataGram.sensorData.Gy = 0;	dataGram.sensorData.Gz = 0;
	dataGram.sensorData.Mx = 0;	dataGram.sensorData.My = 0;	dataGram.sensorData.Mz = 0;
	dataGram.sensorData.A = 0;
	dataGram.sensorData.T[0] = 0;		dataGram.sensorData.T[1] = 0;
	dataGram.index = 0;	    dataGram.time = 0;	dataGram.dT = 0;
	dataGram.roll = 0;		dataGram.pitch = 0;	dataGram.yaw = 0;
//	dataGram.r_a[0] = 0;	dataGram.r_a[1] = 0;	dataGram.r_a[2] = 0;
//	dataGram.r_g[0] = 0;	dataGram.r_g[1] = 0;	dataGram.r_g[2] = 0;
//	dataGram.mu[0] = 0;	dataGram.mu[1] = 0;	dataGram.mu[2] = 0;
//	dataGram.q[0] = 0;   dataGram.q[1] = 0;	dataGram.q[2] = 0;


	dataGram.battery = 0;

	for(int i=0;i<4;i++){
//		dataGram.Kp[i] = 0;
//		dataGram.Ki[i] = 0;
//		dataGram.Kd[i] = 0;

		dataGram.rateKp[i] = 0.05;
		dataGram.rateKi[i] = 0.10;

		dataGram.attitudeKp[i] = 0.05;
		dataGram.attitudeKi[i] = 0.10;
	}

	// Roll
//	dataGram.Kp[0] = .05;
//	dataGram.Ki[0] = .10;
//	dataGram.Kd[0] = .05;

	// Pitch
//	dataGram.Kp[1] = .05;
//	dataGram.Ki[1] = .10;
//	dataGram.Kd[1] = .05;

}


