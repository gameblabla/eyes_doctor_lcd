#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "API.h"
#include "INPUT.h"

/* 
 * Distraction for Eyes 
 * By gameblabla
 * Do whatever you want with it (wrapper is under the apache license though)
*/

unsigned long long timey = 0;
unsigned long long timey_blink = 0;
unsigned long long timey_blink_sub = 0;
int mode = 0;
static unsigned long int next = 1;

int rand(void);
short rand_a_b(short a, short b);

int main ( int argc, char* argv[] )
{
	srand(time(NULL));
	timey = 0;
	timey_blink = 0;
	mode = 0;

    while (!done)
    {
        switch(mode)
        {
			case 2:
				Clear_screen();
				Draw_Rect(0, 0, 320, 240, 255, 0, 0);
				timey++;
				timey_blink_sub++;
				if (timey_blink_sub > 120)
				{
					Close_video();
					timey_blink_sub = 0;
					timey_blink = 0;
					mode = 0;
				}
				Update_video();
			break;
			case 1:
				Clear_screen();
				Draw_Rect(0, 0, 320, 240, rand_a_b(0,255), rand_a_b(0,255), rand_a_b(0,255));
				timey++;
				if (timey > 1200)
				{
					Close_video();
					timey = 0;
					mode = 0;
				}
				Update_video();
			break;
			case 0:
				timey++;
				timey_blink++;
				if (timey > 54000)
				{
					timey = 0;
					mode = 1;
					Init_video();
				}
				if (timey_blink > 3600)
				{
					timey_blink = 0;
					mode = 2;
					Init_video();
				}
				Sync_refresh();
			break;
		}
    } // end main loop

    Clearing();
    return 0;
}

int rand(void)
{
    next = next * 1103515245 + 12345;
    return (unsigned int)(next/65536) % 32768;
}

short rand_a_b(short a, short b)
{
    return rand()%(b-a) +a;
}
