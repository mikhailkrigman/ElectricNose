/*
  Simple my_rtc for Arduino Zero and MKR1000

  Demonstrates the use of the my_rtc library for the Arduino Zero and MKR1000

  This example code is in the public domain

  http://arduino.cc/en/Tutorial/Simplemy_rtc

  created by Arturo Guadalupi <a.guadalupi@arduino.cc>
  15 Jun 2015
  modified
  18 Feb 2016
  modified by Andrea Richetta <a.richetta@arduino.cc>
  24 Aug 2016
*/

#include <RTCZero.h>
#include <SPI.h>
#include <SD.h>
#include <PID_v1.h>
#include <GyverPID.h>


#define PIN_STANDBY_BUTTON 1

#define PIN_Rref A3
#define PIN_Rsens A2
#define PIN_R0 A4
#define PIN_LOAD A5

#define PIN_DAC A0
#define PIN_AIN A1

const double R_ref = 10.0 * pow(10, 3); // 10 kOhm

//     Input,    Output,     Setpoint
double R_heater, PID_output, R_heater_goal;

double control_voltage; // always in units!


//PID my_PID(&R_heater, &PID_output, &R_heater_goal, Kp, Ki, Kd, DIRECT);
//GyverPID pid2(Kp, Ki, Kd);

const int chipSelect = SDCARD_SS_PIN;

/* Create an my_rtc object */
RTCZero my_rtc;

/* Change these values to set the current initial time */
const byte seconds = 00;
const byte minutes = 1;
const byte hours = 11;

/* Change these values to set the current initial date */
const byte day = 4;
const byte month = 3;
const byte year = 24;

void setup_rtc(RTCZero& rtc)
{
  // Set the time
  rtc.setHours(hours);
  rtc.setMinutes(minutes);
  rtc.setSeconds(seconds);

  // Set the date
  rtc.setDay(day);
  rtc.setMonth(month);
  rtc.setYear(year);

  // you can use also
  //rtc.setTime(hours, minutes, seconds);
  //rtc.setDate(day, month, year);
}

void setup_SD()
{
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
}

bool save_on_SD(String filename, String dataString)
{
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open(filename, FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    //Serial.println(dataString);

    return true;
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
    return false;
  }
}

void charge_capacitor(){
  // set high impedance for R0, Rref and Rsens
  pinMode(PIN_R0, INPUT);
  pinMode(PIN_Rref, INPUT);
  pinMode(PIN_Rsens, INPUT);
  
  // load capacitor with pin load
  pinMode(PIN_LOAD, OUTPUT);
  digitalWrite(PIN_LOAD, HIGH);

  unsigned long start_charging = millis();
  unsigned long end_charging = millis();
  while(end_charging - start_charging < 1000){
    compute_control_voltage();
    end_charging = millis();
  }
}

unsigned long discharge_capacitor(int PIN_DISCHARGE){
  /*  Discharge capacitor using resistance on PIN_R0 and PIN_DISCHARGE
      Return the discharging time
  */

  // PIN_DISCHARGE - pin, where is the discharging resistance
  // Capacitor will be discharged always by R0 and discharge resistor
  pinMode(PIN_LOAD, INPUT);
  pinMode(PIN_DISCHARGE, OUTPUT);
  digitalWrite(PIN_DISCHARGE, LOW);

  unsigned long begin_discharging = micros();
  while(digitalRead(PIN_R0) != LOW){
    compute_control_voltage();
  } // wait until capacitor will be discharged
  unsigned long end_discharging = micros();

  return end_discharging - begin_discharging;
}

double timeToDigigtal(int PIN_CALCULATE, int PIN_REFERENCE){
  /*  Calculate resistance on PIN_CALCULATE with 
      Time-To-Digital method using 
      PIN_REFERENCE as reference resistance 
  */ 
  Serial.println("Start Time To Digital");

  // input == Grosse Impedanz
  charge_capacitor();
  unsigned long ref_discharging_time = discharge_capacitor(PIN_REFERENCE);
  
  charge_capacitor();
  unsigned long calc_discharging_time = discharge_capacitor(PIN_CALCULATE);

  double R_calc = R_ref * ((1.0 * calc_discharging_time) / ref_discharging_time);

  return R_calc;
}

