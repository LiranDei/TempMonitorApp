/*
 * Led.h
 *
 *  Created on: 20 Nov 2022
 *      Author: student
 */

#ifndef INC_LED_H_
#define INC_LED_H_
#define DEFAULT_PERIOD 500

#include "main.h"

enum LedState
{
	OFF,
	ON,
	BLINK
};

class Led
{
public:
	virtual void ledOn() = 0;
	virtual void ledOff() = 0;
	virtual LedState getState() = 0;
	virtual void setState(LedState state) = 0;
	virtual void setBrightness(int ) = 0;
};

class LedGpio : public Led
{
private:
	GPIO_TypeDef * _GPIOx;
	uint32_t _Pin;
	LedState _ledState;
	int _period;
	int _counter;
public:
	LedGpio(GPIO_TypeDef* GPIOx, uint32_t Pin)
	{
		_GPIOx = GPIOx;
		_Pin = Pin;
		_ledState = OFF;
		_period = DEFAULT_PERIOD;
		_counter = 0;
	}

	void ledOn();
	void ledOff();
	void toggle();
	void timerFunc();
	LedState getState();
	void setState(LedState state);
	void setBrightness(int );
	void ledBlink();
	int getPeriod();

};

#endif /* INC_LED_H_ */
