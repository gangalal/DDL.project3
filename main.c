/*
===============================================================================
 Name        : Assignment_3.c
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

volatile int M = 51;
volatile int N = 2;

// PLL0 Feed Function
void pllFeed (void) {
	PLL0FEED = 0xAA;
	PLL0FEED = 0x55;
}

int main(void) {

	//CLKSRCSEL |= (0);
//	PLL0CON &= ~(1 << 1);
//	pllFeed();
	//RC source is default, we didn't write to CLKSRCSEL
//	while ((PLL0STAT & (1 << 24)) == 0x00)
//	;
	PLL0CON &= ~(1 << 0);
	pllFeed();

//	while ((PLL0STAT & (1 << 24)) == 0x00)
//			;

// Choose 4MHz Clock
	CLKSRCSEL = 0;

	// Turn on PLL0
	PLL0CFG = ((M-1)<<0)|((N-1)<<16);
	pllFeed();
	PLL0CON |= (1 << 0);
	pllFeed();

	// Wait for main PLL (PLL0) to come up
//	while ((PLL0STAT & (1 << 24)) == 0x00)
//		;
	// Wait for PLOCK0 to become 1
	while ((PLL0STAT & (1 << 26)) == 0x00)
	{}

	// Enable PLL0 to update
	PLL0CON = 0x1;
	pllFeed();

	PLL0CON |= (1<<1);
	pllFeed();
//	while ((PLL0STAT & (1 << 25)) == 0x00)
//		; //Wait for PLL0 to connect

	// shifting 3 into CCLKCFG selecting clock divider value C
	CCLKCFG = 4;

	// Connect to the PLL0
//PLL0CON |= 1 << 1;
//pllFeed();

//CLKOUTCFG &= ~(1<<0);
//CLKOUTCFG &= ~(1<<1);
//CLKOUTCFG &= ~(1<<2);
//CLKOUTCFG &= ~(1<<3);




	// shifting 10 into CLKOUTCFG selecting clock divider value K
	CLKOUTCFG = 10;

	// enables us to read from P1.27
	CLKOUTCFG |= (1 << 8);

	// allows us to select P1.27 to measure clock
	PINSEL3 |= (1 << 22);


	return 0;
}

