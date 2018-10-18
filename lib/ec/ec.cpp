/*!
   \file "ec.cpp"
   \brief "EC custom library for easy integration"
   \author "Spandan Samiran"
   \date "16"/"Oct"/"2018"
*/

#include<ec.h>

double read(uint8_t ec_pin, uint8_t temp_pin){
  /*!
     \brief "read EC values"
     \param "input: ec_pin - ec sensor pin
                    temp_pin - Temperature sensor pin"
     \pre "any function or process calls for ec values"
     \post "the external process receives EC values"
     \return "ec_value:double"
  */


}

void sort(float sensorValue[]){
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
