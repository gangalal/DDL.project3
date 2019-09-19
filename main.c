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
#define FIO0DIR (*(volatile unsigned int *)0x2009c000)
#define FIO0PIN (*(volatile unsigned int *)0x2009c014)

volatile int M = 40;
volatile int N = 2;

// PLL0 Feed Function
void pllFeed(void) {
	PLL0FEED = 0xAA;
	PLL0FEED = 0x55;
}


int main(void) {
	while(1)
	{

	}
	CLKOUTCFG |= (1 << 8); // enables us to read from P1.27
	PINSEL3 |= (1 << 22); // allows us to select P1.27 to measure clock

	PLL0CON &= ~(1 << 1); //disconnect PLL step1
	pllFeed();

	PLL0CON &= ~(1 << 0); //disable PLL step2
	pllFeed();

	CLKSRCSEL = 0; // Choose 4MHz Clock step4

	PLL0CFG = ((M - 1) << 0) | ((N - 1) << 16); // dividing clock step5
	pllFeed();

	PLL0CON |= (1 << 0); // Enable PLL0 to update step 6    //set cpu clk then connect
	pllFeed();

	while ((PLL0STAT & (1 << 26)) == 0x00) {
	} // Wait for PLOCK0 to become 1 step 7

	CCLKCFG = 7; //dividing PLL clock by 4 step 8

	CLKOUTCFG = 16; //dividing CLLK clock by 8

	PLL0CON |= (1 << 1); //connecting clock //enable step9
	pllFeed();

	CLKOUTCFG |= (1 << 8); // enables us to read from P1.27
	PINSEL3 |= (1 << 22); // allows us to select P1.27 to measure clock

	return 0;
}
