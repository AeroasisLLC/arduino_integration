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
#include <Arduino.h>
#endif

class ec{

public:
/*!
   \brief "read ec sensor data"
   \param "pin: sensor pin"
   \pre "any process calls for read function"
   \post "the process receives ec values"
*/
  float read(uint8_t ec_pin, uint8_t temp_pin);

private:
  void  sort(float sensorValue[]);
};
#endif
