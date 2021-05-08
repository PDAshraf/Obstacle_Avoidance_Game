/*
 * task_player_code.c

 *
 *  Created on: 8 feb. 2021
 *      Author: Ashraf Tumah
 */

#include <altera_avalon_sierra_ker.h>
#include <alt_types.h>
#include "Race.h"

void task_player_code(void)
{
	volatile alt_u16 x_val=70,y_val=170;
	volatile alt_u8 game=1;
	volatile alt_32 accelerometer_x, accelerometer_y,accelerometer_z;
	alt_32 acc_x,acc_y;
	printf("Task Player Init\n");
	task_periodic_start_union test;
	{
	    init_period_time(10);
	    while(game!=0)
	    {
			test = wait_for_next_period();
			if(test.periodic_start_integer & 0x01)
			printf("Deadline miss, Task_Player");

	    	accelerometer_read
				( &accelerometer_x,
				  &accelerometer_y,
				  &accelerometer_z);
	    	acc_x=accelerometer_x;
	        acc_y=accelerometer_y;

	        /**********Player ******/
	        draw_car(x_val,y_val,0x1);
			if (acc_x >100 && acc_x < 250)
				{
					if(y_val<180)
					{
						draw_car(x_val,y_val,0x0);
						y_val=y_val+10;
						draw_car(x_val,y_val,0x1);
					}

				}
				else if (acc_x<-100 && acc_x>-250)
				{
					 if(y_val > 20)
					 {
						 draw_car(x_val,y_val,0x0);
						 y_val=y_val-10;
						 draw_car(x_val,y_val,0x1);
					 }

				}

				else if (acc_y >100 && acc_y <250)
				{
					if (x_val < 170)
					{
						draw_car(x_val,y_val,0x0);
						x_val=x_val+10;
						draw_car(x_val,y_val,0x1);
					}
				}
				else  if (acc_y <-100 && acc_y >-250)
				{
					if (x_val > 60)
					{
						draw_car(x_val,y_val,0x0);
						x_val=x_val-10;
						draw_car(x_val,y_val,0x1);
					}

				}
	/******Player ******/
	 }


  }
}
