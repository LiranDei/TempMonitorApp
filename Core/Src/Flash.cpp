/*
 * Flash.cpp
 *
 *  Created on: 19 בנוב׳ 2022
 *      Author: USER
 */


#include "Flash.h"
#include "MonitorManager.h"
#include <stdio.h>
#include <string.h>


HAL_StatusTypeDef Flash :: erasePage()
{
	HAL_FLASH_Unlock();
	FLASH_EraseInitTypeDef flashErase;
	flashErase.TypeErase = FLASH_TYPEERASE_PAGES;
	flashErase.Banks = _bank;
	flashErase.Page = _pageAddr;
	flashErase.NbPages = _nbPages;

	uint32_t pageError;
	HAL_FLASHEx_Erase(&flashErase, &pageError);
	if(pageError == 0xFFFFFFFF)
	{
		HAL_FLASH_Lock();
		LOG_DEBUGGER("successfully erase page in FILE: %s LINE: %d\r\n", __FILE__, __LINE__);
		return HAL_OK;
	}
	else
	{
		HAL_FLASH_Lock();
		LOG_ERROR("error in erase page in FILE: %s LINE: %d\r\n", __FILE__, __LINE__);
		return HAL_ERROR;
	}

}

HAL_StatusTypeDef Flash :: writeToPage(uint8_t* data, int dataSize)
{
	HAL_StatusTypeDef status;
	HAL_FLASH_Unlock();
	int index = 0;
	while (index < dataSize)
	{
		uint64_t Data =*(uint64_t*)(data+index);
		status = HAL_FLASH_Program(_typeProgram, _pageAddr+index, Data);
		if(status != HAL_OK)
		{
			LOG_ERROR("error in write to page in FILE: %s LINE: %d error: %d\r\n", __FILE__, __LINE__, status);
			HAL_FLASH_Lock();
			return status;
		}

		index += sizeof(uint64_t);
	}
	LOG_DEBUGGER("successfully saved data in FILE: %s LINE: %d \r\n", __FILE__, __LINE__);
	LOG_RUN("successfully saved %s\r\n","");
	HAL_FLASH_Lock();
	return status;
}

double Flash :: getWarning()
{
	return _thresholds.warning;
}

double Flash :: getCritical()
{
	return _thresholds.critical;
}

void Flash :: setWarning(double warning)
{
	_thresholds.dataUsed = DATA_IN_USED;
	_thresholds.warning = warning;
	HAL_StatusTypeDef status;
	status = erasePage();
	if(status != HAL_OK)
	{
		LOG_ERROR("error in erase page in line %d in file %s\r\n", __LINE__, __FILE__);
	}
	writeToPage( (uint8_t*)&_thresholds, (sizeof(THRESHOLDS)));

}

void Flash :: setCritical(double critical)
{
	_thresholds.critical = critical;
	HAL_StatusTypeDef status;
	status = erasePage();
	if(status != HAL_OK)
	{
		LOG_ERROR("error in erase page in line %d in file %s\r\n", __LINE__, __FILE__);
	}
	writeToPage((uint8_t*) &_thresholds, (sizeof(THRESHOLDS)));

}

void Flash :: initThresHolds()
{
	THRESHOLDS* data = (THRESHOLDS*)(_pageAddr);
	if(data->dataUsed == DATA_IN_USED)// mean critical and warrning already init
	{
		memcpy(&_thresholds, data, sizeof(THRESHOLDS));
	}
	else
	{
		_thresholds.critical = DEFAULT_TEMP;
		_thresholds.warning = DEFAULT_TEMP;
		_thresholds.dataUsed = DATA_IN_USED;
		_thresholds.soundCanceled = SOUND_ON;
		_thresholds.state = NORMAL;
		writeToPage((uint8_t*)&_thresholds, sizeof(THRESHOLDS));
	}
}

void Flash :: printThresHolds()
{
	LOG_RUN("warning = %.2f, critical = %.2f\r\n",_thresholds.warning, _thresholds.critical);
}

SOUND_STATE Flash :: getSoundCanceled()
{
	return _thresholds.soundCanceled;
}

void Flash :: setSoundCanceled(SOUND_STATE state)
{
	_thresholds.soundCanceled = state;
}

TEMP_STATE Flash :: getTempState()
{
	return _thresholds.state;
}

void Flash :: setTempState(TEMP_STATE state)
{
	_thresholds.state = state;
}
