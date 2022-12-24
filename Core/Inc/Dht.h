/*
 * Dht.h
 *
 *  Created on: Nov 17, 2022
 *      Author: student
 */

#ifndef INC_DHT_H_
#define INC_DHT_H_

#include "main.h"

#define DHT_DELAY 19

class Dht
{
private:
	GPIO_TypeDef * _gpioPort;
	uint16_t _gpioPin;
	TIM_HandleTypeDef * _timer;
	uint8_t _data[5];
	double _temperature;
public:

	Dht(GPIO_TypeDef * gpioPort, uint16_t gpioPin, TIM_HandleTypeDef * timer)
	{
		_gpioPort = gpioPort;
		_gpioPin = gpioPin;
		_timer = timer;
		_temperature = 0.0;
	}

	bool waitWhileEqual (int value, int expectedTime);
	void setGpioOutput();
	void setGpioInput();
	int dhtRead();
	double getTemp();
};


#endif /* INC_DHT_H_ */
