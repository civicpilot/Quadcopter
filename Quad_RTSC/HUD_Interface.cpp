/*
 * HUD_Interface.cpp
 *
 *  Created on: Feb 2, 2015
 *      Author: Chris
 */

#include "header.h"
#include "HUD_Interface.hpp"
#include "SPI_Device.hpp"

Uint8 testPackets[] = {0x7E, 0x00, 0x05, 0x08, 0x01, 0x50, 0x33, 0x30, 0x43};

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
	static int i = 0;
	static unsigned int streamState = 0x0001;
	static unsigned int stateCount = 0x0000;
	static unsigned int data = 0x00000000;
	static unsigned int indx = 0;
	static float temp = 0;

	Uint8 rdata = 0;

	if (!m_xb_Device)
		return;

	for(i = 0; i<= sizeof(testPackets); i++)
		m_xb_Device->writeByteSPI(testPackets[i]);

//	if (SpibRegs.SPIFFTX.bit.TXFFST != 4) {
//		SpibRegs.SPITXBUF = ((Uint16) m_xb_Device->readByteSPI()) << 8;
//	}
//
	if (SpibRegs.SPIFFRX.bit.RXFFST != 0 && !m_xb_Device->m_XB_txBuffer->empty) {
		 rdata = SpibRegs.SPIRXBUF;

    if (rdata != 255)
    	int value = 0x00;

//		rdata = m_xb_Device->readByteSPI();

		switch (streamState)
		{
			case 0x0001: {
				streamState = 0x0002;

				switch (rdata) {

				case 0x0054: {	// ASCII T
					indx = 0;
					//isCommandComplete = 0;
					break;
				}

				case 0x0052: {	// ASCII P
					indx = 1;
					break;
				}

				case 0x0050: {	// ASCII R
					indx = 2;
					break;
				}

				case 0x0059: {	// ASCII Y
					indx = 3;
					break;
				}

				case 0x004B: {	// ASCII K
					indx = 4;
					break;
				}

				case 0x004C: {	// ASCII L
					indx = 5;
					break;
				}

				case 0x004D: {	// ASCII M
					indx = 6;
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

				if (stateCount >= 3) {
					streamState = 0x0003;
					stateCount = 0;
				}
				break;

			}

			case 0x0003: {
				data = data | rdata;
				temp = *(float *) &data;

				switch (indx) {
				case 0x0000: {	// Thrust
					if ( fabs(temp) <= 1 ) {
						motionCommands[indx] = temp;
					}
					break;
				}
				case 0x0001: {	// Pitch
					if ( fabs(temp) <= 1 ) {
						motionCommands[indx] = temp;
					}
					break;
				}

				case 0x0002: {	// Roll
					if ( fabs(temp) <= 1 ) {
						motionCommands[indx] = temp;
					}
					break;
				}
				case 0x0003: {	// Yaw
					if ( fabs(temp) <= 1 ) {
						motionCommands[indx] = temp;
					}
					break;
				}
		//		case 0x0004: {	// Kp
		//
		//				motionCommands[indx] = temp;
		//				sensorData.rateKp[0] = temp;
		//
		//			break;
		//		}
		//		case 0x0005: {	// Kd
		//
		//				motionCommands[indx] = temp;
		////				sensorData.rateKd[0] = temp;
		//			break;
		//		}
		//		case 0x0006: {	// Ki
		//
		//				motionCommands[indx] = temp;
		//				sensorData.rateKi[0] = temp;
		//			break;
		//		}
				default:
					break;

				}
				data = 0x00000000;
				streamState = 0x0001;
				break;

			}

			default:
			{
				streamState = 0x0001;
				break;

			}

		}
	}
	m_xb_Device->Empty_SpiBuffer();
}
// NOTE: Should be called from a TSK
void HUD_Interface::build_packet(unsigned int *data1, float *data2, unsigned int length, unsigned int type)
{

	int i, j,temp;
	unsigned char cs = 0x00;
	unsigned char hdr[15] = { 0x7E, 					// Start Delimiter
							  0x00, 0x00, 				// 1-2 are the packet length
							  0x20, 					// API frame identifier
							  0x00,						// Frame ID
							  0xC0, 0xA8, 0X01, 0x77,	// 5-8 are the Dest. IP: 192.168.1.255
							  0x26, 0x16, 				// 9-10 Source Port
							  0x26, 0x16, 				// 11-12 Destination Port
							  0x00, 					// For TCP: 0x01; for UDP: 0x00
							  0x00 };					// Transmit Options Bitfield; 1: Terminate after tx complete, 0: wait for TCP timeout

//	if (type == 0){
//		hdr[2] = 12 + length * 2;}
//	else if (type == 1){
//		hdr[2] = 12 + length * 2;}

	temp = 12 + length * 2;
	hdr[1] = temp >> 8;
	hdr[2] = temp;
	hdr[8] = 0x02;

	hdr[2] = 12 + length * 2;


	for (i = 0; i < 15; ++i) {
		m_xb_Device->writeByteSPI(hdr[i]);
		if (i > 2) {
			cs += hdr[i];
		}
	}

	for (j = 0; j < (length); ++j) {
		if (type == 0) {
			Uint8 high,low;
			high = (Uint8) (data1[j] >> 8);
			low = (Uint8) (data1[j] & 0x00FF);
			m_xb_Device->writeByteSPI(high);
			m_xb_Device->writeByteSPI(low);
			cs += (high + low);
		}
		else if (type == 1) {
			floatdata q;
			Uint8 byte1,byte2,byte3,byte4;

			q.f = data2[j];

			byte1 = (Uint8) (q.b[1] >> 8);
			m_xb_Device->writeByteSPI(byte1);
			byte2 = (Uint8) (q.b[1] & 0x00FF);
			m_xb_Device->writeByteSPI(byte2);
			byte3 = (Uint8) (q.b[0] >> 8);
			m_xb_Device->writeByteSPI(byte3);
			byte4 = (Uint8) (q.b[0] & 0x00FF);
			m_xb_Device->writeByteSPI(byte4);

			cs += (byte1 + byte2 + byte3 + byte4);
		}

	}

	// cs needs to only be 2 bytes when doing the below subtraction
	cs = (0xFF - ((cs << 8) >> 8));	// the checksum is the 0xFF - sum of bytes from offset 3 of the header until the end of the data
	m_xb_Device->writeByteSPI(cs);
	// Empty_txBuffer();

}


