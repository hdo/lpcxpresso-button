//*****************************************************************************
//   +--+       
//   | ++----+   
//   +-++    |  
//     |     |  
//   +-+--+  |   
//   | +--+--+  
//   +----+    Copyright (c) 2010-11 Code Red Technologies Ltd.
//
// systick_main.c provides an example of how to use the SysTick timer
// built in to the Cortex-M3 core of the LPC1768 MCU on the LPCXpresso1768
// processor board. The example will cause the state of "LED2" (P0_22)
// to toggle every second.
//
//
// Software License Agreement
// 
// The software is owned by Code Red Technologies and/or its suppliers, and is 
// protected under applicable copyright laws.  All rights are reserved.  Any 
// use in violation of the foregoing restrictions may subject the user to criminal 
// sanctions under applicable laws, as well as to civil liability for the breach 
// of the terms and conditions of this license.
// 
// THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
// OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
// USE OF THIS SOFTWARE FOR COMMERCIAL DEVELOPMENT AND/OR EDUCATION IS SUBJECT
// TO A CURRENT END USER LICENSE AGREEMENT (COMMERCIAL OR EDUCATIONAL) WITH
// CODE RED TECHNOLOGIES LTD. 

// CMSIS headers required for setting up SysTick Timer
#include "LPC17xx.h"
#include "eint3.h"

#include <cr_section_macros.h>
#include <NXP/crp.h>

// Variable to store CRP value in. Will be placed automatically
// by the linker when "Enable Code Read Protect" selected.
// See crp.h header for more information
__CRP const unsigned int CRP_WORD = CRP_NO_CRP ;

#include "leds.h"
#include "uart.h"
#include "logger.h"
#include "s0_input.h"

volatile uint32_t msTicks; // counter for 1ms SysTicks
extern volatile unsigned int eint3_count;


// ****************
//  SysTick_Handler - just increment SysTick counter
void SysTick_Handler(void) {
  msTicks++;
}

// ****************
// systick_delay - creates a delay of the appropriate number of Systicks (happens every 10 ms)
__INLINE static void systick_delay (uint32_t delayTicks) {
  uint32_t currentTicks;

  currentTicks = msTicks;	// read current tick counter
  // Now loop until required number of ticks passes.
  while ((msTicks - currentTicks) < delayTicks);
}

uint32_t get_diff(uint32_t value1, uint32_t value2) {
	if (value1 == value2) {
		return 0;
	}
	if (value1 > value2) {
		return (value1 - value2);
	}
	else {
		// check for timer overflow
		return (UINT32_MAX - value2 + value1);
	}
}


// ****************
int main(void) {
	
	// Setup SysTick Timer to interrupt at 10 msec intervals
	if (SysTick_Config(SystemCoreClock / 100)) {
	    while (1);  // Capture error
	}

	led_init();	// Setup GPIO for LED2
	led2_on();		// Turn LED2 on
	//led_on(0);
	//led_on(1);

	systick_delay(100);
	led2_off();
	systick_delay(100);
	led2_on();


	UARTInit(2, 115200);	/* baud rate setting */
	UARTSendCRLF(2);
	UARTSendCRLF(2);
	UARTSendStringln(2, "UART2 online ...");

	//EINT3_init();





	// Enter an infinite loop, just incrementing a counter and toggling leds every second
	//led2_off();
	//int ledstate;


	//EINT3_enable();
	logger_logStringln("logger online ...");
	while(1) {

		/* process logger */
		if (logger_dataAvailable() && UARTTXReady(2)) {
			uint8_t data = logger_read();
			UARTSendByte(2,data);
		}

		process_leds(msTicks);

		process_s0(msTicks);

		uint32_t triggerValue = s0_triggered(0);
		if (triggerValue) {
			logger_logString("s0_0:");
			logger_logNumberln(triggerValue);
			led_signal(0, 100, msTicks);
		}

		triggerValue = s0_triggered(1);
		if (triggerValue) {
			logger_logString("s0_1:");
			logger_logNumberln(triggerValue);
			led_signal(1, 100, msTicks);
		}


		/*
		if (!s0_active) {
			s0_newState = ~LPC_GPIO0->FIOPIN & (S0_INPUT0 | S0_INPUT1);
			if (s0_oldState != s0_newState) {
				s0_active = 1;
				s0_msticks = msTicks;
			}
		}

		if (s0_active && s0_msticks != msTicks) {
			s0_state = ~LPC_GPIO0->FIOPIN & (S0_INPUT0 | S0_INPUT1 );
			logger_logNumberln(s0_state);
			if (s0_state == s0_newState) {
				// falling edge
				if ((s0_newState & S0_INPUT0) > 0) {
					led2_invert();
				}

				// rising edge
				if ((s0_newState & S0_INPUT1) == 0) {
					led2_invert();
				}

			}
			s0_oldState = s0_state;
			s0_active = 0;
		}
		 */
	}
	return 0 ;
}
