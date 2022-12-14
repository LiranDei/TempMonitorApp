/*
 * commTask.c
 *
 *  Created on: Oct 20, 2022
 *      Author: student
 */

#include "commTask.h"
#include "cmsis_os.h"
#include "Cli.h"
#include "MonitorManager.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern  UART_HandleTypeDef huart2;
extern Container* container;
#define MAX_BUFFER_LENGTH 100

uint8_t ch;
uint8_t cmdbuffer[MAX_BUFFER_LENGTH];
int cmdcount = 0;

void commTask(void *argument)
{
	while(1)
	{
		if(commTaskReady(&huart2))
		{
			handleCommand();
		}
	}

}

int commTaskReady(UART_HandleTypeDef* huart2)
{
	HAL_StatusTypeDef Status = HAL_UART_Receive(huart2, &ch, 1, 0);

	if (Status != HAL_OK) {
		return 0;
	}
	if (ch != '\r' && ch != '\n')
	{
		HAL_UART_Transmit(huart2, &ch, 1, 0xFFFF);

		if (cmdcount >= MAX_BUFFER_LENGTH)
		{
			cmdcount = 0;
		}

		cmdbuffer[cmdcount++] = ch;
		return 0;
	}
	HAL_UART_Transmit(huart2, (uint8_t*)"\r\n", 2, 0xFFFF);

	cmdbuffer[cmdcount] = 0;
	cmdcount = 0;
	return 1;
}

void handleCommand()
{
	char * param = NULL;

	// find space to split a command to a command name and parameters
	char * delimPtr = strchr((const char*)cmdbuffer, ' ');
	if (delimPtr != NULL)
	{
		// put null character to split command name and parameters
		*delimPtr = '\0';
		param = delimPtr + 1;

		// find first non space character - it's parameters beginning
		while (*param && *param == ' ')
		{
			param++;
		}

		if (*param == '\0')
		{
			param = nullptr;
		}
	}

	if(container->handle((char *)cmdbuffer, param))
	{
			return;
	}

	LOG_RUN("Invalid command\r\n");
}
