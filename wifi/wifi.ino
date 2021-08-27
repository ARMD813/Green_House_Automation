#include <ESP8266WiFi.h>
//wifi config
const char ssid[]= "BSNL_AP";
const char password[]= "shihabudeen3192";
IPAddress ip;
void setup() {
  // initialise Serial
  Serial.begin(9600);
  delay(100);

  //Connect to Wifi
  Serial.printf("Connecting to Wifi %s ",ssid);
  //Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status()!= WL_CONNECTED){
    Serial.print (".");
    delay (500);
  }
  Serial.println();
  Serial.println("Connected");
    //printing IP Adress
  ip = WiFi.localIP();
  Serial.println(ip);
  Serial.println();
  //show that we are connected

}

void loop() {
  // put your main code here, to run repeatedly:

}
