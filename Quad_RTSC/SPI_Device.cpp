/*
 * SPI_Device.cpp
 *
 *  Created on: Feb 1, 2015
 *      Author: Chris
 */

#include "header.h"
#include "SPI_Device.hpp"

#define BUFFERSIZE	500

SPI_Device::SPI_Device(FrameBuffer *XB_txBuf)
{
	spi_init();
	Empty_SpiBuffer();
	m_XB_txBuffer = XB_txBuf;
}

void SPI_Device::spi_init(void) {

	// Initialize SPI FIFO registers
	SpibRegs.SPIFFTX.all = 0xE040;
	SpibRegs.SPIFFRX.all = 0x2044;
	SpibRegs.SPIFFCT.all = 0x0;

	SpibRegs.SPICCR.all = 0x0007; 	// Reset on, rising edge, 8-bit char bits
	SpibRegs.SPICTL.all = 0x000E; // Enable master mode, delayed phase, enable talk, and SPI int disabled.

	SpibRegs.SPIBRR = 0x000F;			// SPI clock at 3.46MHz
	SpibRegs.SPICCR.all = 0x0087; // Relinquish SPI from Reset, Loopback Disabled
	SpibRegs.SPIPRI.bit.FREE = 1; 		// Set so breakpoints don't disturb TX

}

void SPI_Device::writeByteSPI(unsigned int txdata) {

	if (!m_XB_txBuffer)
		return;
	if (m_XB_txBuffer->EOD != m_XB_txBuffer->cap) {
		m_XB_txBuffer->buffer[m_XB_txBuffer->EOD] = txdata;
		++m_XB_txBuffer->EOD;
		m_XB_txBuffer->empty = 0;
	}

}

unsigned int SPI_Device::readByteSPI(void) {

	unsigned int temp = 0x00;

	if (!m_XB_txBuffer)
		return 0x00;
	if (!m_XB_txBuffer->empty) {
		temp = m_XB_txBuffer->buffer[m_XB_txBuffer->pos];
		++m_XB_txBuffer->pos;
		if (m_XB_txBuffer->EOD == m_XB_txBuffer->pos) {
			m_XB_txBuffer->empty = 1;
			m_XB_txBuffer->pos = 0;
			m_XB_txBuffer->EOD = 0;
		}
		return temp;
	} else {
		return 0x00;
	}

}


void SPI_Device::Empty_SpiBuffer(void) {

	// Resets the XB buffer to 'empty'
	if (!m_XB_txBuffer)
		return;
	m_XB_txBuffer->cap = BUFFERSIZE-1;
	m_XB_txBuffer->pos = 0;
	m_XB_txBuffer->EOD = 0;
	m_XB_txBuffer->empty = 1;

}


