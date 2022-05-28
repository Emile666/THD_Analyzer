/*==================================================================
  File Name    : scheduler.c
  Author       : Emile
  ------------------------------------------------------------------
  Purpose : This files contains all the functions for adding and
            executing tasks in a cooperative (non pre-emptive) way.
  ------------------------------------------------------------------
  This is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
 
  This file is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with this file.  If not, see <http://www.gnu.org/licenses/>.
  ==================================================================*/ 
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "scheduler.h"
#include "delay.h"
#include "uart.h"

task_struct task_list[MAX_TASKS]; // struct with all tasks
uint8_t max_tasks = 0;

/*-----------------------------------------------------------------------------
  Purpose  : Initialization function for scheduler. Should be called before 
	           calling any other scheduler function.
  Variables: task_list[] structure
  Returns  : -
  ---------------------------------------------------------------------------*/
void scheduler_init(void)
{
	  memset(task_list,0x00,sizeof(task_list)); // clear task_list array
} // scheduler_init()

/*-----------------------------------------------------------------------------
  Purpose  : Run-time function for scheduler. Should be called from within
             an ISR. This function goes through the task-list and decrements
             eacht task counter. On time-out, the ready flag is set.
  Variables: task_list[] structure
  Returns  : -
  ---------------------------------------------------------------------------*/
void scheduler_isr(void)
{
	uint8_t index = 0; // index in task_list struct

	while ((index < MAX_TASKS) && task_list[index].pFunction)
	{
		//First go through the initial delay
		if(task_list[index].Delay > 0)
		{
			task_list[index].Delay--;
		} // if
		else
		{	//now we decrement the actual period counter 
			task_list[index].Counter--;
			if(task_list[index].Counter == 0)
			{
				//Set the flag and reset the counter;
				task_list[index].Status |= TASK_READY;
			} // if
		} // else
		index++;
	} // while
} // scheduler_isr()

/*-----------------------------------------------------------------------------
  Purpose  : Run all tasks for which the ready flag is set. Should be called 
             from within the main() function, not from an interrupt routine!
  Variables: task_list[] structure
  Returns  : -
  ---------------------------------------------------------------------------*/
void dispatch_tasks(void)
{
	uint8_t  index = 0;
	uint32_t time1; // Measured #clock-ticks of 50 usec. (TMR1 frequency)
	uint32_t time2;

	//go through the active tasks
	while ((index < MAX_TASKS) && task_list[index].pFunction)
	{
		if((task_list[index].Status & (TASK_READY | TASK_ENABLED)) == (TASK_READY | TASK_ENABLED))
		{
            time1 = millis(); // Read msec. timer
			task_list[index].pFunction(); // run the task
			task_list[index].Status  &= ~TASK_READY; // reset the task when finished
			task_list[index].Counter  = task_list[index].Period; // reset counter
			time2 = millis(); // read msec. timer
			if (time2 < time1) time2 += UINT32_MAX - time1; // overflows every 49.7 days, unlikely
			else               time2 -= time1; 
			task_list[index].Duration  = (uint16_t)time2; // time difference in milliseconds
			if (time2 > task_list[index].Duration_Max)
			{
				task_list[index].Duration_Max = time2;
			} // if
		} // if
		index++;
	} // while
	//go to sleep till next tick!
} // dispatch_tasks()

/*-----------------------------------------------------------------------------
  Purpose  : Add a function to the task-list struct. Should be called upon
  		     initialization.
  Variables: task_ptr: pointer to function
             delay   : initial delay in msec.
             period  : period between two calls in msec.
  Returns  : [NO_ERR, ERR_MAX_TASKS]
  ---------------------------------------------------------------------------*/
