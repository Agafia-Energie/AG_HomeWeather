#ifndef __DISPLAY__H__
#define __DISPLAY__H__

#include <Arduino.h>

void Display_Init();
void Display_ClearBuffer();  
void Display_UpdateScreen();
void Display_Weather(uint8_t hour, uint8_t minute, uint8_t second, uint8_t weekDay);
void Display_String(uint8_t row, uint8_t col, String strValue);

#endif  //!__DISPLAY__H__