#include <Arduino.h>
 
/*
 * Based on Blink_AnalogRead example from: https://github.com/feilipu/Arduino_FreeRTOS_Library
 * Modified by: Frederic Pillon <frederic.pillon (at) st.com>
 */
#include <STM32FreeRTOS.h>
#include <STM32RTC.h>
#include <MicroSD.h>
#include <TempHum.h>
#include "NoDelay.h"
#include <Display.h>


/* Get the rtc object */
STM32RTC& rtc = STM32RTC::getInstance();

/* Change these values to set the current initial time */
const byte seconds = 0;
const byte minutes = 0;
const byte hours = 16;

/* Change these values to set the current initial date */
/* Monday 15th June 2015 */
const byte weekDay = 1;
const byte day = 15;
const byte month = 6;
const byte year = 15;

noDelay dataLogTimer(2000);

// define two tasks for Blink & AnalogRead
void TaskBlink( void *pvParameters );
void TaskDisplay(void *pvParameters);
void TaskDataLogger( void *pvParameters );
void RTC_Setup();
// the setup function runs once when you press reset or power the board
void setup() {

  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  }
  RTC_Setup();

  // Now set up two tasks to run independently.
  xTaskCreate(
    TaskBlink
    ,  (const portCHAR *)"Blink"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  xTaskCreate(
    TaskDisplay
    ,  (const portCHAR *) "Display"
    ,  512  // Stack size, increase it if mcu crashes or freeze.
    ,  NULL
    ,  2  // Priority
    ,  NULL );

  xTaskCreate(
    TaskDataLogger
    ,  (const portCHAR *) "HDCLog"
    ,  512  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );

  // start scheduler
  vTaskStartScheduler();
  Serial.println("Insufficient RAM");
  while(1);
}

void loop()
{
  // Empty. Things are done in Tasks.

  // for demonstration purpose, check in loop
  if(Serial.available() > 0){
    // read the incoming command
    char command = Serial.read();
    // check if the command is "set"
      if(command == 's'){  
      // read the incoming epoch time
      String epochTime = Serial.readStringUntil('\n');
      // convert to long
      long epochTimeLong = epochTime.toInt();
      //check if the epoch time is valid
      if(epochTimeLong < 10000000000 && epochTimeLong > 0){
        Serial.print("\nEpoch time: ");
        Serial.println(epochTimeLong);
        rtc.setEpoch(epochTimeLong);
      } else{
        Serial.println("\nInvalid epoch time. Please enter a valid epoch time.");
      }
    }
    else if(command == 'f'){
      Serial.println("\nFormatting SD card...");
      // format the SD card
      SD_Format(); 
    }else if(command == 'r'){
      Serial.println("\nRead SD card...");
      // format the SD card
      SD_PrintFile(); 
    } 
    else{
      Serial.println("\nInvalid command. Please enter a valid command.");
    }
  }
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskBlink(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
 
  // initialize digital LED_BUILTIN on pin 13 as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  for (;;) // A Task shall never return or exit.
  {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second 
  }
}

void TaskDisplay(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
 
  // initialize digital LED_BUILTIN on pin 13 as an output.
  Display_Init();
  Display_ClearBuffer();
  Display_String(20, 10, "Home Weather");
  Display_UpdateScreen();
 
  for (;;) // A Task shall never return or exit.
  { 
    Display_Weather(rtc.getHours(), rtc.getMinutes(), rtc.getSeconds(), rtc.getWeekDay());
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for 2.5 second 
  }
}

void TaskDataLogger(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  long lastMillis = 0;

  SD_Init();
  TempHum_Init(); 

  for (;;)
  { 
   
    TempHum_Loop(); // read the temperature and humidity
    
    SD_WriteHDC(rtc.getEpoch(), TempHum_Process(HDC_TEMP),TempHum_Process(HDC_HUM)); 
   

    vTaskDelay( 5000 / portTICK_PERIOD_MS ); // wait for one second 
  }
}


void RTC_Setup()
{
  rtc.setClockSource(STM32RTC::LSI_CLOCK); // or use LSI_CLOCK if no external crystal is used

  rtc.begin(); // initialize RTC 24H format

  // Set the time
  rtc.setHours(hours);
  rtc.setMinutes(minutes);
  rtc.setSeconds(seconds);

  // Set the date
  rtc.setWeekDay(weekDay);
  rtc.setDay(day);
  rtc.setMonth(month);
  rtc.setYear(year);

  rtc.setEpoch(1745992510); // set the epoch time
}