uint8_t add_task(void (*task_ptr)(), char *Name, uint16_t delay, uint16_t period)
{
	uint8_t  index = 0;
	uint16_t temp1 = (uint16_t)(delay  * TICKS_PER_SEC / 1000);
	uint16_t temp2 = (uint16_t)(period * TICKS_PER_SEC / 1000);

	if (max_tasks >= MAX_TASKS) return ERR_MAX_TASKS;
	//go through the active tasks
	while ((index < MAX_TASKS) && task_list[index].Period) index++;
    if (index >= MAX_TASKS)     return ERR_MAX_TASKS;
	//if(task_list[index].Period != 0)
	//{
	//	while(task_list[++index].Period != 0) ;
	//} // if

	if (task_list[index].Period == 0)
	{
		task_list[index].pFunction    = task_ptr;       // Pointer to Function
		task_list[index].Period       = temp2;          // Period in msec.
		task_list[index].Counter      = temp2;	        // Countdown timer
		task_list[index].Delay        = temp1;          // Initial delay before start
		task_list[index].Status      |= TASK_ENABLED;   // Enable task by default
		task_list[index].Status      &= ~TASK_READY;    // Task not ready to run
		task_list[index].Duration     = 0;              // Actual Task Duration
		task_list[index].Duration_Max = 0;              // Max. Task Duration
		strncpy(task_list[index].Name, Name, NAME_LEN); // Name of Task
		max_tasks++; // increase number of tasks
	} // if
	return NO_ERR;
} // add_task()

/*-----------------------------------------------------------------------------
  Purpose  : Enable a task.
  Variables: Name: Name of task to enable
  Returns  : error [NO_ERR, ERR_NAME, ERR_EMPTY]
  ---------------------------------------------------------------------------*/
uint8_t enable_task(char *Name)
{
	uint8_t index = 0;
	bool    found = false;
	
	//go through the active tasks
	if(task_list[index].Period != 0)
	{
		while ((task_list[index].Period != 0) && !found)
		{
			if (!strcmp(task_list[index].Name,Name))
			{
				task_list[index].Status |= TASK_ENABLED;
				found = true;
			} // if
			index++;
		} // while
	} // if
	else return ERR_EMPTY;
	if (!found)
	     return ERR_NAME;
	else return NO_ERR;	
} // enable_task()

/*-----------------------------------------------------------------------------
  Purpose  : Disable a task.
  Variables: Name: Name of task to disable
  Returns  : error [NO_ERR, ERR_NAME, ERR_EMPTY]
  ---------------------------------------------------------------------------*/
uint8_t disable_task(char *Name)
{
	uint8_t index = 0;
	bool    found = false;
	
	//go through the active tasks
	if(task_list[index].Period != 0)
	{
		while ((task_list[index].Period != 0) && !found)
		{
			if (!strcmp(task_list[index].Name,Name))
			{
				task_list[index].Status &= ~TASK_ENABLED;
				found = true;
			} // if
			index++;
		} // while
	} // if
	else return ERR_EMPTY;
	if (!found)
	     return ERR_NAME;
	else return NO_ERR;	
} // disable_task()

/*-----------------------------------------------------------------------------
  Purpose  : Set the time-period (msec.) of a task.
  Variables: msec: the time in milliseconds
             name: the name of the task to set the time for
  Returns  : error [NO_ERR, ERR_NAME, ERR_EMPTY]
  ---------------------------------------------------------------------------*/
uint8_t set_task_time_period(uint16_t Period, char *Name)
{
	uint8_t index = 0;
	bool    found = false;
	
	//go through the active tasks
	if(task_list[index].Period != 0)
	{
		while ((task_list[index].Period != 0) && !found)
		{
			if (!strcmp(task_list[index].Name,Name))
			{
				task_list[index].Period = (uint16_t)(Period * TICKS_PER_SEC / 1000);
				found = true;
			} // if
			index++;
		} // while
	} // if
	else return ERR_EMPTY;
	if (!found)
	     return ERR_NAME;
	else return NO_ERR;	
} // set_task_time_period()

/*-----------------------------------------------------------------------------
  Purpose  : list all tasks and send result to the UART.
  Variables: -
 Returns   : -
  ---------------------------------------------------------------------------*/
void list_all_tasks(void)
{
	uint8_t index = 0;
	char    s[50];

	xputs("Task-Name,T(ms),Stat,T(ms),M(ms)\n");
	//go through the active tasks
	if(task_list[index].Period != 0)
	{
		while ((index < MAX_TASKS) && (task_list[index].Period != 0))
		{
            xputs(task_list[index].Name);
            
            sprintf(s,",%u,0x%x,%d,%d\n", 
                      task_list[index].Period  , (uint16_t)task_list[index].Status, 
                      task_list[index].Duration, task_list[index].Duration_Max);
	    xputs(s);
            index++;
		} // while
	} // if
} // list_all_tasks()
