/*
 * task_game_code.c
 *
 *  Created on: 8 feb. 2021
 *      Author: Ashraf Tumah
 */

#include <alt_types.h>
#include <altera_avalon_sierra_ker.h>
#include "Race.h"

void task_game_code(void)
{
	volatile alt_u8 score;
	volatile alt_u16 x_val,y_val;
	volatile alt_u8 game=1, color=2;
	volatile alt_u16 xpos1=70,xpos2=100,xpos3=130,ypos=20;
	printf("Task game Init\n");
	task_periodic_start_union test;
	{
		init_period_time(25);
		while(game!=0)
		{
			test = wait_for_next_period();
			if(test.periodic_start_integer & 0x01)
			printf("Deadline miss, Task_Game");
			/******OBSTACLE ******/
			if (ypos<180)
			{
				draw_car(xpos1,ypos,0x0);
				draw_car(xpos2,ypos,0x0);
				draw_car(xpos3,ypos,0x0);
				ypos=ypos+10;
				draw_car(xpos1,ypos,color);
				draw_car(xpos2,ypos,color);
				draw_car(xpos3,ypos,color);
			}
			if (ypos>=180)
			{
				draw_car(xpos1,ypos,0x0);
				draw_car(xpos2,ypos,0x0);
				draw_car(xpos3,ypos,0x0);
				color = rand()% 4 + 2;
				xpos1 = rand()% 33 + 70;
				xpos2 = rand()% 33 + 103;
				xpos3 = rand()% 33 + 134;
				ypos=20;
				score=score+10;}
			/******OBSTACLE ******/

			/******colide ******/
			sem_take(SEM_GAME);
			if (ypos>=y_val-10 && ypos<=y_val+10)
			{
				if (xpos1>=x_val-10 && xpos1<= x_val)
				{
					game=0;
					end_screen();
				}
				else if (xpos2>=x_val-10 && xpos2<= x_val)
				{
					game=0;
					end_screen();
				}
				else if (xpos3>=x_val-10 && xpos3<= x_val)
				{
					game=0;
					end_screen();
				}
			}
			sem_release(SEM_GAME);
			/******COLIDE*******/

		}
	}
}
