/*!
   \file "ph.cpp"
   \brief "EC custom library for easy integration"
   \author "Spandan Samiran"
   \date "16"/"Oct"/"2018"
*/

#include<ph.h>

float ph::read(uint8_t pin){
  /*!
     \brief "read Ph values"
     \param "pin: ph sensor pin"
     \pre "any function or process calls for ph values"
     \post "the external process receives PH values"
     \return "pH:double"
  */
  /*!< PHS_bits: read 10 ph values */
  float PH_bits[10];
  /*!< PH_avg: average of 6 ph values */
  float PH_avg=0;

  // clear buffer
  for (int i=0;i<10;i++) PH_bits[i] = 0;

  // read 10 ph analog values
  for (int i=0;i<10;i++){
    PH_bits[i] = analogRead(pin);
    delay(100);
   }
  // sort the ph buffer
  sort(PH_bits);
  // sum of 10 ph values
  for (int i=2;i<8;i++){
    PH_avg += PH_bits[i];
  }
  // average of ph values
  float pH = float(PH_avg *5.0/1024/6) * 3.5;
  return pH;
}

void ph::sort(float sensorValue[]){
  /*!
     \brief "sort the array received"
     \param "tempi: temporary variable"
     \pre "function calls sort"
     \post "the array is sorted and accessible"
     \return "no Return"
  */
  float tempi = 0.0;
  for (int i=0; i<9; i++){
    for (int j=1; j<i+1; j++){
      if (sensorValue[i]>sensorValue[j]){
        tempi = sensorValue[i];
        sensorValue[i] = sensorValue[j];
        sensorValue[j] = tempi;
      }// end if
    }// end of for_2
  }// end of for_1

}
