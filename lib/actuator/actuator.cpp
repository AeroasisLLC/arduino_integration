/*!
   \file actuator.cpp
   \brief "to get and set the state of actuators"
   \author "Spandan"
   \date "16"/"Oct"/"2018"
*/

#include <actuator.h>

void actuator::set_status(uint8_t actuator_id, uint8_t state, uint8_t pin){
  /*!
     \brief "set state of the actuator"
     \param "actuator_id: actuator uid for recognition
            state: state of the actuator
            pin: the actuator connection pin"
     \pre "the process would call the function to change actuator state"
     \post "actuator would change state"
     \return "Return of the function"
  */

  // digitalWrite state of actuator
  if (actuator_id == lights)
    digitalWrite(pin, state);
  else if (actuator_id == fans)
    digitalWrite(pin, state);
  else if (actuator_id == pump_mixing)
    digitalWrite(pin, state);
  else if (actuator_id == pump_pour)
    digitalWrite(pin, state);
}
