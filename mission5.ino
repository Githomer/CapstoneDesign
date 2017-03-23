#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS D4

const char* host = "api.thingspeak.com";
const int httpPort = 80;
const char* ssid = "KT_WLAN_8362";   
const char* password = "000000162a";

String temp_url = "/update?api_key=ROQYYP08K2QD3HIN";   // API Key - temperature


unsigned long mark = 0;
int interval = 60000;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

void connect_ap() 
{
    Serial.println();
    Serial.print("connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
    
    Serial.print("\n Got WiFi, IP address: ");
    Serial.println(WiFi.localIP());  
}

void delivering(String payload) 
{ 
    WiFiClient client;
  
    Serial.print("connecting to ");
    Serial.println(host);
  
    if (!client.connect(host, httpPort)) 
    {
      Serial.print("connection failed: ");
      Serial.println(payload);
    
      return;
    }
  
    String getheader = "GET "+ String(temp_url) +"&"+ String(payload) +" HTTP/1.1";
  
    client.println(getheader);
    client.println("User-Agent: ESP8266 Dohyung Kim");  
    client.println("Host: " + String(host));  
    client.println("Connection: close");  
    client.println();

    Serial.println(getheader);
  
    while (client.connected()) 
    {
      String line = client.readStringUntil('\n');
      
      Serial.println(line);
    }
  
    Serial.println("Done cycle.");
}

void setup() 
{
    Serial.begin(115200);
    connect_ap();
    Serial.println("#5 Temperature Sensor");
}

void loop() 
{
  if (millis() > mark ) 
  {
     mark = millis() + interval;    
     String payload = working_temp();
     delivering(payload);

     /* do something more */


     
  }
}

///////////////////////////////////////////////////////////
/* Woking Functions */

/* #1 : Indoor Temperature*/
String working_temp() 
{ 
    float temperature;

    DS18B20.requestTemperatures();
    temperature = DS18B20.getTempCByIndex(0);
    
    return(String("field1=")+String(temperature));
}

