/*!
   \file "ec.cpp"
   \brief "EC custom library for easy integration"
   \author "Spandan Samiran"
   \date "16"/"Oct"/"2018"
*/

#include<ec.h>
#include <OneWire.h>

#define StartConvert 0
#define ReadTemperature 1
#define DS18B20_Pin A4
//Temperature chip i/o
OneWire ds(DS18B20_Pin);

 ec::ec(){
  clearBuffer();
  TempProcess(StartConvert);
}

float ec::read(uint8_t ec_pin, uint8_t temp_pin){
  /*!
     \brief "read EC values"
     \param "input: ec_pin - ec sensor pin
                    temp_pin - Temperature sensor pin"
     \pre "any function or process calls for ec values"
     \post "the external process receives EC values"
     \return "ec_value:double"
  */
    float return_value = 0.0;


    AnalogValueTotal = AnalogValueTotal - readings[index];
    // read from the sensor:
    readings[index] = analogRead(ec_pin);
    // add the reading to the total:
    AnalogValueTotal = AnalogValueTotal + readings[index];
    // advance to the next position in the array:
    index = index + 1;
    // if we're at the end of the array...
    if (index >= numReadings)
    // ...wrap around to the beginning:
    index = 0;
    // calculate the average:
    AnalogAverage = AnalogValueTotal / numReadings;

    temperature = TempProcess(ReadTemperature);  // read the current temperature from the  DS18B20
    // Serial.println(temperature);
    TempProcess(StartConvert);                   //after the reading,start the convert for next reading

    float TempCoefficient=1.0+0.0185*(temperature-25.0);    //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.0185*(fTP-25.0));
    float CoefficientVolatge=(float)averageVoltage/TempCoefficient;
    if(CoefficientVolatge<150)return_value = -1;   //25^C 1413us/cm<-->about 216mv  if the voltage(compensate)<150,that is <1ms/cm,out of the range
    else if(CoefficientVolatge>3300)return_value = -100;  //>20ms/cm,out of the range
    else
    {
      if(CoefficientVolatge<=448)ECcurrent=6.84*CoefficientVolatge-64.32;   //1ms/cm<EC<=3ms/cm
      else if(CoefficientVolatge<=1457)ECcurrent=6.98*CoefficientVolatge-127;  //3ms/cm<EC<=10ms/cm
      else ECcurrent=5.3*CoefficientVolatge+2278;                           //10ms/cm<EC<20ms/cm
      ECcurrent/=1000;    //convert us/cm to ms/cm
      return_value = ECcurrent;
    }


return return_value;
}

void ec::sort(float sensorValue[]){
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

void ec::clearBuffer(){
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;
}

float ec::TempProcess(bool ch)
{
  //returns the temperature from one DS18B20 in DEG Celsius
  static byte data[12];
  static byte addr[8];
  static float TemperatureSum;
  if(!ch){
          if ( !ds.search(addr)) {
              // Serial.println("no more sensors on chain, reset search!");
              ds.reset_search();
              return 0;
          }
          if ( OneWire::crc8( addr, 7) != addr[7]) {
              // Serial.println("CRC is not valid!");
              return 0;
          }
          if ( addr[0] != 0x10 && addr[0] != 0x28) {
              // Serial.print("Device is not recognized!");
              return 0;
          }
          ds.reset();
          ds.select(addr);
          ds.write(0x44,1); // start conversion, with parasite power on at the end
  }
  else{
          byte present = ds.reset();
          ds.select(addr);
          ds.write(0xBE); // Read Scratchpad
          for (int i = 0; i < 9; i++) { // we need 9 bytes
            data[i] = ds.read();
          }
          ds.reset_search();
          byte MSB = data[1];
          byte LSB = data[0];
          float tempRead = ((MSB << 8) | LSB); //using two's compliment
          TemperatureSum = tempRead / 16;
    }
          return TemperatureSum;
}
