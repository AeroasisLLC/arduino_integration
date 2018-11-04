/*!
   \file "waterlevel.cpp"
   \brief "Read waterlevel values"
   \author "Spandan"
   \date "16"/"Oct"/"2018"
*/

#include<waterlevel.h>

float waterlevel::read(int trig_pin, int echo_pin){
  /*!
     \brief read waterlevel
     \param "input: trig_pin, echo_pin
             working param: duration, cm"
     \pre "process sends req to read waterlevel sensor"
     \post "process receives waterlevel sensor value"
     \return "percent: Return"
  */

  long duration, cm;
  int full_length = 100;
  float percent;
  // set the pinMode
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(5);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echo_pin, INPUT);
  duration = pulseIn(echo_pin, HIGH);

  // Convert the time into a distance
  cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343

  // convert to percent
  percent = ((cm - full_length)/full_length)*100;
  return percent;

}
