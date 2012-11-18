#ifndef __S0_INPUT_H
#define __S0_INPUT_H

// USING P0.23, P0.24, P0.25, P0.26; GPIO0!

#define S0_INPUT0 (1 << 23) // PIN 15
#define S0_INPUT1 (1 << 24) // PIN 16
#define S0_INPUT2 (1 << 25) // PIN 17
#define S0_INPUT3 (1 << 26) // PIN 18


void s0_init(void);
uint32_t read_s0_status(void);
void process_s0(uint32_t msticks);
uint32_t s0_triggered(uint8_t index);



#endif /* end __S0_INPUT_H */
