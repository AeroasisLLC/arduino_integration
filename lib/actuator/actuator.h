/*!
   \file "actuator.h"
   \brief "Header file for actuator"
   \author "Spandan"
   \date "16"/"Oct"/"2018"
*/

#ifndef actuator_h
#define actuator_h

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

class actuator{
/*!
   \brief "class function for actuator"
   \param "Param description"
   \pre "Pre-conditions"
   \post "Post-conditions"
   \return "Return of the function"
*/
/*!< "lights: 133
      fans: 143
      pump_mixing: 58
      pump_pour: 99" */

uint8_t lights = 133;
uint8_t fans = 143;
uint8_t pump_mixing = 58;
uint8_t pump_pour = 99;


public:
  void set_status(uint8_t actuator_id, uint8_t state, uint8_t pin);
  void get_status(uint8_t actuator_id, uint8_t state, uint8_t pin);

};
#endif
