/*
 * MonitorManager.h
 *
 *  Created on: Nov 17, 2022
 *      Author: student
 */

#ifndef INC_MONITORMANAGER_H_
#define INC_MONITORMANAGER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#define VERBOSITY_RUN 0
#define VERBOSITY_ERROR 1
#define VERBOSITY_DEBUGGER 2
#define VERBOSITY VERBOSITY_DEBUGGER

#if VERBOSITY >= VERBOSITY_RUN
#define LOG_RUN(FORMAT, ...) printf(FORMAT, ## __VA_ARGS__)
#else
#define LOG_RUN(FORMAT, ...)
#endif

#if VERBOSITY >= VERBOSITY_ERROR
#define LOG_ERROR(FORMAT, ...) printf(FORMAT,## __VA_ARGS__)
#else
#define LOG_ERROR(FORMAT, ...)
#endif

#if VERBOSITY >= VERBOSITY_DEBUGGER
#define LOG_DEBUGGER(FORMAT, ...) printf(FORMAT,## __VA_ARGS__)
#else
#define LOG_DEBUGGER(FORMAT, ...)
#endif

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void measureTemp(void *argument);
void monitorInit();
void monitorTask(void *argument);
void writeLog(void *argument);
void updateLogBuffer();
void accurateMeasureTime(void *argument);

#ifdef __cplusplus
}
#endif

#endif /* INC_MONITORMANAGER_H_ */
