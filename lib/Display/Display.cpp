
#include "U8g2lib.h" 
#include <TempHum.h>
 #include <TimeLib.h>

//-----------------------------------------------------------------------------------------------
// Define the Oled pin
#define OLED_SDA PB9
#define OLED_SCL PB8
 
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, OLED_SCL, OLED_SDA);

char weekDayStr[7][10] =  {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"}; // Weekday names

void Display_Init()
{
    u8g2.setColorIndex(1);  // set the color to white
    u8g2.begin();
    u8g2.setBitmapMode(1);   
    u8g2.setFont(u8g2_font_NokiaLargeBold_tf);
}

void Display_ClearBuffer()
  {
    u8g2.clearBuffer();
  }
  
void Display_UpdateScreen()
{
    u8g2.sendBuffer();
}

void Display_String(uint8_t row, uint8_t col, String strValue)
{
    u8g2.drawStr(col, row, strValue.c_str());
}


void Display_Weather(uint8_t hour, uint8_t minute, uint8_t second, uint8_t weekDay)
{ 
    bool isTime = true;

    static int timer = 0;
    timer++;

    Serial.print("\t Timer: ");
    Serial.print(timer);
    
    Display_ClearBuffer();

    //Get weekday
    String weekDayStrValue = String(weekDayStr[weekDay-1]);
    
    // Draw the Main Title
    u8g2.setFont(u8g2_font_12x6LED_tf);
    Display_String(10, 10, weekDayStrValue);

    char buffer[20];   
    // // Display the formatted time with leading zeros
    sprintf(buffer, "%02d:%02d:%02d", hour, minute, second);

    // String timeStr = String(hour) + ":" + String(minute) + ":" + String(second);
    // // Display the time on the screen
    u8g2.setFont(u8g2_font_9x6LED_tf);
    Display_String(13, 80, String(buffer));
    
    // Draw the weather icon
    u8g2.setFont(u8g2_font_open_iconic_weather_2x_t);
    u8g2.drawGlyph(15, 35, 65);	
 
    // Print the temperature
    u8g2.setFont(u8g2_font_logisoso20_tf);
    Display_String(43, 35, String(TempHum_Process(HDC_TEMP),1));

    // Draw the degree icon
    u8g2.setFont(u8g2_font_m2icon_7_tf);
    u8g2.drawGlyph(90, 30, 81);	
     
    // Print the degree Celsius
    u8g2.setFont(u8g2_font_12x6LED_tf);
    Display_String(38, 99, "C");
    
    // Print the humidity
    u8g2.setFont(u8g2_font_9x6LED_tf);
    Display_String(60, 10, "Humidity: " + String(TempHum_Process(HDC_HUM),1) + " %");

    // update the screen
    Display_UpdateScreen();
}