void go_stanbyMode(RTCZero& rtc)
{
  pinMode(PIN_STANDBY_BUTTON, INPUT);
  digitalWrite(LED_BUILTIN, LOW);
  rtc.standbyMode();
}

void setup_standbyButton(){
  pinMode(PIN_STANDBY_BUTTON, INPUT_PULLUP); //Abschaltung
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); //Arduino bereit
}


void setup_control_unit(){
  R_heater_goal = 110; // set setpoint for PID-controller

  control_voltage = v2unit(1.0);
  
  analogWriteResolution(10);
  analogWrite(PIN_DAC, control_voltage); // start with 1V
}

double U_heater;
double I_heater;
double Kp = 30, Ki = 1, integralSum = 0;
void compute_control_voltage(){
  U_heater = 2.0 * unit2v(analogRead(PIN_AIN)); // 2*U_AIN
  I_heater = (1.0 * unit2v(control_voltage) / (2.0 * 33.0));

  R_heater = (I_heater - 1e-4 > 0) ? U_heater / I_heater : 83;

  double error = R_heater_goal - R_heater;
  integralSum = constrain(integralSum + double(error) * Ki, 1, 1023);
  PID_output = constrain(Kp * error + integralSum, 1, 1023);

  control_voltage = PID_output; 
  analogWrite(PIN_DAC, control_voltage); 
}
unsigned long duration;
void setup()
{
  Serial.begin(9600);

  setup_control_unit();

  setup_standbyButton();  

  setup_SD();

  my_rtc.begin();
  setup_rtc(my_rtc);
  duration = millis();

}

String readString;
void loop()
{

  while (Serial.available()) {
    delay(2);  //delay to allow byte to arrive in input buffer
    char c = Serial.read();
    readString += c;
  }

  if (readString.length() >0) {
    Serial.print("I've become:");
    

    R_heater_goal = readString.toInt();
    Serial.println(R_heater_goal);

    readString="";
  } 
  if (digitalRead(PIN_STANDBY_BUTTON) == LOW)
  {
    go_stanbyMode(my_rtc);
  }

  // Print date and time...
  //printRTCDate(my_rtc);

  //String current_time = RTCDate2string(my_rtc);
  //Serial.println(current_time);
  //save_on_SD("time_log.txt", current_time);

  

  compute_control_voltage();

  double R_sens = timeToDigigtal(PIN_Rsens, PIN_Rref);
  save_on_SD("r_sensor.txt", String(R_sens));
  Serial.print("R_sens: ");
  Serial.println(R_sens);

  unsigned long now = millis();
  if (now - duration > 1000){
    Serial.print("Heater voltage: ");
    Serial.println(U_heater, 3);
    Serial.print("Heater current: ");
    Serial.println(I_heater, 3);
    Serial.print("Heater resistance: ");
    Serial.println(R_heater, 3);
    Serial.print("PID output: ");
    Serial.println(PID_output);

    duration = millis();
  }
  //Serial.print("R Sensor: ");
  //Serial.println(R_sens);
}


String RTCDate2string(RTCZero rtc){
  String time;

  time += num2string(rtc.getDay()) + "/";
  time += num2string(rtc.getMonth()) + "/";
  time += num2string(rtc.getYear()) + " ";
  time += num2string(rtc.getHours()) + ":";
  time += num2string(rtc.getMinutes()) + ":";
  time += num2string(rtc.getSeconds());

  return time;
}

String num2string(int num){
  String num_str = (num < 10) ? "0" : "";
  num_str += num;
  return num_str;
}

void printRTCDate(RTCZero rtc){
  print2digits(rtc.getDay());
  Serial.print("/");
  print2digits(rtc.getMonth());
  Serial.print("/");
  print2digits(rtc.getYear());
  Serial.print(" ");

  // ...and time
  print2digits(rtc.getHours());
  Serial.print(":");
  print2digits(rtc.getMinutes());
  Serial.print(":");
  print2digits(rtc.getSeconds());

  Serial.println();
}

void print2digits(int number) {
  char num_char = '0' + number;
  if (number < 10) {
    Serial.print("0"); // print a 0 before if the number is < than 10
  }
  Serial.print(number);
}

int v2unit(double v){
  return (int)(v / 3.3 * 1023);
}

double unit2v(int unit){
  return (3.3 * ((1.0 * unit) / 1023));
}
