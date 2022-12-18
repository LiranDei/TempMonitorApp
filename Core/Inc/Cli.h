/*
 * Cli.h
 *
 *  Created on: 19 בנוב׳ 2022
 *      Author: USER
 */

#ifndef INC_CLI_H_
#define INC_CLI_H_

#define COMMAND_ARR_SIZE 20
#define COMMAND_NAME_SIZE 20

#include "Flash.h"
#include "SdCard.h"
#include "Rtc.h"
#include "Dht.h"
#include <string.h>

class ICommand
{
private:
	char _commandName[COMMAND_NAME_SIZE];

public:
	ICommand(const char* name)
	{
		strcpy(_commandName, name);
	}

	virtual void doCommand(char* param) = 0;
	virtual ~ICommand(){}
	const char* getCommandName() const {return _commandName;}
};

//class SetFlashWarning : public ICommand
//{
//private:
//	Flash* _flash;
//public:
//	SetFlashWarning(const char* name, Flash* flash) : ICommand(name), _flash(flash){}
//	void doCommand(char* param);
//};
//
//class SetFlashCritical : public ICommand
//{
//private:
//	Flash* _flash;
//public:
//	SetFlashCritical(const char* name, Flash* flash) : ICommand(name), _flash(flash){}
//	void doCommand(char* param);
//};
//
//class GetFlashInfo : public ICommand
//{
//private:
//	Flash* _flash;
//public:
//	GetFlashInfo(const char* name, Flash* flash) : ICommand(name), _flash(flash){}
//	void doCommand(char* param);
//};
//
//class SetRtcTime : public ICommand
//{
//private:
//	Rtc* _rtc;
//public:
//	SetRtcTime(const char* name, Rtc* rtc) : ICommand(name), _rtc(rtc){}
//	void doCommand(char* param);
//};
//
//class GetRtcTime : public ICommand
//{
//private:
//	Rtc* _rtc;
//public:
//	GetRtcTime(const char* name, Rtc* rtc) : ICommand(name), _rtc(rtc){}
//	void doCommand(char* param);
//};
//
//class ClearLog : public ICommand
//{
//private:
//	SdCard* _sdCard;
//public:
//	ClearLog(const char* name, SdCard* sdCard) : ICommand(name), _sdCard(sdCard){}
//	void doCommand(char* param);
//};
//
//class PrintLog : public ICommand
//{
//private:
//	SdCard* _sdCard;
//public:
//	PrintLog(const char* name, SdCard* sdCard) : ICommand(name), _sdCard(sdCard){}
//	void doCommand(char* param);
//};


class Container
{
private:
	int _counter = 0;
	ICommand* _iCommand[COMMAND_ARR_SIZE];
public:
	void add(ICommand* icommand);
	int handle (const char* name, char* param);
	void registerCommand();
	~Container()
	{
		for(int i = 0; i < _counter; i++)
		{
			delete(_iCommand[i]);
		}
	}
};
#endif /* INC_CLI_H_ */
