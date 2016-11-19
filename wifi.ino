/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <ESP8266WiFi.h>
#include <String.h>
#include <SoftwareSerial.h>
#include "DHT.h"

#define DHTPIN 2     // GPIO 2 pin of ESP8266
#define DHTTYPE DHT22   // DHT 22  (AM2302)

//const char* ssid     = "CowOrKingSpaceByPuzl";
//const char* password = "digitalnomad";
const char* ssid     = "Basel_SPY";
const char* password = "booba883735050";

const char* host = "data.sparkfun.com";
const char* publicKey = "yA2oVwZQDmi83En7MGN7";
const char* privateKey = "4WpaAzoGKZC1WRvNwPpN";

DHT dht(DHTPIN, DHTTYPE,30); // 30 is for cpu clock of esp8266 80Mhz
SoftwareSerial metro(5,4);
void setup() {
  Serial.begin(9600);
  metro.begin(115200);
  dht.begin();
  
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("ESP8266 Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  String inputStr = "";
  float DHTtemp = 0;
  float DHThum = 0;
  float BMPpres = 0;
  delay(5000);
  if(metro.available()){
    inputStr = metro.readString();
    Serial.println();
    Serial.print("First 4 symbols: ");
    Serial.println(inputStr.substring(1,8));
    String DHTtemp_s = inputStr.substring(1, 10);
    DHTtemp = DHTtemp_s.toFloat();
    Serial.println(inputStr.substring(1, 10));
    String DHThum_s = inputStr.substring(13, 22);
    DHThum = DHThum_s.toFloat();
    Serial.println(inputStr.substring(13, 22));
    String BMPpres = inputStr.substring(25, 40);
    Serial.println(inputStr.substring(25, 40));
    BMPpres = BMPpres.toFloat();

  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/input/";
  url += publicKey;
  url += "?private_key=";
  url += privateKey;
  url += "&temp=";
  url += DHTtemp;
  url += "&hum=";
  url += DHThum;
  url += "&pres=";
  url += BMPpres;
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(10);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
  delay(600000); // Send data every 10 minutes
  }
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  /*float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  // Compute heat index
  // Must send in temp in Fahrenheit!
  float hi = dht.computeHeatIndex(f, h);

  Serial.print("Humidity: "); 
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: "); 
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hi);
  Serial.println(" *F");

  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/input/";
  url += publicKey;
  url += "?private_key=";
  url += privateKey;
  url += "&temp=";
  url += DHTtemp;
  url += "&hum=";
  url += DHThum;
  url += "&hidx=";
  url += BMPpres;
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(10);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
  delay(600000); // Send data every 10 minutes
  */
}
