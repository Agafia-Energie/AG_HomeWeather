#ifndef __MICROSD__H__
#define __MICROSD__H__

#include <Arduino.h>


void SD_Init();
void SD_WriteHDC(uint32_t epoch, float temp, float humidity);
void SD_Format();
void SD_PrintFile();
#endif  //!__MICROSD__H__