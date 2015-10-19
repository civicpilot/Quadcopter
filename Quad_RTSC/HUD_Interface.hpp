/*
 * HUD_Interface.hpp
 *
 *  Created on: Feb 2, 2015
 *      Author: Chris
 */

#ifndef HUD_INTERFACE_HPP_
#define HUD_INTERFACE_HPP_

#include "SPI_Device.hpp"
#ifndef HEADER_H_
#include "header.h"
#endif

class HUD_Interface {
public:
	HUD_Interface();
	~HUD_Interface();

	void parseInputData(float *motionCommands);
	void build_packet(unsigned int *data1, float *data2, unsigned int length, unsigned int type);

public:
	FrameBuffer m_XB_txBuf;
	SPI_Device *m_xb_Device;
};

#endif /* HUD_INTERFACE_HPP_ */
