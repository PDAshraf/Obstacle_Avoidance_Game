/*
 * Race.h
 *
 *  Created on: 29 jan. 2021
 *      Author: Ashraf Tumah
 */

#ifndef RACE_H_
#define RACE_H_

//Tasks
#define IDLE            0
#define Task_Time	    1
#define Task_Game		2
#define Task_Player     3

//Semaphore
#define SEM_LOG  1
#define SEM_GAME 2

//Stack
#define STACK_SIZE 800
char idle_stack[STACK_SIZE];
char task_time_stack[STACK_SIZE];
char task_game_stack[STACK_SIZE];
char task_player_stack[STACK_SIZE];
char task_plot_stack[STACK_SIZE];



#endif /* RACE_H_ */
