/*
 * Dht.cpp
 *
 *  Created on: Nov 17, 2022
 *      Author: student
 */

#include "Dht.h"
#include "MonitorManager.h"
#include <stdio.h>
#include "cmsis_os.h"


bool Dht :: waitWhileEqual(int value, int expectedTime)
{
	// set a timeout 20% bigger than the expected time
	uint32_t timeout = expectedTime + (expectedTime) / 5;

	__HAL_TIM_SET_COUNTER(_timer, 0);
	while (HAL_GPIO_ReadPin(_gpioPort, _gpioPin) == value) {
		if (__HAL_TIM_GET_COUNTER(_timer) > timeout) {
			// too much time in the state
			return false;
		}
	}

	return true;
}

void Dht :: setGpioOutput()
{
	GPIO_InitTypeDef gpioStruct = {0};

	gpioStruct.Pin = _gpioPin;
	gpioStruct.Mode = GPIO_MODE_OUTPUT_PP;
	gpioStruct.Pull = GPIO_NOPULL;
	gpioStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(_gpioPort, &gpioStruct);
	HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
}

void Dht :: setGpioInput()
{
	GPIO_InitTypeDef gpioStruct = {0};

	gpioStruct.Pin = _gpioPin;
	gpioStruct.Mode = GPIO_MODE_INPUT;
	gpioStruct.Pull = GPIO_NOPULL;
	gpioStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(_gpioPort, &gpioStruct);
	HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
}

int Dht :: dhtRead()
{
	setGpioOutput();

	HAL_TIM_Base_Start(_timer);
	__HAL_TIM_SET_COUNTER(_timer, 0);

	// switch the sensor on by putting the line in '0'
	HAL_GPIO_WritePin(_gpioPort, _gpioPin,(GPIO_PinState) 0);
	osDelay(DHT_DELAY);


	HAL_GPIO_WritePin(_gpioPort, _gpioPin, (GPIO_PinState)1);

	// start listening
	setGpioInput();

	// wait for response 20-40us
	if (!waitWhileEqual( 1, 40)) {
		LOG_ERROR("DHT timeout in FILE: %s LINE: %d\r\n", __FILE__, __LINE__);
		return HAL_ERROR;
	}

	// DHT should keep in low for 80us
	if (!waitWhileEqual( 0, 80)) {
		LOG_ERROR("DHT timeout in FILE: %s LINE: %d\r\n", __FILE__, __LINE__);
		return HAL_ERROR;
	}

	// DHT should keep in high for 80us
	if (!waitWhileEqual( 1, 80)) {
		LOG_ERROR("DHT timeout in FILE: %s LINE: %d\r\n", __FILE__, __LINE__);
		return HAL_ERROR;
	}

	// DHT start send data bits

	uint8_t data[5] = { 0 };
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 8; j++) {

			// DHT keeps in low for 50us before each bit
			if (!waitWhileEqual( 0, 60)) {
				LOG_ERROR("DHT timeout in FILE: %s LINE: %d\r\n", __FILE__, __LINE__);
				return HAL_ERROR;
			}

			// DHT sends bit keeping line in high maximum 70us
			if (!waitWhileEqual( 1, 70)) {
				LOG_ERROR("DHT timeout in FILE: %s LINE: %d\r\n", __FILE__, __LINE__);
				return HAL_ERROR;
			}

			data[i] <<= 1;

			// 26-28us for '0' and 70us for '1', so 50us is a good split time
			if (__HAL_TIM_GET_COUNTER(_timer) > 50) {
				data[i] |= 1;
			}
		}
	}

	uint8_t checksum = data[0] + data[1] + data[2] + data[3];
	if (checksum != data[4]) {
		LOG_ERROR("wrong cheakSum's dht in FILE: %s LINE: %d\r\n", __FILE__, __LINE__);
		return HAL_ERROR;
	}

	_temperature = (double)data[2] + ((double)data[3]) / 10;

	// wait while DHT return to high
	if (!waitWhileEqual(0, 100)) {
		LOG_ERROR("DHT timeout in FILE: %s LINE: %d\r\n", __FILE__, __LINE__);
		return HAL_ERROR;
	}

	HAL_TIM_Base_Stop(_timer);

	return HAL_OK;

}

double Dht :: getTemp()
{
	return _temperature;
}
