#include <SoftwareSerial.h>
const int sensor_pin = A0;  /* Soil moisture sensor O/P pin */

void setup() {
  Serial.begin(9600);

}

void loop() {
  float moisture_percentage= 0;
  int sensor_analog;
  float mp = 0;
  for  (int i = 0; i<=500; i++){
  sensor_analog = analogRead(sensor_pin);
  moisture_percentage =  ( 100 - ( (sensor_analog/1023.00) * 100 ) );
  mp = mp+ moisture_percentage;
  }
  mp = mp/500;
  Serial.println(mp);

  delay(1000);
}
