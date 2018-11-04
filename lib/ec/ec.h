/*!
   \file "ec.h"
   \brief "Header file for EC"
   \author "Spandan"
   \date "16"/"Oct"/"2018"
*/

#ifndef ec_h
#define ec_h

#if ARDUINO < 100
#include <WProgram.h>
#else
#include "Arduino.h"
#endif



class ec{

public:
  static const int numReadings = 20;
   //DS18B20 signal, pin on digital 2
  unsigned int readings[numReadings];      // the readings from the analog input
  int index = 0;                  // the index of the current reading
  unsigned long AnalogValueTotal = 0;                  // the running total
  unsigned int AnalogAverage = 0,averageVoltage=0;                // the average
  unsigned long AnalogSampleTime,printTime,tempSampleTime;
  float temperature,ECcurrent;

  ec();
  float read(uint8_t ec_pin, uint8_t temp_pin);

private:
  void  sort(float sensorValue[]);
  float TempProcess(bool ch);
  void clearBuffer();
};
#endif
