/*!
   \file "ph.h"
   \brief "Header file for ph"
   \author "Spandan"
   \date "16"/"Oct"/"2018"
*/

#ifndef ph_h
#define ph_h

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

class ph{
/*!
   \brief "class function for "
   \param "Param description"
   \pre "Pre-conditions"
   \post "Post-conditions"
   \return "Return of the function"
*/
public:
  float read(uint8_t pin);

private:
  void  sort(float sensorValue[]);
};
#endif
