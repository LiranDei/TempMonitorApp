/*
 * Buzzer.cpp
 *
 *  Created on: Nov 17, 2022
 *      Author: student
 */

#include "main.h"
#include "Buzzer.h"
#include "MonitorManager.h"

void Buzzer :: buzzerOn()
{
	_status = BUZZER_ON;
	HAL_TIM_Base_Start(_pwmTimer);
	HAL_TIM_PWM_Start(_pwmTimer, _channel);
	__HAL_TIM_SET_AUTORELOAD(_pwmTimer, 150);
}

void Buzzer :: buzzerOff()
{
	_status = BUZZER_OFF;
	HAL_TIM_Base_Stop(_pwmTimer);
	HAL_TIM_PWM_Stop(_pwmTimer, _channel);
}

BUZZER_STATUS Buzzer :: getStatus()
{
	return _status;
}
