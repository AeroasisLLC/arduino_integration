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


class waterlevel{

public:
  /** function to read waterlevel sensor*/
  float read(int trig_pin, int echo_pin);

};
#endif
