/*
 * Rtc.h
 *
 *  Created on: Nov 17, 2022
 *      Author: student
 */

#ifndef INC_RTC_H_
#define INC_RTC_H_

#include "main.h"
#include "DateTime.h"

class Rtc
{
private:
	I2C_HandleTypeDef * _hi2c;
	uint8_t _devAddr;

public:

	Rtc (I2C_HandleTypeDef * hi2c, uint8_t devAddr)
	{
		_hi2c = hi2c;
		_devAddr = devAddr;
	}

	void rtcGetTime(DateTime * dateTime);
	HAL_StatusTypeDef rtcSetTime(DateTime * dateTime);
	int bcdToInt(uint8_t bcd);
	int intToBcd(int value, int minVal, int maxVal);

};

#endif /* INC_RTC_H_ */
