/*
 * vga.c
 *
 *  Created on: 30 jan. 2021
 *      Author: Ashraf Tumah
 */
#include <alt_types.h>
#include "vga.h"
#include <altera_avalon_pio_regs.h>
#include <stdlib.h>
#include <system.h>


void key_1_down_up()
{
	alt_u8 key_r;
	while(1)
	{
		int key_in = KEY_1 & IORD_ALTERA_AVALON_PIO_DATA(PIO_BUTTONS_IN_BASE);

		if(key_in!=KEY_1)
		{
			key_r=0;
			for(size_t i=10000;i>0;i--);
		}
		if ((key_in==KEY_1) && (key_r==0)) break;
	}
}



void init_vga()
{
	clear_screen(0x0);
	tty_print(10,10,"TEIS AB 21-02-01",0x7,0x0);
	tty_print(10,20,"Engineer Job C",0x7,0x0);
	tty_print(10,40,"By Ashraf Tumah",0x7,0x0);
	tty_print(70,80,"Welcome to Retro Race!",0x6,0x0);
	tty_print(70,120,"Press Key1 to continue...",0x6,0x0);
}
