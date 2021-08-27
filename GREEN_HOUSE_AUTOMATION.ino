#include <ESP8266WiFi.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "ThingSpeak.h"
#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT22   // DHT 11
#define  DHTPin_1 0
//#define  DHTPin_2 3
DHT dht_1(DHTPin_1, DHTTYPE);
//DHT dht_2(DHTPin_2, DHTTYPE);

#define S0 5                            
#define S1 6                           
#define S2 7                             
#define S3 8                      
#define SIG A0 
int decimal = 2;                          // Decimal places of the sensor value outputs 
float Soil_sensor0;                            /* Assign the name "sensor0" as analog output value from Channel C0 */
float Soil_sensor1;                            /* Assign the name "sensor1" as analog output value from Channel C1 */
float Soil_sensor2;                            /* Assign the name "sensor2" as analog output value from Channel C2 */
float Soil_sensor3;                            /* Assign the name "sensor3" as analog output value from Channel C3 */

// WiFi config
const char ssid[] = "";      //  give Wifi name
const char password[] = "";         // Wifi password
//thingSpeak Config
const unsigned long channel_id =  ;  // Thingspeak Channel ID
const char write_api_key[] = "";            // GIve Thingspeak write key
WiFiClient client;                          //WiFi client object
LiquidCrystal_I2C lcd(0x3F,20,4);           // LCD adress check it and give
//const int moisture_pin = A0;                // Soil moisture sensor O/P pin
const int pump = 3;
const int fan = 4;

class DHTsensor {
  public:
   float temperature(DHT X){
      float t=0 ; 
      for  (int i = 0; i<=100; i++){
         t = X.readTemperature() +t ;
      }
      t = t/100;
      return (t);
   }
   float humidity(DHT Y){
      float h=0 ;
      for  (int i = 0; i<=100; i++){
         h = Y.readHumidity() +h ;
      }
      h = h/100;
      return(h);  
   }  
};

void setup() {
  Serial.begin(9600);
  lcd.init();                      // initialize the lcd 
  lcd.init();
  lcd.setCursor(4,0);
  lcd.print("GREEN HOUSE");
  lcd.setCursor(4,1);
  lcd.print("AUTOMATION &");
  lcd.setCursor(5,2);
  lcd.print("MONITORING");
  lcd.setCursor(5,3);
  lcd.print("USING IOT");
  lcd.backlight();
  // Connect to WiFi
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("   Connecting to     ");
  lcd.print(ssid);
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay(500);
    lcd.print(".");
  }
  delay(1500);
  lcd.clear();
  lcd.setCursor(4,1);         
  lcd.print("WiFi Connected!");    // Show that we are connected
  ThingSpeak.begin(client);        //Connect to ThingSpeak
  pinMode(DHTPin_1, INPUT);
 // pinMode(DHTPin_2, INPUT);
  dht_1.begin();
  //dht_2.begin();
  delay(700);
  Serial.setTimeout(2000);
  pinMode(pump,OUTPUT);
  pinMode(fan,OUTPUT);
  pinMode(S0,OUTPUT);                       /* Define digital signal pin as output to the Multiplexer pin SO */        
  pinMode(S1,OUTPUT);                       /* Define digital signal pin as output to the Multiplexer pin S1 */  
  pinMode(S2,OUTPUT);                       /* Define digital signal pin as output to the Multiplexer pin S2 */ 
  pinMode(S3,OUTPUT);                       /* Define digital signal pin as output to the Multiplexer pin S3 */  
  pinMode(SIG, INPUT);  
  delay(1000);
}

void loop() {
  lcd.clear();
  DHTsensor t1;
  DHTsensor h1;
  //DHTsensor t2;
  //DHTsensor h2;
  float temp_1 = t1.temperature(dht_1);
  float humd_1 = h1.humidity(dht_1); 
  //float temp_2 = t2.temperature(dht_2);
  //float humd_2 = h2.humidity(dht_2); 
  
  // Channel 0 (C0 pin - binary output 0,0,0,0)
  digitalWrite(S0,LOW); digitalWrite(S1,LOW); digitalWrite(S2,LOW); digitalWrite(S3,LOW);
  Soil_sensor0 = Soil_moisture(SIG); 
  Serial.print("S0=");
  Serial.println(Soil_sensor0);
  // Channel 1 (C1 pin - binary output 1,0,0,0)
  digitalWrite(S0,HIGH); digitalWrite(S1,LOW); digitalWrite(S2,LOW); digitalWrite(S3,LOW);
  Soil_sensor1 = Soil_moisture(SIG); 
  Serial.print("S1=");
  Serial.println(Soil_sensor1);
  // Channel 2 (C2 pin - binary output 0,1,0,0)
  digitalWrite(S0,LOW); digitalWrite(S1,HIGH); digitalWrite(S2,LOW); digitalWrite(S3,LOW);
  Soil_sensor2 = Soil_moisture(SIG);
  Serial.print("S2=");
  Serial.println(Soil_sensor2);
  // Channel 3 (C3 pin - binary output 1,1,0,0)
  digitalWrite(S0,HIGH); digitalWrite(S1,HIGH); digitalWrite(S2,LOW); digitalWrite(S3,LOW);
  Soil_sensor3 = Soil_moisture(SIG);
  Serial.print("S3=");
  Serial.println(Soil_sensor0);
  
  float soilMstr = (Soil_sensor0 + Soil_sensor1 + Soil_sensor2 + Soil_sensor3)/4 ;
  if_else (30 , temp_1, fan);
  if_else (soilMstr, 10.5, pump);
  lcd.setCursor(0,0);
  lcd.print("Temperature :");
  lcd.setCursor(13,0);
  lcd.print(temp_1);
  lcd.setCursor(18,0);
  lcd.print(char(223));
  lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("Humidity     :");
  lcd.setCursor(14,1);
  lcd.print(humd_1);
  lcd.setCursor(19,1);
  lcd.print("%");
  lcd.setCursor(0,2);
  lcd.print("Soil moisture:");
  lcd.setCursor(14,2);
  lcd.print(soilMstr);
  lcd.setCursor(19,2);
  lcd.print("%");
  /*Serial.print("Humidity :");
  Serial.println(humd_1);
  Serial.print("Temperature :");
  Serial.println(temp_1);
  Serial.print("Soil Moisture :");
  Serial.println(soilMstr);*/
  ThingSpeak.setField(1, temp_1);
  ThingSpeak.setField(2, humd_1);
  ThingSpeak.setField(3, soilMstr);
  ThingSpeak.writeFields(channel_id, write_api_key);
  if (soilMstr < 10.5){
    delay(1000);
  }else{
    delay(1000);
  }
}
float Soil_moisture(const int pin){
  float moisture_percentage= 0;
  int sensor_analog;
  float mp = 0;
  for  (int i = 0; i<=100; i++){
  sensor_analog = analogRead(pin);
  moisture_percentage =  ( 100 - ( (sensor_analog/1023.00) * 100 ) );
  mp = mp+ moisture_percentage;
  delay(2);
  }
  mp = mp/100;
  return (mp);
  }
void if_else(float val_1, float val_2, int val_3){
  if (val_1 < val_2){
    digitalWrite(val_3, LOW);
  }else{
    digitalWrite(val_3, HIGH);
  }
}
