/*!
   \file "arduino_integration"
   \brief "Integrating Raspi and Arduino
          Send Sensor data Arduino -> Raspi
          Receive Actuator Commands Raspi -> Arduino
          Execute Actuator Commands"
   \author "Spandan"
*/
/**  DHT11_PIN - Temp/Humidity sensor pin
  WLS - Water Level Sensor
  THS - Temperature n Humidity Sensor
  TDS - EC Sensor
  PHS - pH Sensor

  av - analog value
  dv - digital value*/

 #include<dht.h>
 #include<string.h>
 #include<Arduino.h>
 #define DHT11_PIN A3
 #define WLS_sensorpin A2
 #define TDS_sensorpin A0
 #define PHS_sensorpin A1

// initialize Interrupt variable
const int BUTTON =2;
dht DHT;             // initialize DHT variable

// Serial data variables
boolean newData = false;
const byte numChars = 32;
char serial_data[numChars];

// initialize SensorData variables as struct
struct s_data{
  float WaterLevel;
  float Ph;
  float EC;
  int Temperature;
  int Humidity;
};

// initialize Actuators variables as struct
struct{
  int LEDpin_in, FANpin_in, PUMPpin_in;
  int LEDpin_out, FANpin_out, PUMPpin_out;
}actuator_pin;

struct{
  bool LED_state, FAN_state, PUMP_state;
}actuator_var;

void setup() {
  // Serial connection at 115200
  Serial.begin(15200);

  // Serial output "AEROASIS LOG"
  while (!Serial);
  Serial.println("\t \t AEROASIS LOG");
  Serial.println("Temperature(C) \tHumidity(%) \tWater Level \tpH \tTurbidity");

  // initialize Interrupt as HIGH
  //pinMode(BUTTON, INPUT);
  digitalWrite(BUTTON, HIGH);

  // initialize sensor pins as INPUT
  pinMode(WLS_sensorpin, INPUT);
  pinMode(PHS_sensorpin, INPUT);
  pinMode(TDS_sensorpin, INPUT);

  // initialize input pins Raspberry Pi -> Arduino
  pinMode(actuator_var.LEDpin_in, INPUT);
  pinMode(actuator_var.FANpin_in, INPUT);

  // initialize output pins Arduino -> Actuators
  pinMode(actuator_var.LEDpin_out, OUTPUT);
  pinMode(actuator_var.FANpin_out, OUTPUT);

  // initialize Interrupt
  attachInterrupt (0, data_transceive_interrupt, LOW);

  // initialize sensor data & actuator variables
  initialize_variables();
}

void data_transceive_interrupt() {
  cli(); // disable Interrupt

  // send "SEND REQ" to Raspberry Pi
  Serial.print("SEND REQ");

  // Receive data from Raspberry Pi
  do {
    receive_from_rasp();
  } while(newData == false);


  if (serial_data == "SENSOR DATA"){
    read_sensors();
  }
  else if(serial_data == "ACTUATOR"){
    read_actuator_command();
  }


}// end of data_transceive_interrupt


char receive_from_rasp() {
  // Function to receive Serial Data from Raspberry Pi
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;
  newData = false;

  // reading Serial data
  while (Serial.available() > 0 && newData == false) {
      rc = Serial.read();

      if (rc != endMarker) {
          serial_data[ndx] = rc;
          ndx++;
          if (ndx >= numChars) {
              ndx = numChars - 1;
          }// end of if numChars
      }// end of if endMarker

      else {
          serial_data[ndx] = '\0'; // terminate the string
          ndx = 0;
          newData = true;
        }// end of else
    }// end of while

}// end of receive_from_rasp

void initialize_variables() {
  /*!
     \brief "initialize variables"
     \param "actuator_var struct, sensor_data struct, actuator_pin struct"
     \pre "during system start"
     \post "variables and pins are initialized"
     \return "No return"
  */
  actuator_pin.FANpin_in = 3;
  actuator_pin.LEDpin_in = 5;
  actuator_pin.LEDpin_out = 8;
  actuator_pin.FANpin_out = 6;
  sensor_data.EC = 0.0;
  sensor_data.Ph = 0.0;
  sensor_data.WaterLevel = 0.0;
  sensor_data.Temperature = 0;
  sensor_data.Humidity = 0;
  actuator_var.FAN_state = false
  actuator_var.LED_state = false
  actuator_var.PUMP_state = false
}// end of initialize_variables

void read_sensors() {
  /*!
     \brief "Read SensorData"
     \param "Param description"
     \pre "arduino receives SENSOR DATA as input on Interrupt"
     \post "arduino reads and sends the SensorData to Raspi"
     \return "Return of the function"
  */

}

void read_actuator_command() {
  /* code */
}

void execute_actuator_command(){

}

void sort(float sensorValue[])
{
  /*!
     \brief "Sort the array"
     \param "tempi: temporary variable, sensorValue[]: received array"
     \pre "arduino reads 10 sensor values in succession"
     \post "sorts the array and sends to the main function for further action"
     \return "No return"
  */
  float tempi=0.0;
  for (int i=0;i<9;i++){
    for(int j=i+1;j<10;j++){
      if(sensorValue[i]>sensorValue[j]){
        tempi = sensorValue[i];
        sensorValue[i] = sensorValue[j];
        sensorValue[j] = tempi;
      }// end of if
      }// end of for_2
    }// end of for_1
}

void loop() {
  /* code */
}
