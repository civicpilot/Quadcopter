/*
 * main.cpp
 */
#include "header.h"
#include <ti/sysbios/knl/Task.h>

#include "HUD_Interface.hpp"
#include "QuadProcessor.hpp"
#include "Command_Transform.hpp"
#include "Sensor_Drvr.hpp"

// Global objects for interrupt routines
HUD_Interface* hud_Interface;
Sensor_Drvr* sensor_Drvr;
Quad_Processor* quadProc;
float motionCommands[4];
Datagram sensor_data;

LED_Driver* ledDriver;


void Initialize(void);
extern "C"
{
	void parseInputData(UArg arg);
	void getSensorData(UArg arg);
	void taskFxn(UArg a0, UArg a1);
}

/*
 *  ======== taskFxn ========
 */
void taskFxn(UArg a0, UArg a1)
{

	System_printf("enter taskFxn()\n");
	for(;;)
	{
		Task_sleep(10);
	}

//    System_printf("exit taskFxn()\n");
}

Int main(void)
{
	System_printf("System Initialized\n");
//	hud_Interface = new HUD_Interface();
	sensor_Drvr = new Sensor_Drvr();
//	ledDriver = new LED_Driver();
	quadProc = new Quad_Processor();

    BIOS_start();    /* does not return */
    return(0);
}

//Void parseInputData(UArg arg)
//{
//	HUD_Interface *hud_interface = (HUD_Interface *) arg;
//	hud_interface->parseInputData(motionCommands);
//}

void getSensorData(UArg arg)
{
	sensor_data = sensor_Drvr->GetSensorData();
	quadProc->Process_Data(sensor_data);
//	ledDriver->flashLED();
	return;
}
