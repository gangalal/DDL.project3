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

#include <stdio.h>

// Initialize Ports
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
#define FIO0DIR (*(volatile unsigned int *)0x2009c000) // add switch
#define FIO0PIN (*(volatile unsigned int *)0x2009c014) // add switch

/**
 * PLL0 Feed Function
 */
void pllFeed(void)
{
    PLL0FEED = 0xAA;
    PLL0FEED = 0x55;
}

/**
 * volatile Integer M = 40 -> 10MHz, M = 36 -> 9MHz volatile integer N = 1;
 */

// Generate Frequency
void genFreq(int M, int N)
{
    PLL0CON &= ~(1 << 1); //disconnect PLL step1
    pllFeed();
    PLL0CON &= ~(1 << 0); //disable PLL step2
    pllFeed();
    CLKSRCSEL = 0; // Choose 4MHz Clock step4
    PLL0CFG = ((M - 1) << 0) | ((N - 1) << 16); // dividing clock step5
    pllFeed();
    PLL0CON |= (1 << 0); // Enable PLL0 to update step 6
    pllFeed();
    while ((PLL0STAT & (1 << 26)) == 0x00)
    {
        // Wait for PLOCK0 to become 1 step 7
    }
    CCLKCFG = 7; //dividing PLL clock by 8 step 8 , shift into bits 0,1, & 2
    CLKOUTCFG |= (1 << 4) | (1 << 5); //dividing CCLK clock by 4, shifting into bits 4 & 5

    PLL0CON |= (1 << 1); //connecting clock step9
    pllFeed();
    CLKOUTCFG |= (1 << 8); // enables us to read from P1.27
    PINSEL3 |= (1 << 22); // allows us to select P1.27 to measure clock
}

/**
 * wait function
 * @parameter count passes value to iterate number of times
 */
void wait_ticks(unsigned long count)
{
    volatile int ticks;
    for (ticks = 0; ticks < count; ticks++)
    {

    }
}


int main(void)
{
    while (1)
    {
        if (((FIO0PIN >> 4) & 0x01) == 0)
        {
            genFreq(36, 1); // if switch is pressed, adjust M and N values
            wait_ticks(10000000); // control switch bounce
        }
        else
        {
            genFreq(40, 1); // if no switch, generate 10MHz
            wait_ticks(100000); //oscilloscope won't generate frequency correctly so, we need to wait some time here
        }
    }
    return 0;
}
