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

#define   MQ2_PIN     A1
#define   MQ4_PIN     A2
#define   MQ5_PIN     6
#define   MQ6_PIN     A3
#define   MQ135_PIN   5

#define   DHT_PIN     2
#define   DHT_TYPE    DHT22


Adafruit_BMP280 bme;
DHT dht(DHT_PIN, DHT_TYPE);
TinyGPSPlus gps;

SoftwareSerial ss(4, 3);

void setup() {
  Serial.begin(9600); //for debugging
  ss.begin(9600);
  
  bme.begin();
  dht.begin();
}

void loop() {
  Serial.println("Data from Sensor: BMP280");
  readBMP280temp();
  readBMP280pres();
  Serial.println("Data from Sensor: DHT22");
  readDHT22temp();
  readDHT22hum();

  while (ss.available() > 0)
    if (gps.encode(ss.read())){
      Serial.println("Location:");
      getLocationLat();
      Serial.print(",");
      getLocationLng();
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
    }
    
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println(F("No GPS detected: check wiring.")); 
  }
  
}

/*
 * Function that process the data received from BMP280 Sensor.
 * Input:
 * 
 * Return value:
 */
float readBMP280temp(){
  float BMP280temp = 0;
  Serial.print(F("/BMP280/ Temperature: "));
  BMP280temp = bme.readTemperature();
  Serial.print(BMP280temp);
  Serial.println(" *C");
  Serial.println();
  //delay(2000);
  return BMP280temp;
}

float readBMP280pres(){
  float BMP280pres = 0;
  Serial.print(F("/BMP280/ Pressure: "));
  BMP280pres = bme.readPressure();
  Serial.print(BMP280pres);
  Serial.println(" Pa");
  Serial.println();
  return BMP280pres;
}

/*
 * Function that process the data received from DHT22 Sensor.
 * Input:
 * 
 * Return value:
 */
float readDHT22hum(){
  float DHT22hum = 0;
  //Reading temperaure or humidity takes about 250 ms
  DHT22hum = dht.readHumidity();

  if(isnan(DHT22hum)){
    Serial.println("Failed to read from DHT sensor!");
    return -1;
  }
  Serial.print("/DHT22/ Humidity: ");
  Serial.print(DHT22hum);
  Serial.println();
  
}

float readDHT22temp(){
  float DHT22temp = 0; 

  DHT22temp = dht.readTemperature();

  if(isnan(DHT22temp)){
    Serial.println("Failed to read from DHT sensor!");
    return -1;
  }
  Serial.print("/DHT22/ Temperature: ");
  Serial.print(DHT22temp);
  Serial.println();
}
/*
 * Function that receives GPS location
 * Input:
 * 
 * Return value:
 */
int getLocationLat(){
  int GPSlat = 0;
  if(gps.location.isValid()){
    GPSlat = gps.location.lat();
    Serial.print(GPSlat, 6);
  }
  else
    Serial.print(F("INVALID"));
  return GPSlat;
}
int getLocationLng(){
  int GPSlng = 0;
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
 * Input:
 * 
 * Return value:
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
    Serial.print(GPShh);
  }
  else
    Serial.print(F("INVALID"));
  return GPShh;
}


