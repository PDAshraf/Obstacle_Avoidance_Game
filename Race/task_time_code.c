/*
 * task_time_code.c

 *
 *  Created on: 8 feb. 2021
 *      Author: Ashraf Tumah
 */

#include <altera_avalon_sierra_ker.h>
#include <alt_types.h>
#include "Race.h"

void task_time_code(void)
{
	volatile alt_u8 score, min, sec;
	volatile alt_u8 game;
    printf("Task Time Init\n");
    task_periodic_start_union test;
    {
		init_period_time(50);

		while(game!=0)
		{
			test = wait_for_next_period();
			if(test.periodic_start_integer & 0x01)
			printf("Deadline miss, Task_Time");

			if (sec<=58)
			{
				sec=sec+1;
				score=score+1;

			}
			else
			{
				sec=0;
				min = min+1;
				score=score+100;
			}
			sem_take(SEM_LOG);
			int_print(250, 10, score,4, 0x2,0x7);
			int_print(95, 205, min,2, 0x2,0x7);
			int_print(95, 215, sec,2, 0x2,0x7);
			sem_release(SEM_LOG);
		}


	}

}
