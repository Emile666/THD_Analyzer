/*==================================================================
  File Name    : scheduler.h
  Author       : Emile
  ------------------------------------------------------------------
  Purpose : This is the header-file for scheduler.c
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
#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define MAX_TASKS	  (4)
#define MAX_MSEC      (60000)
#define TICKS_PER_SEC (1000L) /* 1000: 1 kHz interrupt frequency */
#define NAME_LEN         (12)

#define TASK_READY    (0x01)
#define TASK_ENABLED  (0x02)

#define NO_ERR        (0x00)
#define ERR_MAX_TASKS (0x03)
#define ERR_NAME      (0x04)
#define ERR_EMPTY     (0x05)

typedef struct _task_struct
{
	void     (* pFunction)(void); // Function pointer
	char     Name[NAME_LEN];      // Task name
	uint16_t Period;              // Period between 2 calls in msec.
	uint16_t Delay;               // Initial delay before Counter starts in msec.
	uint16_t Counter;             // Running counter, is init. from Period
	uint8_t	 Status;              // bit 1: 1=enabled ; bit 0: 1=ready to run
	uint16_t Duration;            // Measured task-duration in clock-ticks
	uint16_t Duration_Max;        // Max. measured task-duration
} task_struct;

void    scheduler_init(void); // clear task_list struct
void    scheduler_isr(void);  // run-time function for scheduler
void    dispatch_tasks(void); // run all tasks that are ready
uint8_t add_task(void (*task_ptr)(void), char *Name, uint16_t delay, uint16_t period);
uint8_t set_task_time_period(uint16_t Period, char *Name);
uint8_t enable_task(char *Name);
uint8_t disable_task(char *Name);
void    list_all_tasks(void);

#endif
