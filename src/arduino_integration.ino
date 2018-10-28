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
 #include<waterlevel.h>
 #include<ph.h>
 #include<ec.h>
 #include<actuator.h>

 // sensor pins
 #define DHT11_PIN A3
 #define WLS_sensorpin A2
 #define TDS_sensorpin A0
 #define PHS_sensorpin A1
 #define trig_pin
 #define echo_pin
 #define DS188B20_pin 12;
// actuator pins
// input
#define Led_pin_in
#define Fans_pin_in
#define Pump_mixing_pin_in
#define Pump_pour_pin_in
// output
#define Led_pin_out
#define Fans_pin_out
#define Pump_mixing_pin_out
#define Pump_pour_pin_out

//create objects of sensor data classes
waterlevel WaterLevel;
ph Ph;
ec Ec;
actuator Actuator;
dht DHT;

// initialize Interrupt variable
const int BUTTON =2;
// Serial data variables
boolean newData = false;
const byte numChars = 32;
char serial_data[numChars];

// initialize SensorData variables as struct
struct {
  float WaterLevel;
  float Ph;
  float EC;
  int Temperature;
  int Humidity;
}sensor_data;

// state variables for actuators
struct{
  bool LED_state, FAN_state, PUMP_mixing_state, PUMP_pour_state;
}actuator_state;

void setup() {
  // Serial connection at 115200
  Serial.begin(15200);

  // initialize Interrupt as HIGH
  //pinMode(BUTTON, INPUT);
  digitalWrite(BUTTON, HIGH);

  //initialize pinMode
  initialize_pins();
  // initialize Interrupt
  attachInterrupt (0, data_transceive_interrupt, LOW);

  // initialize sensor data & actuator variables
  initialize_variables();
}

void data_transceive_interrupt() {
  cli(); // disable Interrupt

  // send Sensor Data to Raspberry Pi
  // Serial.print("SEND REQ");
  Serial.print("AE6C88FE");
  Serial.print("\t\t");
  Serial.print(sensor_data.Temperature);
  Serial.print("\t\t");
  Serial.print(sensor_data.Humidity);
  Serial.print("\t\t");
  Serial.print(sensor_data.WaterLevel);
  Serial.print("\t\t");
  Serial.print(sensor_data.Ph);
  Serial.print("\t\t");
  Serial.println(sensor_data.EC);
sei();  // enable global Interrupt

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

  sensor_data.EC = 0.0;
  sensor_data.Ph = 0.0;
  sensor_data.WaterLevel = 0.0;
  sensor_data.Temperature = 0;
  sensor_data.Humidity = 0;

  // initialize actuator states
  actuator_state.FAN_state = false;
  actuator_state.LED_state = false;
  actuator_state.PUMP_mixing_state = false;
  actuator_state.PUMP_pour_state = false;
}// end of initialize_variables

void initialize_pins(){
  // initialize sensor pins as INPUT
  pinMode(WLS_sensorpin, INPUT);
  pinMode(PHS_sensorpin, INPUT);
  pinMode(TDS_sensorpin, INPUT);

  // initialize input pins Raspberry Pi -> Arduino
  pinMode(Led_pin_in, INPUT);
  pinMode(Fans_pin_in, INPUT);
  pinMode(Pump_pour_pin_in, INPUT);
  pinMode(Pump_mixing_pin_in, INPUT);

  // initialize output pins Arduino -> Actuators
  pinMode(Led_pin_out, OUTPUT);
  pinMode(Fans_pin_out, OUTPUT);
  pinMode(Pump_pour_pin_out, OUTPUT);
  pinMode(Pump_mixing_pin_out, OUTPUT);
}


void read_sensors() {
  /*!
     \brief "Read SensorData and send it to Raspi"
     \param "AE6C88FE: data packet signature
            sensor_data.temperature: temperature data
            sensor_data.humidity: humidity data
            sensor_data.ec: ec data
            sensor_data.ph: ph data
            sensor_data.waterlevel: waterlevel data"
     \pre "arduino receives SENSOR DATA as input on Interrupt"
     \post "arduino reads and sends the SensorData to Raspi"
     \return "Return of the function"
  */
  /*!< THS_ok: start the temp/Humidity sensor */
  int THS_ok = 0;

  // read temperature data
  THS_ok = DHT.read11(DHT11_PIN);
  if (!THS_ok)
    sensor_data.Temperature = DHT.temperature;
  else
    sensor_data.Temperature = -1;

  // read Humidity data
  if (!THS_ok)
    sensor_data.Humidity = DHT.humidity;
  else
    sensor_data.Humidity = -1;

  // read ph data
  sensor_data.Ph = Ph.read();
  // read ec data
  sensor_data.EC = Ec.read();
  // read water level data
  sensor_data.WaterLevel = WaterLevel.read();
  

}


void read_actuator_command() {
  /*!
     \brief "read actuator Commands"
     \param "actuator_state.FAN_state: state of fan
            actuator_state.LED_state: state of LED
            actuator_state.PUMP_pour_state: state of pouring pump
            actuator_state.PUMP_mixing_state: state of mixing pump"
     \pre "Raspi sends request for actuator reading"
     \post "the actuators are operated as per received input"
     \return "no Return"
  */

  actuator_state.LED_state = digitalRead(Led_pin_in);
  actuator_state.FAN_state = digitalRead(Fans_pin_in);
  actuator_state.PUMP_mixing_state = digitalRead(Pump_mixing_pin_in);
  actuator_state.PUMP_pour_state = digitalRead(Pump_pour_pin_in);

  // execute actuator Commands
  execute_actuator_command();

}

void execute_actuator_command(){
/*!
   \brief "execute actuator commands as read before"
   \pre "The actuator readings are taken"
   \post "the actuators are performing as per Commands"
   \return "No Return"
*/
digitalWrite(Led_pin_out, actuator_state.LED_state);
digitalWrite(Fans_pin_out, actuator_state.FAN_state);
digitalWrite(Pump_mixing_pin_out, actuator_state.PUMP_mixing_state);
digitaWrite(Pump_pour_pin_out, actuator_state.PUMP_pour_state);
}

void loop() {
  read_sensors();
  read_actuator_command();
  execute_actuator_command();
  delay(200);
}
