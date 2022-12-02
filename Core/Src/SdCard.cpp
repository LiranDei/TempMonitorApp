/*
 * SdCard.cpp
 *
 *  Created on: Nov 27, 2022
 *      Author: student
 */
#include "SdCard.h"
#include "MonitorManager.h"
#include <stdio.h>
#include "cmsis_os2.h"
extern osMutexId_t filMutexHandle;

void SdCard :: print(char* fileName)
{
	osMutexAcquire(filMutexHandle, osWaitForever);
	char line[100];
	_fres = f_open(&_fil, fileName, FA_READ|FA_OPEN_ALWAYS);
	if(_fres != FR_OK)
	{
		LOG_DEBUGGER("(print) f_open error in file %s line %d\r\n", __FILE__, __LINE__);
	}

	while(f_gets(line, sizeof(line), &_fil))
	{
		LOG_RUN("%s\r", line);
	}
	LOG_RUN("\r\n");

	f_close(&_fil);
	osMutexRelease(filMutexHandle);

}

void SdCard :: clear(char* fileName)
{
	osMutexAcquire(filMutexHandle, osWaitForever);
	_fres = f_open(&_fil, fileName, FA_CREATE_ALWAYS);
	if(_fres != FR_OK)
	{
		LOG_DEBUGGER("(clear)f_open error in file %s in line %d\r\n", __FILE__, __LINE__);
	}
	else
	{
		LOG_RUN("The file %s has been cleared\r\n", fileName);
	}

	f_close(&_fil);
	osMutexRelease(filMutexHandle);
}

void SdCard :: write(char* fileName,char* str)
{
	osMutexAcquire(filMutexHandle, osWaitForever);
	_fres = f_open(&_fil, fileName, FA_WRITE | FA_OPEN_APPEND);
	if(_fres != FR_OK)
	{
		LOG_DEBUGGER("f_open error in file %s in line %d\r\n", __FILE__, __LINE__);
	}
	else
	{
		UINT bytesWrote;
		_fres = f_write(&_fil, str, strlen(str), &bytesWrote);
		if(_fres != FR_OK)
		{
			LOG_DEBUGGER("f_write error in file %s in line %d\r\n", __FILE__, __LINE__);
		}
		else
		{
			LOG_DEBUGGER("wrote %i bytes to %s \r\n", bytesWrote, fileName);
		}
	}
	f_close(&_fil);
	osMutexRelease(filMutexHandle);

}

void SdCard :: mount()
{
	_fres = f_mount(&_FatFs,"",1);
	if(_fres != FR_OK)
	{
		LOG_DEBUGGER("f_mount error in file %s line %d \r\n", __FILE__, __LINE__);
		LOG_RUN("mount failed %s\r\n","");
	}
}

void SdCard :: deMount()
{
	f_mount(NULL,"",0);
}

char* SdCard :: getFileName()
{
	return _fileName;
}

char* SdCard :: getErrorFileName()
{
	return _errorFileName;
}
