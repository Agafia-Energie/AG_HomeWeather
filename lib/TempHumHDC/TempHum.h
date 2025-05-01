#ifndef __TEMPHUM__H__
#define __TEMPHUM__H__

enum HDC_PARAM {
	HDC_TEMP = 0,
	HDC_HUM
};

void TempHum_Init();
void TempHum_Loop();
float TempHum_Process(HDC_PARAM hdc);

#endif  //!__TEMPHUM__H__