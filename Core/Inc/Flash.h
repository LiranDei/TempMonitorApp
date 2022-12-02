/*
 * Flash.h
 *
 *  Created on: 19 בנוב׳ 2022
 *      Author: USER
 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_
#include "main.h"

#define DATA_IN_USED 0xDE
#define DEFAULT_TEMP 50

enum SOUND_STATE{SOUND_ON, SOUND_CANCELD};
enum TEMP_STATE{NORMAL, WARNING, CRITICAL};

struct THRESHOLDS
{
	uint16_t dataUsed;
	SOUND_STATE soundCanceled;
	TEMP_STATE state;
	int spare;
	double warning;
	double critical;
};

class Flash
{
private:
	uint32_t _bank;
	uint32_t _pageAddr;
	uint32_t _nbPages;
	uint32_t _typeProgram;
	THRESHOLDS _thresholds;
public:
	Flash(uint32_t bank, uint32_t pageAddr, uint32_t nbPage, uint32_t typeProgram)
	{
		_bank = bank;
		_pageAddr = pageAddr;
		_nbPages = nbPage;
		_typeProgram = typeProgram;
		_thresholds = {0};
	}

	HAL_StatusTypeDef erasePage();
	HAL_StatusTypeDef writeToPage(uint8_t* data, int dataSize);
	uint32_t getBank(){return _bank;}
	uint32_t getPageAddr(){return _pageAddr;}
	uint32_t getNbPages(){return _nbPages;}
	uint32_t getTypeProgram(){return _typeProgram;}
	double getWarning();
	double getCritical();
	void printThresHolds();
	void setWarning(double warning);
	void setCritical(double critical);
	SOUND_STATE getSoundCanceled();
	void setSoundCanceled(SOUND_STATE state);
	TEMP_STATE getTempState();
	void setTempState(TEMP_STATE state);
	void initThresHolds();

};


#endif /* INC_FLASH_H_ */
