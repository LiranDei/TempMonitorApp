/*
 * MonitorManager.cpp
 *
 *  Created on: Nov 17, 2022
 *      Author: student
 */

#include <iostream>
#include "main.h"
#include "Btn.h"
#include "SdCard.h"
#include "MonitorManager.h"
#include "Dht.h"
#include "Buzzer.h"
#include "Rtc.h"
#include "Led.h"
#include "cmsis_os.h"
#include "Cli.h"
#include "Clock.h"
#include "Flash.h"
#include <stdio.h>


enum{ONE_MINUTE = 60, MAX_SIZE_LOG_BUFFER = 100, ONE_SECOND = 1000, DHT_DEVICE_ADDR = 0xD0, THRESHOLDS_PAGE_256 = 0x08080000};

extern  TIM_HandleTypeDef htim6;
extern  TIM_HandleTypeDef htim3;
extern  I2C_HandleTypeDef hi2c1;
extern osSemaphoreId_t measureSemaphoreHandle;
extern osSemaphoreId_t logSemaphoreHandle;

Btn* rightBtn = new Btn(rightBtn_GPIO_Port, rightBtn_Pin);
Dht* dht = new Dht(DHT_GPIO_Port, DHT_Pin, &htim6);
Rtc* rtc = new Rtc(&hi2c1, DHT_DEVICE_ADDR);
Buzzer* buzzer = new Buzzer (&htim3, TIM_CHANNEL_1);
Flash* thresholdsFlash = new Flash (THRESHOLDS_PAGE_256, 1, FLASH_TYPEPROGRAM_DOUBLEWORD);
Container* container = new Container();
LedGpio* blueLed = new LedGpio(blueLed_GPIO_Port, blueLed_Pin);
LedGpio* redLed = new LedGpio(redLed_GPIO_Port, redLed_Pin);
SdCard* logSdCard = new SdCard("Log.txt", "ErrorLog.txt");
Clock* clock; // clock for fix Rtc clock accuracy

char logBuffer[MAX_SIZE_LOG_BUFFER];
uint32_t timeKeep = 0;

void monitorInit()
{
	HAL_TIM_Base_Init(&htim6);
	container->registerCommand(); // registers user command in the cli.cpp

	thresholdsFlash->initThresHolds();// init warning and critical from the flash
	thresholdsFlash->printThresHolds();// print critical and warning

	DateTime dateTime;
	rtc->rtcGetTime(&dateTime);
	clock = new Clock(dateTime.hours, dateTime.min, dateTime.sec);// init clock Time

	osSemaphoreAcquire(measureSemaphoreHandle, osWaitForever);
	osSemaphoreAcquire(logSemaphoreHandle, osWaitForever);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) //when right btn pressed
{
	if(buzzer->getStatus() == BUZZER_ON)
	{
		buzzer->buzzerOff();
		if(thresholdsFlash->getTempState() == CRITICAL) // in case we want turn off the buzzer in crit temp
		{
			thresholdsFlash->setSoundCanceled(SOUND_CANCELD);
		}
	}
}

void accurateMeasureTime(void *argument)
{

	int countSec = 0;
	while(1)
	{
		int time = HAL_GetTick() - timeKeep ;
		if(time >= ONE_SECOND) // when second passed
		{
			++*clock;
			if(time > ONE_SECOND)
			{
				printf("time = %d\r\n", time);
			}

			timeKeep = HAL_GetTick();
			osSemaphoreRelease(measureSemaphoreHandle);
			countSec = (countSec + 1) % ONE_MINUTE;
			if(countSec == 0) // when minute passed
			{
				DateTime dateTime;
				rtc->rtcGetTime(&dateTime);
				if(dateTime.sec != clock->getSec()) // Fix Rtc's clock accuracy
				{
					dateTime.sec = clock->getSec();
					dateTime.min = clock->getMin();
					dateTime.hours = clock->getHour();
					rtc->rtcSetTime(&dateTime);
				}
				osSemaphoreRelease(logSemaphoreHandle);
			}
		}
		osDelay(1);
	}

}

