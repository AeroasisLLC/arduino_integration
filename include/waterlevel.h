/*!
   \file "waterlevel.h"
   \brief "Header file for waterlevel"
   \author "Spandan"
   \date "16"/"Oct"/"2018"
*/

#ifndef waterlevel_h
#define waterlevel_h

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

int full_length = 100;
class waterlevel{

public:
  /** function to read waterlevel sensor*/
  float read(uint8_t trig_pin, uint8_t echo_pin);

};
#endif
