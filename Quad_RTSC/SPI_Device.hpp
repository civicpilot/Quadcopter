/*
 * SPI_Device.hpp
 *
 *  Created on: Feb 1, 2015
 *      Author: Chris
 *
 *  This controls the XBee WIFI module for this device.
 */

#include "header.h"

#ifndef SPI_DEVICE_HPP_
#define SPI_DEVICE_HPP_

class SPI_Device
{
public:
	SPI_Device(FrameBuffer *XB_txBuffer);
	~SPI_Device(){};

	void spi_init(void);
	Uint8 readByteSPI(void);
	void writeByteSPI(Uint8 txdata);
	void Empty_SpiBuffer(void);

	FrameBuffer *m_XB_txBuffer;
};


#endif /* SPI_DEVICE_HPP_ */
