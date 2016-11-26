#include <ESP8266WiFi.h>
#include <String.h>
#include <SoftwareSerial.h>

const char* ssid     = "";
const char* password = "";

const char* host = "data.sparkfun.com";
const char* publicKey = "";
const char* privateKey = "";

int ind = 0;

SoftwareSerial metro(5,4);

void setup() {
  Serial.begin(9600);
  metro.begin(9600);

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
  ind = 0;
  String inputStr = "";
  float DHTtemp = 0;
  float DHThum = 0;
  float BMPpres = 0;
  float BattVolt = 0;
  int MQ5data = 0;
  int MQ135data = 0;
  delay(500);
  if(metro.available()){
    
    inputStr = metro.readString();
    metro.flush();
    Serial.println(inputStr);
    
    String DHTtemp_s = returnSubStr(inputStr, inputStr.length());
    String DHThum_s = returnSubStr(inputStr, inputStr.length());
    String BMPpres_s = returnSubStr(inputStr, inputStr.length());
    String BattVolt_s = returnSubStr(inputStr, inputStr.length());
    String MQ5data_s = returnSubStr(inputStr, inputStr.length());
    String MQ135data_s = returnSubStr(inputStr, inputStr.length());
    String WaterTemp_s = returnSubStr(inputStr, inputStr.length());
    String SoilHum_s = returnSubStr(inputStr, inputStr.length());

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
  url += DHTtemp_s;
  url += "&hum=";
  url += DHThum_s;
  url += "&pres=";
  url += BMPpres_s;
  url += "&volt=";
  url += BattVolt_s;
  url += "&mq5=";
  url += MQ5data_s;
  url += "&mq135=";
  url += MQ135data_s;
  url += "&tempw=";
  url += WaterTemp_s;
  url += "&hums=";
  url += SoilHum_s;
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
  
  delay(10000); // Send data every 10 minutes
  }
}
String returnSubStr(String str, int maxC){
  String subStr = "";
  
  for(int i = ind; i < maxC ; i++){
    if(str.charAt(i) != ' ')
      subStr += str.charAt(i);
    else {
      ind = i+1;
      break;
    }
  }
  return subStr;
}


