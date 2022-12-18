/*
 * DateTime.h
 *
 *  Created on: Nov 17, 2022
 *      Author: student
 */

#ifndef INC_DATETIME_H_
#define INC_DATETIME_H_

struct DateTime
{
	uint8_t sec;
	uint8_t min;
	uint8_t hours;
	uint8_t weekDay;
	uint8_t day;
	uint8_t month;
	uint8_t year;
	uint8_t spare;

};

#endif /* INC_DATETIME_H_ */
