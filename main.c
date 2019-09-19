/*
 ===============================================================================
 Name        : project2.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
 ===============================================================================
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <stdio.h>

#define PLL0CON (*(volatile unsigned char *)(0x400FC080))
#define PLL0FEED (*(volatile unsigned char *)(0x400FC08C))
#define PLL0STAT (*(volatile unsigned int *)(0x400FC088))
#define PLL0CFG (*(volatile unsigned int *)(0x400FC084))
#define CCLKCFG (*(volatile unsigned char *)(0x400FC104))
#define CLKOUTCFG (*(volatile unsigned int *)(0x400FC1C8))
#define CLKSRCSEL (*(volatile unsigned int *)(0x400FC10C))
#define PINSEL3 (*(volatile unsigned int *)(0x4002C00C))

volatile int M = 50;
volatile int N = 1;

int main(void) {
	//CLKSRCSEL |= (0)
	PLL0CON &= ~(1 << 1);
	PLL0FEED = 0xAA;
	PLL0FEED = 0x55;
	//RC source is default, we didn't write to CLKSRCSEL
	while ((PLL0STAT & (1 << 24)) == 0x00)
			;
	PLL0CON &= ~(1 << 0);
	PLL0FEED = 0xAA;
	PLL0FEED = 0x55;

	while ((PLL0STAT & (1 << 24)) == 0x00)
			;
	// Turn on PLL0
	PLL0CFG = 50;//((M-1)<<0)|((N-1)<<16);
	PLL0FEED = 0xAA; // Feed the PLL
	PLL0FEED = 0x55;
	PLL0CON |= 1 << 0;
	PLL0FEED = 0xAA; // Feed the PLL
	PLL0FEED = 0x55;
	// Wait for main PLL (PLL0) to come up
	while ((PLL0STAT & (1 << 24)) == 0x00)
		;
	// Wait for PLOCK0 to become 1
	while ((PLL0STAT & (1 << 26)) == 0x00)
		;
	// Connect to the PLL0
	PLL0CON |= 1 << 1;
	PLL0FEED = 0xAA; // Feed the PLL
	PLL0FEED = 0x55;
	while ((PLL0STAT & (1 << 25)) == 0x00)
		; //Wait for PLL0 to connect

	// shifting 3 into CCLKCFG selecting clock divider value C
	CCLKCFG |= (1 << 0);
	CCLKCFG |= (1 << 1);

	CLKOUTCFG &= ~(1<<0);
	CLKOUTCFG &= ~(1<<1);
	CLKOUTCFG &= ~(1<<2);
	CLKOUTCFG &= ~(1<<3);


	// shifting 10 into CLKOUTCFG selecting clock divider value K
	CLKOUTCFG |= (1 << 5);
	CLKOUTCFG |= (1 << 7);

	// allows us to select P1.27 to measure clock
	PINSEL3 |= (1 << 22);

	// enables us to read from P1.27
	CLKOUTCFG |= (1 << 8);

	return 0;
}

