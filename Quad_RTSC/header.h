#ifndef HEADER_H_
#define HEADER_H_

#include <xdc/std.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include "DSP28x_Project.h"     		// Device Headerfile and Examples Include File
#include "F2806x_GlobalPrototypes.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "float.h"

#define PERIOD		56250
#define SATURATION	56250
#define START_DUTYCYCLE 3200
#define ACCEL_SENS0 	16384
#define ACCEL_SENS1		8192
#define ACCEL_SENS2 	4096
#define ACCEL_SENS3	2048
#define GYRO_SENS0	131
#define GYRO_SENS1	65.5
#define GYRO_SENS2	32.8
#define GYRO_SENS3	16.4
#define MAG_SENS	0.3
#define nAverage	32
#define PI 			3.14156

//PID Constants
//#define KP 0.98
//#define KI 1
//#define KD 0.50

//// Structures
typedef struct{
	float old_pos_roll;
	float old_pos_pitch;
	float old_pos_yaw;
	float old_pos_altitude;
	float sum_error_roll;
	float sum_error_pitch;
	float sum_error_yaw;
	float sum_error_altitude;

	float old_pos_Gx;
	float old_pos_Gy;
	float old_pos_Gz;
	float sum_error_Gx;
	float sum_error_Gy;
	float sum_error_Gz;
}ERROR;



typedef struct {
	Uint8 buffer[50];
	unsigned int pos;
	unsigned int EOD;
	unsigned int cap;
	unsigned int empty;
} FrameBuffer;


typedef struct{
	float Ax;
	float Ay;
	float Az;
	float Gx;
	float Gy;
	float Gz;
	float Mx;
	float My;
	float Mz;
	float A;
	float T[2];

} Sensors;

typedef struct{
	float values[nAverage];
	float newValue;
	unsigned char oldestIndex;
	float currentTotal;
	float average;

} Averaging;

typedef union
{
  float f;
  unsigned short b[2];
} floatdata;

typedef struct{
	Sensors sensorData;
	float index;
	float dT;
	float time;
	float rateKp[4];
	float rateKi[4];
	float rateKd[4];
	float attitudeKp[4];
	float attitudeKi[4];
	float attitudeKd[4];
	double roll;
	double pitch;
	double yaw;
	double battery;
	double GxOffset;
	double GyOffset;
	double GzOffset;

} Datagram;

#endif /* HEADER_H_ */
