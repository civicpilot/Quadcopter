/*
 * QuadProcessor.cpp
 *
 *  Created on: Feb 1, 2015
 *      Author: Chris
 */

#include "header.h"
#include "QuadProcessor.hpp"

#define ALPHA 0.98
#define ERROR_SUM_MAX			.25

Quad_Processor::Quad_Processor() {
	// TODO Auto-generated constructor stub
	for (int i = 0; i < sizeof(m_pidCommands); i++)
		m_pidCommands[i] = 0;

	m_cmdTransform = new Command_Transform();
	m_lastDataGram.roll = 0;
	m_lastDataGram.pitch = 0;
	m_lastDataGram.yaw = 0;
}

Quad_Processor::~Quad_Processor() {
	// TODO Auto-generated destructor stub
}

void Quad_Processor::Process_Data(Datagram data) {
	// MotionCmds will be coming from user input.
	float motionCmds[4] = {250, 0, 0, 0};
	m_dataGram = data;
	CompFilter();
	CollectData(motionCmds);
	m_cmdTransform->CommandMixer(motionCmds, m_pidCommands, 0);
	return;
}

// Should be called from a TSK
float Quad_Processor::PID2(float desired, float current_pos, float* last_pos, float* error_sum, int type)
{
	float error = 0;
	float command = 0;

	error = desired - current_pos;

	// Limit the Integral wind-up
	if ( fabs(*error_sum) > ERROR_SUM_MAX )
		*error_sum = ERROR_SUM_MAX;
	else
		*error_sum += error * m_dataGram.dT/2;

	// Form: cmd = Kp*error + Ki*errorIntegral + Kd*errorDerivative
	command = 	( m_dataGram.rateKp[type] * error )
			  + ( m_dataGram.rateKp[type] * (*error_sum) )
			  + ( m_dataGram.rateKp[type] * (current_pos - *last_pos) / m_dataGram.dT );

	*last_pos = current_pos;

	if (command > 1)
		return 1;
	else if (command < -1)
		return -1;
	else
		return command;
}
// TODO Need to convert these to handle an array.
// Should be called from a TSK
float Quad_Processor::RatePI(float desired, float current_pos, float* last_pos, float* error_sum, int type)
{
	float error = 0;
	float command = 0;

	error = desired - current_pos;

	// Limit the Integral wind-up
	if ( fabs(*error_sum) > ERROR_SUM_MAX )
		*error_sum = ERROR_SUM_MAX;
	else
		*error_sum += error * m_dataGram.dT;

	// Form: cmd = Kp*error + Ki*errorIntegral + Kd*errorDerivative
	command = 	( m_dataGram.rateKp[type] * error )
			  + ( m_dataGram.rateKi[type] * (*error_sum) )
			  + ( m_dataGram.rateKd[type] * (current_pos - *last_pos) / m_dataGram.dT );

	*last_pos = current_pos;

//	if (command > 1)
//		return 1;
//	else if (command < -1)
//		return -1;
//	else
		return command;
}
// TODO Need to convert these to handle an array.
// Should be called from a TSK
float Quad_Processor::AttitudePI(float desired, float current_pos, float* last_pos, float* error_sum, int type)
{
	float error = 0;
	float command = 0;

	error = desired - current_pos;

	// Limit the Integral wind-up
	if ( fabs(*error_sum) > ERROR_SUM_MAX )
		*error_sum = ERROR_SUM_MAX;
	else
		*error_sum += error * m_dataGram.dT;

	// Form: cmd = Kp*error + Ki*errorIntegral + Kd*errorDerivative
	command = 	( m_dataGram.attitudeKp[type] * error )
			  + ( m_dataGram.attitudeKi[type] * (*error_sum) )
			  + ( m_dataGram.attitudeKd[type] * (current_pos - *last_pos) / m_dataGram.dT );

	*last_pos = current_pos;
//
//	if (command > 1)
//		return 1;
//	else if (command < -1)
//		return -1;
//	else
		return command;
}
// Should be called from a TSK
void Quad_Processor::CompFilter(void)
{
	int oneOff = 0;
	if (oneOff == 0)
	{
		m_dataGram.roll = m_dataGram.sensorData.Ax;
		m_dataGram.pitch = m_dataGram.sensorData.Ay;
		m_dataGram.yaw = m_dataGram.sensorData.Az;
		oneOff++;
	}

	m_dataGram.roll = (1 - ALPHA)*(m_dataGram.roll + (m_dataGram.sensorData.Gx)*m_dataGram.dT) + (ALPHA)*(m_dataGram.sensorData.Ax);
	m_dataGram.pitch = (1 - ALPHA)*(m_dataGram.pitch + (m_dataGram.sensorData.Gy)*m_dataGram.dT) + (ALPHA)*(m_dataGram.sensorData.Ay);
	m_dataGram.yaw = (1 - ALPHA)*(m_dataGram.yaw + (m_dataGram.sensorData.Gz)*m_dataGram.dT) + (ALPHA)*(m_dataGram.sensorData.Az);

//	m_dataGram.roll = (ALPHA)*(m_dataGram.roll + (m_dataGram.sensorData.Gx)*m_dataGram.dT) + (1 - ALPHA)*(m_dataGram.sensorData.Ax);
//	m_dataGram.pitch = (ALPHA)*(m_dataGram.pitch + (m_dataGram.sensorData.Gy)*m_dataGram.dT) + (1 - ALPHA)*(m_dataGram.sensorData.Ay);
//	m_dataGram.yaw = (ALPHA)*(m_dataGram.yaw + (m_dataGram.sensorData.Gz)*m_dataGram.dT) + (1 - ALPHA)*(m_dataGram.sensorData.Az);

//	m_dataGram.roll = (ALPHA)*(m_dataGram.sensorData.Gx*m_dataGram.dT) + (1-ALPHA)*(m_dataGram.sensorData.Ax);
//	m_dataGram.pitch = (ALPHA)*(m_dataGram.sensorData.Gy*m_dataGram.dT) + (1-ALPHA)*(m_dataGram.sensorData.Ay);
//	m_dataGram.yaw = (ALPHA)*(m_dataGram.sensorData.Gz*m_dataGram.dT) + (1-ALPHA)*(m_dataGram.sensorData.Az);

	m_lastDataGram.roll = m_dataGram.roll;
	m_lastDataGram.pitch = m_dataGram.pitch;
	m_lastDataGram.yaw = m_dataGram.yaw;
}
// Should be called from a TSK
void Quad_Processor::CollectData(float *motionCmds)
{
	float attitudeCmds[4] = {0, 0, 0, 0};

	// Outer PI Loop (Attitude)
	// Roll
	attitudeCmds[0] = AttitudePI(motionCmds[1], m_dataGram.roll, &m_sensorErrs.old_pos_roll, &m_sensorErrs.sum_error_roll,0);
	// Pitch
	attitudeCmds[1] = AttitudePI(motionCmds[2], m_dataGram.pitch, &m_sensorErrs.old_pos_pitch, &m_sensorErrs.sum_error_pitch,1);
	// Yaw
	attitudeCmds[2] = AttitudePI(motionCmds[3], m_dataGram.yaw, &m_sensorErrs.old_pos_yaw, &m_sensorErrs.sum_error_yaw,2);
	// Thrust
	attitudeCmds[3] = AttitudePI(motionCmds[0], m_dataGram.sensorData.A, &m_sensorErrs.old_pos_altitude, &m_sensorErrs.sum_error_altitude,3);

	// Inner PI loop (Rate)
	// Roll
	m_pidCommands[0] = RatePI(attitudeCmds[0], m_dataGram.sensorData.Gx, &m_sensorErrs.old_pos_Gx, &m_sensorErrs.sum_error_Gx,0);
	// Pitch
	m_pidCommands[1] = RatePI(attitudeCmds[1], m_dataGram.sensorData.Gy, &m_sensorErrs.old_pos_Gy, &m_sensorErrs.sum_error_Gy,1);
	// Yaw
	m_pidCommands[2] = RatePI(attitudeCmds[2], m_dataGram.sensorData.Gz, &m_sensorErrs.old_pos_Gz, &m_sensorErrs.sum_error_Gz,2);
	// Thrust
	m_pidCommands[3] = RatePI(attitudeCmds[3], m_dataGram.sensorData.A, &m_sensorErrs.old_pos_altitude, &m_sensorErrs.sum_error_altitude,3);

//	m_pidCommands[0] = PID2(motionCmds[1], m_dataGram.roll, &m_sensorErrs.old_pos_roll, &m_sensorErrs.sum_error_roll,0);
//	m_pidCommands[1] = PID2(motionCmds[2], m_dataGram.pitch, &m_sensorErrs.old_pos_pitch, &m_sensorErrs.sum_error_pitch,1);
//	m_pidCommands[2] = PID2(motionCmds[3], m_dataGram.yaw, &m_sensorErrs.old_pos_yaw, &m_sensorErrs.sum_error_yaw,2);
//	m_pidCommands[3] = PID2(motionCmds[0], m_dataGram.sensorData.A, &m_sensorErrs.old_pos_altitude, &m_sensorErrs.sum_error_altitude,3);

	// Limit Range of motion
//	if (fabs(m_dataGram.roll) > 0.60 || fabs(m_dataGram.pitch) > 0.60) {
//		m_pidCommands[0] = 0;
//		m_pidCommands[1] = 0;
//		m_pidCommands[2] = 0;
//		m_pidCommands[3] = 0;
//	}
}
