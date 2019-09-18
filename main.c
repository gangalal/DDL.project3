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
#define FIO1DIR (*(volatile unsigned int *) (0x2009C020))
#define FIO1PIN (*(volatile unsigned int *) (0x2009C034))
#define PINSEL3 (*(volatile unsigned int *)(0x4002C00C))

volatile int M = 50;
volatile int N = 1;

int main(void) {
	//CLKSRCSEL |= (0)
	PLL0CON &= ~(1 << 1);
	PLL0FEED = 0xAA;
	PLL0FEED = 0x55;
	//RC source is default, we didn't write to CLKSRCSEL

	PLL0CON &= ~(1 << 0);
	PLL0FEED = 0xAA;
	PLL0FEED = 0x55;
	// Turn on PLL0
	PLL0CFG = 0x32;
	PLL0FEED = 0xAA; // Feed the PLL
	PLL0FEED = 0x55;
	PLL0CON |= 1 << 0;
	PLL0FEED = 0xAA; // Feed the PLL
	PLL0FEED = 0x55;

	/*CLKSRCSEL = 3;
	CLKOUTCFG = 10;*/
	PINSEL3 |= (1 << 22);
	FIO1DIR |= (1 << 27);
	FIO1PIN |= (1 << 27);
	CLKOUTCFG |= (1 << 8); // clock out config bit 8
	while (1) {

	}
	return 0;
}

