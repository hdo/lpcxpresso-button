#include "LPC17xx.h"
#include "s0_input.h"

uint32_t s0_msticks0 = 0;
uint32_t s0_diff0 = 0;
uint32_t s0_oldState = 0;
uint32_t s0_newState = 0;

void s0_init(void) {
	// no need to set PINSEL, since GPIO (00) is selected as default
	// no need to set PINMODE, since Pull-Up (00) is selected as default
	// no need to set FIODIR, since INPUT (0) is selected as default
}

uint32_t read_s0_status() {
	// we're are using GPIO0 here !!!
	return ~LPC_GPIO0->FIOPIN & (S0_INPUT0 | S0_INPUT1 | S0_INPUT2 | S0_INPUT3 );
}

void process_s0(uint32_t msTicks) {
	s0_newState = read_s0_status();
	if (s0_newState != s0_oldState) {
		// if triggered
		if (s0_newState & S0_INPUT0) {
			// if old is 0
			if ((s0_oldState & S0_INPUT0) == 0) {
				s0_msticks0 = msTicks;
				s0_diff0 = 0;
			}
		}
		else {
			// if old is 1
			if (s0_oldState & S0_INPUT0) {
				s0_diff0 = msTicks - s0_msticks0;
				//s0_diff0 = get_diff(msTicks, s0_msticks0);
			}
		}
		s0_oldState = s0_newState;
	}
}

uint32_t s0_triggered() {
	if (s0_diff0 > 10) {
		uint32_t ret = s0_diff0;
		s0_diff0 = 0;
		return ret;
	}
	return 0;
}
