/*
 * Buzzer.h
 *
 *  Created on: Nov 17, 2022
 *      Author: student
 */

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

enum BUZZER_STATUS
{
	BUZZER_OFF,
	BUZZER_ON
};


class Buzzer
{
private:
	BUZZER_STATUS _status;
	TIM_HandleTypeDef* _pwmTimer;
	int _channel;

public:
	Buzzer(TIM_HandleTypeDef* timer, int channel)
	{
		_pwmTimer = timer;
		_channel = channel;
		_status = BUZZER_OFF;
	}

	void buzzerOn();
	void buzzerOff();
	BUZZER_STATUS getStatus();
};

#endif /* INC_BUZZER_H_ */