void measureTemp(void *argument)
{

	while(1)// this task measure every one second the temp.
	{
		osSemaphoreAcquire(measureSemaphoreHandle, osWaitForever);
		if(dht->dhtRead() != HAL_OK)
		{
			LOG_ERROR("error in dht read in file %s line %d\r\n", __FILE__, __LINE__);
		}

	}
}


void writeLog(void *argument)// this task write to the log file
{
	osDelay(1000);
	logSdCard->mount();
	while(1)
	{
		osSemaphoreAcquire(logSemaphoreHandle, osWaitForever);
		updateLogBuffer();
		logSdCard->write(logSdCard->getFileName(),logBuffer);
	}
}

void monitorTask(void *argument)
{
	while(1)
	{
		buzzer->buzzerOff();
		TEMP_STATE tempState = NORMAL;
		double temp = dht->getTemp();
		if(temp >= thresholdsFlash->getWarning())
		{
			tempState = WARNING;
		}
		if(temp >= thresholdsFlash->getCritical())
		{
			tempState = CRITICAL;
		}

		switch(tempState)
		{
		case NORMAL:
			thresholdsFlash->setTempState(NORMAL);
			blueLed->ledOn();
			redLed->ledOff();
			thresholdsFlash->setSoundCanceled(SOUND_ON);
			break;
		case WARNING:
			if(thresholdsFlash->getTempState() != WARNING)// first time in warning state - write to errorfile
			{
				thresholdsFlash->setTempState(WARNING);
				updateLogBuffer();
				logSdCard->write(logSdCard->getErrorFileName(),logBuffer);
			}
			blueLed->ledOff();
			redLed->ledOn();
			thresholdsFlash->setSoundCanceled(SOUND_ON);
			break;
		case CRITICAL:
			if(thresholdsFlash->getTempState() != CRITICAL)// first time in critical state - write to errorfile
			{
				thresholdsFlash->setTempState(CRITICAL);
				updateLogBuffer();
				logSdCard->write(logSdCard->getErrorFileName(),logBuffer);
			}
			blueLed->ledOff();
			redLed->ledBlink();
			if(thresholdsFlash->getSoundCanceled() != SOUND_CANCELD)
			{
				buzzer->buzzerOn();
			}
			break;
		default:
			LOG_ERROR("default in file %s line %d\r\n", __FILE__, __LINE__);
		}

		tempState >= WARNING ? osDelay(redLed->getPeriod()): osDelay(blueLed->getPeriod());

	}

}

void updateLogBuffer()
{

	DateTime dateTime;
	rtc->rtcGetTime(&dateTime);
	TEMP_STATE tempState = thresholdsFlash->getTempState();

	switch(tempState)
	{
	case NORMAL:
		sprintf(logBuffer, "\r\n date:%02d.%02d.%02d time: %02d:%02d:%02d temp: %.2f status: NORMAL",
				dateTime.day, dateTime.month, dateTime.year,
				dateTime.hours, dateTime.min, dateTime.sec,
				dht->getTemp());
		break;
	case WARNING:
		sprintf(logBuffer, "\r\n date:%02d.%02d.%02d time: %02d:%02d:%02d temp: %.2f status: WARNING",
				dateTime.day, dateTime.month, dateTime.year,
				dateTime.hours, dateTime.min, dateTime.sec,
				dht->getTemp());
		break;
	case CRITICAL:
		sprintf(logBuffer, "\r\n date:%02d.%02d.%02d time: %02d:%02d:%02d temp: %.2f status: CRITICAL",
				dateTime.day, dateTime.month, dateTime.year,
				dateTime.hours, dateTime.min, dateTime.sec,
				dht->getTemp());
		break;
	default:
		LOG_DEBUGGER("Wrong status file: %s line: %d \r\n", __FILE__, __LINE__);
	}

}


