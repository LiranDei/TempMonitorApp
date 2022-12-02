/*
 * Led.cpp
 *
 *  Created on: 20 Nov 2022
 *      Author: student
 */


#include "Led.h"

void LedGpio :: toggle()
{
	HAL_GPIO_TogglePin(_GPIOx, _Pin);
}

void LedGpio :: ledOn()
{
	HAL_GPIO_WritePin(_GPIOx, _Pin, (GPIO_PinState)1);
	_ledState = ON;
}

void LedGpio :: ledOff()
{
	HAL_GPIO_WritePin(_GPIOx, _Pin, (GPIO_PinState)0);
	_ledState = OFF;
}

void LedGpio :: ledBlink()
{
	_ledState = BLINK;
	toggle();
}


LedState LedGpio :: getState()
{
	return _ledState;
}

void LedGpio :: setState(LedState state)
{
	_ledState = state;
}

void LedGpio :: setBrightness(int brightness)
{

}

int LedGpio :: getPeriod()
{
	return _period;
}
