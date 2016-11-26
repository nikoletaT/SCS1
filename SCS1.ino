#include <Wire.h>   //for I2C interface
#include <SPI.h>    //for SPI interface
#include <SoftwareSerial.h>
//for BMP280 Pressure and Temperature Sensor
#include <Adafruit_Sensor.h> 
#include <Adafruit_BMP280.h>

//for GPS
#include <TinyGPS++.h>

//for DHT22 Temperature and Humidity Sensor
#include "DHT.h"

#include <OneWire.h>
#include <DallasTemperature.h>

#define   MQ5_DPIN    6
#define   MQ5_APIN    A1
#define   MQ135_DPIN  7
#define   MQ135_APIN  A2
#define   DHT_PIN     2
#define   DHT_TYPE    DHT22
#define   SOIL_PIN    A3
#define ONE_WIRE_BUS 8

Adafruit_BMP280 bme;
DHT dht(DHT_PIN, DHT_TYPE);
TinyGPSPlus gps;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

SoftwareSerial ss(4, 3);
SoftwareSerial node(13,12);
String output = "";

void setup() {
  pinMode(MQ5_DPIN, INPUT);
  pinMode(MQ5_APIN, INPUT);
  pinMode(MQ135_DPIN, INPUT);
  pinMode(MQ135_APIN, INPUT);
  
  Serial.begin(57600); //for debugging
  ss.begin(9600);
  node.begin(9600);
  bme.begin();
  dht.begin();
  sensors.begin();
}

void loop() {
  output = "";
  
  output += String(readDHT22temp());
  output += ' ';
  output += String(readDHT22hum());
  output += ' ';
  output += String(readBMP280pres());
  output += ' ';
  output += String(getBatteryVoltage());
  output += ' ';
  output += String(getMQ5data());
  output += ' ';
  output += String(getMQ135data());
  output += ' ';
  output += String(getWaterTemp());
  output += ' ';
  output += String(getSoilMoisture());
  node.flush();
  node.println(output);
  
  Serial.println(output);

  while (ss.available() > 0)
    if (gps.encode(ss.read())){
      Serial.println("Location:");
      getLocationLat();
      Serial.print(",");
      getLocationLng();
      delay(100);
      Serial.println("Data/Time:");
      getDateMonth();
      Serial.print("/");
      getDateDay();
      Serial.print("/");
      getDateYear();
      Serial.print(" ");
      getTimeHours();
      Serial.print(":");
      getTimeMins();
      Serial.print(":");
      getTimeSecs();
      Serial.println();
      //delay(100);
    }
    
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println(F("No GPS detected")); 
  }
  

  Serial.println();
  delay(10000);  
}

/*
 * Function that process the data received from BMP280 Sensor.
 * Input:
 * 
 * Return value:
 */
float readBMP280temp(){
  float BMP280temp = 0;
  BMP280temp = bme.readTemperature();
  delay(2000);
  return BMP280temp;
}

float readBMP280pres(){
  float BMP280pres = 0;
  BMP280pres = bme.readPressure();
  return BMP280pres;
}

/*
 * Function that process the data received from DHT22 Sensor.
 * Input: NONE
 * 
 * Return value: DHT22hum - float; the humidity value received from the sensor
 */
float readDHT22hum(){
  float DHT22hum = 0;
  //Reading temperaure or humidity takes about 250 ms
  DHT22hum = dht.readHumidity();

  if(isnan(DHT22hum)){
    Serial.println("Failed to read from DHT sensor!");
    return -1;
  }

  return DHT22hum;
}
/*
 * Function that process the data received from DHT22 Sensor.
 * Input: NONE
 * 
 * Return value: DHT22temp - float; the temperature value received from the sensor
 */
float readDHT22temp(){
  float DHT22temp = 0; 

  DHT22temp = dht.readTemperature();

  if(isnan(DHT22temp)){
    Serial.println("Failed to read from DHT sensor!");
    return -1;
  }

  return DHT22temp;
}
/*
 * Function that receives GPS location
 * Input: NONE
 * 
 * Return value: GPSlat - long; the latitude value received from the GPS module
 */
long getLocationLat(){
  long GPSlat = 0;
  if(gps.location.isValid()){
    GPSlat = gps.location.lat();
    Serial.print(GPSlat, 6);
  }
  else
    Serial.print(F("INVALID"));
  return GPSlat;
}
/*
 * Function that receives GPS location
 * Input: NONE
 * 
 * Return value: GPSlng - long; the longitude value received from the GPS module
 */
long getLocationLng(){
  long GPSlng = 0;
  if(gps.location.isValid()){
    GPSlng = gps.location.lng();
    Serial.print(F(","));
    Serial.print(GPSlng,6);
  }
  else
    Serial.print(F("INVALID"));
  return GPSlng;
}

/*
 * Function that receives GPS date
 * Input: NONE
 * 
 * Return value: GPSdd - int; the day value received from the GPS module
 */
int getDateDay(){
  int GPSdd = 0;
  if(gps.date.isValid()){
    GPSdd = gps.date.day();  
    Serial.print(GPSdd);
  }
  else
    Serial.print(F("INVALID"));
  return GPSdd;
}
int getDateMonth(){
  int GPSmm = 0;
  if(gps.date.isValid()){
    GPSmm = gps.date.month();
    Serial.print(GPSmm);
  }
  else
    Serial.print(F("INVALID"));
  return GPSmm;
}
int getDateYear(){
  int GPSyy = 0;
  if(gps.date.isValid()){
    GPSyy = gps.date.year();
    Serial.print(GPSyy);
  }
  else
    Serial.print(F("INVALID"));
  return GPSyy;
}
/*
 * Function that receives GPS time
 * Input:
 * 
 * Return value:
 */
int getTimeSecs(){
  int GPSss = 0;
  if (gps.time.isValid()){
    GPSss = gps.time.second();
    if (GPSss < 10) 
      Serial.print(F("0"));
    Serial.print(GPSss);
  }
  else
    Serial.print(F("INVALID"));
  return GPSss;
}
int getTimeMins(){
  int GPSmins = 0;
  if (gps.time.isValid()){
    GPSmins = gps.time.minute();
    if (GPSmins < 10)   
      Serial.print(F("0"));  
    Serial.print(GPSmins);
  }
  else
    Serial.print(F("INVALID"));
  return GPSmins;
}
int getTimeHours(){
  int GPShh = 0;
  if (gps.time.isValid()){
    GPShh = gps.time.hour();
    if (GPShh < 10)      
      Serial.print(F("0"));
    Serial.print(GPShh);
  }
  else
    Serial.print(F("INVALID"));
  return GPShh;
}
float getBatteryVoltage(){
  int valueBattery = analogRead(A0);

  return ((valueBattery*(5.0 / 1023.0))*(2.54+1));
}
int getMQ5data(){
  int ad_value = 0;
  ad_value = analogRead(MQ5_APIN);

  return ad_value;
}
int getMQ135data(){
  int ad_value = -1;
  ad_value = analogRead(MQ135_APIN);

  return ad_value;
}
float getWaterTemp(){
  float value = -1;
  sensors.requestTemperatures();
  value = sensors.getTempCByIndex(0);

  return value;
}
int getSoilMoisture(){
  int value = -1;
  value = analogRead(SOIL_PIN);

  return value;
}

