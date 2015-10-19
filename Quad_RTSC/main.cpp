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
float motionCommands[4];
Datagram sensor_data;

LED_Driver* ledDriver;


void Initialize(void);
extern "C"
{
	Void parseInputData(UArg arg);
	Void getSensorData(UArg arg);
}

/*
 *  ======== taskFxn ========
 */
extern "C"
{
Void taskFxn(UArg a0, UArg a1)
{
    System_printf("enter taskFxn()\n");

	Task_sleep(10);
	ledDriver->flashLED();

    System_printf("exit taskFxn()\n");
}
}

Int main(void)
{
	Initialize();
	System_printf("System Initialized\n");
//	hud_Interface = new HUD_Interface();
	sensor_Drvr = new Sensor_Drvr();
	ledDriver = new LED_Driver();

    BIOS_start();    /* does not return */
    return(0);
}

//// Functions
void Initialize(void)
{

#ifdef _FLASH
	memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);
#endif

//	Empty_txBuffer();

	// Step 1. Initialize System Control:
	// PLL, WatchDog, enable Peripheral Clocks
	InitSysCtrl();

	//Initialize the PWMS
	InitEPwm3Gpio();
	InitEPwm4Gpio();
	InitEPwm7Gpio();
	InitEPwm8Gpio();

	InitSpibGpio();

	// Interferring with RTOS
//	InitAdc();

	//RTOS Should handle this
	InitCpuTimers();

	// Step 3. Clear all interrupts and initialize PIE vector table:
	// Disable CPU interrupts
	DINT; //REMOVED FOR RTOS

	// Initialize PIE control registers to their default state.
	// The default state is all PIE interrupts disabled and flags
	// are cleared.
	InitPieCtrl();

	// Disable CPU interrupts and clear all CPU interrupt flags:
	IER = 0x0000;
	IFR = 0x0000;

	// Initialize the PIE vector table with pointers to the shell Interrupt
	// Service Routines (ISR).
	// This will populate the entire table, even if the interrupt
	// is not used in this example.  This is useful for debug purposes.
	InitPieVectTable();

//	// Setup the PWM for the ESCs
//	PWMsetup();
//
//	// Step 4. Initialize all the Device Peripherals:
//	spi_init();					// Initialize SPI
//	GPIO_Setup_LED();			// Initialize GPIO for LED functionality
//	I2CA_Init();				// Initialize all the I2C Device Peripherals

	CpuTimer0Regs.TCR.bit.FREE = 1;	// Enable free count mode on the timer

//	Sensor_Setup();				// Setup the sensors
//	Sensor_testConnect();		// Check sensor communication on I2C


	// Configure ADC
		EALLOW;
	    AdcRegs.ADCCTL2.bit.ADCNONOVERLAP = 1;	// Enable non-overlap mode
		AdcRegs.ADCCTL1.bit.INTPULSEPOS	= 1;	// ADCINT1 trips after AdcResults latch
		AdcRegs.INTSEL1N2.bit.INT1E     = 1;	// Enabled ADCINT1
		AdcRegs.INTSEL1N2.bit.INT1CONT  = 0;	// Disable ADCINT1 Continuous mode
	    AdcRegs.INTSEL1N2.bit.INT1SEL 	= 1;    // setup EOC1 to trigger ADCINT1 to fire
	    AdcRegs.ADCSOC0CTL.bit.CHSEL 	= 4;    // set SOC0 channel select to ADCINA4
	    AdcRegs.ADCSOC1CTL.bit.CHSEL 	= 2;    // set SOC1 channel select to ADCINA2
	    AdcRegs.ADCSOC0CTL.bit.TRIGSEL 	= 5;    // set SOC0 start trigger on EPWM1A, due to round-robin SOC0 converts first then SOC1
	    AdcRegs.ADCSOC1CTL.bit.TRIGSEL 	= 5;    // set SOC1 start trigger on EPWM1A, due to round-robin SOC0 converts first then SOC1
		AdcRegs.ADCSOC0CTL.bit.ACQPS 	= 6;	// set SOC0 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
		AdcRegs.ADCSOC1CTL.bit.ACQPS 	= 6;	// set SOC1 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
		EDIS;


} // end Initialize

//Void parseInputData(UArg arg)
//{
//	HUD_Interface *hud_interface = (HUD_Interface *) arg;
//	hud_interface->parseInputData(motionCommands);
//}

Void getSensorData(UArg arg)
{
//	Sensor_Drvr *sensor_drvr = (Sensor_Drvr *) arg;
	ledDriver->flashLED();
}
