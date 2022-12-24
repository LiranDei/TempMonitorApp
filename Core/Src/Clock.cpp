/*
 * Qlock.cpp
 *
 *  Created on: Dec 23, 2022
 *      Author: USER
 */

#include "Clock.h"

#define HOURS_IN_DAY 24
#define MINS_IN_HOUR 60
#define SECS_IN_MIN 60
#define SEC_IN_DAY 86400

uint8_t Clock :: getSec()
{
	return _sec;
}

uint8_t Clock :: getMin()
{
	return _min;
}

uint8_t Clock :: getHour()
{
	return _hour;
}

Clock Clock :: operator+=(const Clock & other)
{

	int secInOther = (other._hour *3600) + (other._min * 60) + other._sec;
	int secInThis = (_hour *3600) + (_min * 60) + _sec;
	int sumSec = (secInOther + secInThis) % SEC_IN_DAY;
	_hour = (sumSec/3600) % HOURS_IN_DAY;
	_min = (sumSec/60) % MINS_IN_HOUR;
	_sec = sumSec % SECS_IN_MIN;
	return *this;
}

Clock Clock :: operator++()
{

	*this += Clock(0,0,1);
	return *this;
}

void Clock :: setNewTime(uint8_t hour, uint8_t min, uint8_t sec)
{
	_hour = hour;
	_min = min;
	_sec = sec;
}
