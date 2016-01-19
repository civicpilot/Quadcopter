/*
 * HUD_Interface.cpp
 *
 *  Created on: Feb 2, 2015
 *      Author: Chris
 */

#include "header.h"
#include "HUD_Interface.hpp"
#include "SPI_Device.hpp"

Uint16 testPackets[] = {0x7E, 0x00, 0x05, 0x08, 0x01, 0x50, 0x33, 0x30, 0x43};

HUD_Interface::HUD_Interface() {
	// TODO Auto-generated constructor stub
	m_xb_Device = new SPI_Device(&m_XB_txBuf);
}

HUD_Interface::~HUD_Interface() {
	// TODO Auto-generated destructor stub
}

// This should be called from a SWI
void HUD_Interface::parseInputData(float *motionCommands) // Reads one unsigned int and then processes it.
{
	static Uint32 streamState = 0x0001;
	static Uint32 stateCount = 0x0000;
	static Uint32 data = 0x00000000;
	static Uint32 indx = 0;
	static float temp = 0;

	Uint8 rdata = 0;
	Uint8 sdata = 0;

	if (!m_xb_Device)
		return;

	build_packet(m_xb_Device->m_XB_txBuffer, testPackets, sizeof(testPackets));

	if (SpibRegs.SPIFFTX.bit.TXFFST != 4) {
		sdata = ((Uint16) m_xb_Device->readByteSPI()) << 8;
//		sdata = 0x00;
		SpibRegs.SPITXBUF = sdata;
	}

	if (SpibRegs.SPIFFRX.bit.RXFFST != 0) { // && !m_xb_Device->m_XB_txBuffer->empty) {
		 rdata = SpibRegs.SPIRXBUF;

		switch (streamState) {

		case 0x0001: {
			streamState = 0x0002;

			switch (rdata) {

			case 0x0054: {
				indx = 0;
				break;
			}

			case 0x0052: {
				indx = 1;
				break;
			}

			case 0x0050: {
				indx = 2;
				break;
			}

			case 0x0059: {
				indx = 3;
				break;
			}

			default: {
				streamState = 0x0001;
				break;
			}
			}
			break;

		}

		case 0x0002: {
			data = (data | rdata) << 8;
			stateCount++;

			if (stateCount >= 8) {
				streamState = 0x0003;
				stateCount = 0;
			}
			break;

		}

		case 0x0003: {
			data = data | rdata;
			temp = *(float32 *) &data;
			if (temp <= 1.0f && temp >= -1.0f) {
				motionCommands[indx] = temp;
			}
			//      motionCommands[indx] = *(float32 *) &data;
			data = 0x00000000;
			streamState = 0x0001;
			break;

		}

		default: {
			streamState = 0x0001;
			break;

		}

		}
	}

//	m_xb_Device->Empty_SpiBuffer();
}
// NOTE: Should be called from a TSK
void HUD_Interface::build_packet(FrameBuffer *XB_txBuffer, Uint16 *sensor_data, Uint8 length)
 {

	int i, j;
	Uint8 high, low;
	Uint8 cs = 0x00;
	Uint8 hdr[15] = {0x7E, 0x00, 0x00, 0x20, 0x00, 0xC0, 0xA8, 0X01, 0xFF,
		0x26, 0x17, 0x26, 0x16, 0x00, 0x00};
	hdr[2] = 12 + length * 2;
	for (i = 0; i < 15; ++i) {
		m_xb_Device->writeByteSPI(hdr[i]);
		if (i > 2) {
			cs += hdr[i];
		}
	}
	for (j = 0; j < (length); ++j) {
		high = (Uint8) (sensor_data[j] >> 8);
		low = (Uint8) (sensor_data[j] & 0x00FF);
		m_xb_Device->writeByteSPI(high);
		m_xb_Device->writeByteSPI(low);
		cs += (high + low);
	}
	cs = 0xFF - cs;
	m_xb_Device->writeByteSPI(cs);
}


