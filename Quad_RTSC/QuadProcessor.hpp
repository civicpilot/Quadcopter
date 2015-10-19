/*
 * QuadProcessor.hpp
 *
 *  Created on: Feb 1, 2015
 *      Author: Chris
 */

#ifndef QUADPROCESSOR_HPP_
#define QUADPROCESSOR_HPP_

#ifndef HEADER_H_
#include "header.h"
#endif

class Quad_Processor {
public:
	Quad_Processor();
	~Quad_Processor();

	// These need to be re-written to return arrays.
	float PID2(float desired, float current_pos, float* last_pos, float* error_sum, int type);
	float RatePI(float desired, float current_pos, float* last_pos, float* error_sum, int type);
	float AttitudePI(float desired, float current_pos, float* last_pos, float* error_sum, int type);
	void CompFilter(void);
	void CollectData(float* motionCmds, float* correctCmds);

private:
	ERROR m_sensorErrs;

public:
	Datagram m_dataGram;
};

#endif /* QUADPROCESSOR_HPP_ */
