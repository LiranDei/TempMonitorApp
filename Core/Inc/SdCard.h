/*
 * SdCard.h
 *
 *  Created on: Nov 27, 2022
 *      Author: student
 */

#ifndef INC_SDCARD_H_
#define INC_SDCARD_H_

#include <string.h>
#include "fatfs.h"


#define FILE_NAME_MAX_SIZE 50


class SdCard
{
private:
	char _fileName[FILE_NAME_MAX_SIZE];
	char _errorFileName[FILE_NAME_MAX_SIZE];
	FATFS _FatFs;
	FIL _fil;
	FRESULT _fres;
public:
	SdCard(const char* name, const char* errorName)
	{
		strcpy(_fileName, name);
		strcpy(_errorFileName, errorName);
	}

	char* getFileName();
	char* getErrorFileName();
	void write(char* fileName, char* str);
	void clear(char* fileName);
	void print(char* fileName);
	void mount();
	void deMount();
};

#endif /* INC_SDCARD_H_ */
