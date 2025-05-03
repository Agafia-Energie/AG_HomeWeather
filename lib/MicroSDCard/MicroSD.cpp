/*
  SD card read/write

  This example shows how to read and write data to and from an SD card file
  The circuit. Pin numbers reflect the default
  SPI pins for Uno and Nano models:
   SD card attached to SPI bus as follows:
 ** SDO - pin 11
 ** SDI - pin 12
 ** CLK - pin 13
 ** CS  - pin 10  (Chip Select for AGFIA SD card module on MikroBus 1)
  Note:
  If you have a shield, check the documentation for the correct CS pin.
  If you are using a module, check the documentation for the correct CS pin.

  This example code is in the public domain.
  http://www.arduino.cc/en/Tutorial/SDCard

*/
#include "Arduino.h"
#include <SD.h>
#include <TimeLib.h>

const char filename[] = "datalog.txt"; // File name to be created on SD card
const int chipSelect = PB0;
bool isSDCardPresent = false; // Flag to check if SD card is present
File myFile;

void SD_Init() {
  // Open serial communications and wait for port to open:
  
  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("1. is a card inserted?");
    Serial.println("2. is your wiring correct?");
    Serial.println("3. did you change the chipSelect pin to match your shield or module?");
    Serial.println("Note: press reset button on the board and reopen this Serial Monitor after fixing your issue!");
    while (true);
  }
  isSDCardPresent = true;

  Serial.println("initialization done.");

  /* -----------------------------------------------------------------------------
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("datalog.txt", FILE_WRITE);
  
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to datalog.txt...");
    myFile.println("testing 1, 2, 3. \n Writing more.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening datalog.txt");
  }
  
  // re-open the file for reading:
  myFile = SD.open("datalog.txt");
  if (myFile) {
    Serial.println("datalog.txt");
    
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.print("Error opening file ");
    Serial.println("datalog.txt");
  }
  --------------------------------------------------- */
}

void SD_WriteHDC(uint32_t epoch, float temp, float humidity)
{

  // Convert epoch time to human-readable format
  char buffer[20]; 
  // Convert unsigned long epoch to time_t
  time_t epochTime = (time_t)epoch;
 
  // Print the formatted string
  setTime(epochTime); // Set the time using the epoch time
  // Get the current time 
  sprintf(buffer, "%02d:%02d:%02d", hour(), minute(), second());
 
  // Print the formatted string
  Serial.print("\nTime: ");
  Serial.print(buffer); 
  Serial.print(" | Temp: ");
  Serial.print(temp);
  Serial.print(" | Humidity: ");
  Serial.print(humidity);

  
  // Check if SD card is present
  
  if(isSDCardPresent == false) {
    Serial.println("SD card not present. Cannot write data.");
    return;
  }
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("datalog.txt", FILE_WRITE);

  String  data = String(buffer) + " T:" + String(temp) + " RH:" + String(humidity) + "\n";
  myFile.println(data);
  myFile.close();
}
 
void SD_Format(){
  SD.end();
  SD.begin(chipSelect);
  SD.remove("datalog.txt");
  myFile = SD.open("datalog.txt", FILE_WRITE);
  myFile.println("Data Logger Initialized.");
  myFile.close();
}

void SD_PrintFile(){
  myFile = SD.open("datalog.txt");
  if (myFile) {
    Serial.print("\n--------------Reading datalog.txt -------------\n");
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
    } else {
      // if the file didn't open, print an error:
      Serial.print("Error opening file ");
      Serial.println("datalog.txt");
    }
    Serial.print("\n--------------End Reading datalog.txt -------------\n");
}