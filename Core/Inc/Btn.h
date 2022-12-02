/*
 * Btn.h
 *
 *  Created on: Nov 17, 2022
 *      Author: student
 */

#ifndef INC_BTN_H_
#define INC_BTN_H_

#include "main.h"

class Btn
{
private:
	GPIO_TypeDef* _gpioPort;
	uint16_t _gpioPin;

public:
	Btn(GPIO_TypeDef* gpioPort, uint16_t gpioPin)
	{
		_gpioPort = gpioPort;
		_gpioPin = gpioPin;
	}

};
#endif /* INC_BTN_H_ */
