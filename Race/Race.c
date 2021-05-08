/*****************************************************************************
 *      Company : TEIS AB       *											 *
 ********************************											 *
 *  Engineer : Ashraf Tumah     *											 *
 *  Date     : 20-02-06         *											 *
 *  Task     : C_eng_job_b_M    *											 *
 ********************************											 *
 *   Description:                                                            *
 *      Main file in which contain a state machine.						     *
 *      start_screen_state - Generate start screen with information          *
 *      game_on_state - Generate game screen and activates task switch       *
 *                        which starts the communication between tasks.      *
 *                                                                           *
 *****************************************************************************/

//Sierra RTK
#include <altera_avalon_sierra_ker.h>
#include <altera_avalon_sierra_io.h>
#include <altera_avalon_sierra_regs.h>
#include <altera_avalon_sierra_name.h>
#include <altera_avalon_pio_regs.h>
#include <altera_avalon_timer_regs.h>
#include <stdio.h>
#include <io.h>
#include <stdlib.h>

#include <alt_types.h>
#include <DE10_LITE_ARDUINO_DRIVER.h>


#include <draw_vga.h>
#include <DE10_Lite_VGA_Driver.h>
#include <vga.h>

#include "Race.h"
#include "task_game_code.h"
#include "task_player_code.h"
#include "task_time_code.h"
#include "idle_task.h"


volatile alt_u8 score, min, sec;

void road_frame_black()
{
	alt_u8 x,y;
	for (y=0; y<200;y++)
	{
		for (x=50; x<190; x++)
		{
			write_pixel(x,y,0x0);
		}
	}
}

void end_screen()
{
	clear_screen(0x0);
	clear_screen(0x0);
	tty_print(100,80,"Game Over!",0x6,0x0);
	tty_print(100,120,"Score:",0x6,0x0);
	tty_print(100,140,"Time :",0x6,0x0);
	int_print(150, 120, score,4, 0x5,0x0);
	int_print(150, 140, min,2, 0x2,0x0);
	int_print(180, 140, sec,2, 0x2,0x0);
}

void  draw_car(alt_u32 x1,alt_u32 y1, alt_u32 color)
{
	alt_u16 x,y;
	alt_u16 x2=x1+10,y2=y1+15;

	for(y=y1;y<y2;y++)
	{
		for(x=x1;x<x2;x++)
		{
			write_pixel(x,y,color);
		}

	}

}

/********************
 * MAIN             *
 ********************/
int main (void)
{
	char state;
	enum state {start_screen_state, game_on_state};
	//Sierra Help function
	Sierra_Initiation_HW_and_SW();

	//Printa HW/SW Versioner
	printf(" Sierra HW version = %d\n", sierra_HW_version());
	printf(" Sierra SW driver version = %d\n", sierra_SW_driver_version());

	printf("Accelerometer Start\n");
	accelerometer_open_dev();
	accelerometer_init();



	/*******************************************
	 * 50MHz Systemfrekvens                    *
	 * 20ms(50Hz) Tick periodtid               *
	 * ----------------------------------------*
	 * 20ms x 50Mhz / 1000 => 1000(dec)        *
	 * Resultat: set_timebase(1000)            *
	 *******************************************/
		set_timebase(1000);

		//TaskCreate
		task_create(IDLE, 0, READY_TASK_STATE,idle_task_code, idle_stack, STACK_SIZE);
		task_create(Task_Time,1, READY_TASK_STATE, task_time_code, task_time_stack, STACK_SIZE);
		task_create(Task_Game,2, READY_TASK_STATE, task_game_code, task_game_stack, STACK_SIZE);
		task_create(Task_Player,3, READY_TASK_STATE, task_player_code, task_player_stack,STACK_SIZE);




	   /**********StartScreen---GameOn***********/

	printf("* Error! SYSTEM FAILED *\n");
}
