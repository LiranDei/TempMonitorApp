/*
 * Clock.h
 *
 *  Created on: Dec 23, 2022
 *      Author: USER
 */

#ifndef INC_CLOCK_H_
#define INC_CLOCK_H_
#include <stdint.h>

class Clock{
private:
	uint8_t _hour;
	uint8_t _min;
	uint8_t _sec;
public:

	Clock()
	{
		_hour = 0;
		_min = 0;
		_sec = 0;
	}

	Clock(uint8_t hour, uint8_t min, uint8_t sec )
	{
		_hour = hour;
		_min = min;
		_sec = sec;
	}

	Clock(const Clock & t)
	{
		_hour = t._hour;
		_min = t._min;
		_sec = t._sec;
	}

	Clock & operator=(const Clock & src)
	{
		if(&src != this)
		{
			_hour = src._hour;
			_min = src._min;
			_sec = src._sec;
		}
		return *this;
	}

	Clock operator+=(const Clock & other);
	Clock operator++();
	void setNewTime(uint8_t hour, uint8_t min, uint8_t sec);
	uint8_t getSec();
	uint8_t getMin();
	uint8_t getHour();

};


#endif /* INC_CLOCK_H_ */
