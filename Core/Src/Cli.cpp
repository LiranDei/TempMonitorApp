/*
 * Cli.cpp
 *
 *  Created on: 19 בנוב׳ 2022
 *      Author: USER
 */
#include "Cli.h"
#include "stdlib.h"
#include "Dht.h"
#include "MonitorManager.h"
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>


extern Flash* thresholdsFlash;
extern Container* container;
extern Rtc* rtc;
extern Dht* dht;
extern SdCard* logSdCard;


void SetFlashWarning :: doCommand(char* param)
{
	if(param == nullptr)
	{
		LOG_RUN("Please insert number %s\r\n", "");
	}
	else
	{
		double value;
		if(sscanf(param,"%lf",&value) == 1)
		{
			_flash->setWarning(value);
		}

	}
}

void SetFlashCritical :: doCommand(char* param)
{
	if(param == nullptr)
	{
		LOG_RUN("Please insert number %s\r\n", "");
	}
	else
	{
		double value;
		if(sscanf(param,"%lf",&value) == 1)
		{
			_flash->setCritical(value);
		}

	}
}


void GetFlashInfo :: doCommand(char* param)
{

	_flash->printThresHolds();
	LOG_RUN("temp = %.2f \r\n", dht->getTemp());
}

void SetRtcTime :: doCommand(char* param)
{
	DateTime dateTime = {0};
	int day,month,year,hours,min,sec;
	int ret = 0;
	if(sscanf(param, "%d.%d.%d %d:%d:%d", &day, &month, &year, &hours, &min, &sec ) == 6)
	{
		do
		{
			if(day < 0 || day > 31)
			{
				ret = 1;
				break;
			}
			dateTime.day = day;
			if(month < 0 || month > 12)
			{
				ret = 1;
				break;
			}
			dateTime.month = month;
			if(year < 0 || year > 99)
			{
				ret = 1;
				break;
			}
			dateTime.year = year;
			if(hours < 0 || hours > 23)
			{
				ret = 1;
				break;
			}
			dateTime.hours = hours;
			if(min < 0 || min > 59)
			{
				ret = 1;
				break;
			}
			dateTime.min = min;
			if(sec < 0 || sec > 59)
			{
				ret = 1;
				break;
			}
			dateTime.sec = sec;
		}while(0);

		if(ret == 0)// its mean no error - valid parmeters.
		{
			rtc->rtcSetTime(&dateTime);
			LOG_RUN("date successfully updated! %s\r\n","");
		}
		else
		{
			LOG_RUN("not valid parameters %s \r\n","");
		}
	}
	else
	{
		LOG_RUN("please try again - bad input %s\r\n","");
	}
}

void GetRtcTime :: doCommand(char* param)
{
	(void)param;
	DateTime dateTime;
	rtc->rtcGetTime(&dateTime);
	LOG_RUN("%02d:%02d:%02d %02d.%02d.%02d\r\n",
			dateTime.hours, dateTime.min, dateTime.sec,
			dateTime.day, dateTime.month, dateTime.year);

}

void ClearLog :: doCommand(char* param)
{
	if(strcmp(getCommandName(),"clearErrorLog") == 0)
	{
		_sdCard->clear(_sdCard->getErrorFileName());
	}
	if(strcmp(getCommandName(),"clearLog") == 0)
	{
		_sdCard->clear(_sdCard->getFileName());
	}
}

void PrintLog :: doCommand(char* param)
{
	if(strcmp(getCommandName(),"printErrorLog") == 0)
	{
		_sdCard->print(_sdCard->getErrorFileName());
	}
	if(strcmp(getCommandName(),"printLog") == 0)
	{
		_sdCard->print(_sdCard->getFileName());
	}
}



void Container :: add (ICommand* command)
{
	if(_counter < COMMAND_ARR_SIZE)
	{
		_iCommand[_counter++] = command;
	}
}


int Container :: handle(const char* name, char* param)
{
	for(int i = 0; i < _counter; i++)
	{
		if(strcmp(name, _iCommand[i]->getCommandName()) == 0)
		{
			_iCommand[i]->doCommand(param);
			return 1;
		}
	}
	return 0;
}

void Container :: registerCommand()
{
	SetFlashWarning* setFlashWarning = new SetFlashWarning("setWarning", thresholdsFlash);
	container->add(setFlashWarning);

	SetFlashCritical* setFlashCritical = new SetFlashCritical("setCritical", thresholdsFlash);
	container->add(setFlashCritical);

	GetFlashInfo* getFlashInfo = new GetFlashInfo("getFlashInfo", thresholdsFlash);//print curr critical and warning
	container->add(getFlashInfo);

	SetRtcTime* setRtcTime = new SetRtcTime("setDate", rtc);
	container->add(setRtcTime);

	GetRtcTime* getRtcTime = new GetRtcTime("getDate", rtc);
	container->add(getRtcTime);

	ClearLog* clearLog = new ClearLog("clearLog", logSdCard);
	container->add(clearLog);

	ClearLog* clearErrorLog = new ClearLog("clearErrorLog", logSdCard);
	container->add(clearErrorLog);

	PrintLog* printLog = new PrintLog("printLog", logSdCard);
	container->add(printLog);

	PrintLog* printErrorLog = new PrintLog("printErrorLog", logSdCard);
	container->add(printErrorLog);


}
