#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include "DHT.h"
#define DHTPIN 6
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Wifi_Control";
const char* password = "secure_app";
String serverName = "http://api.thingspeak.com";
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;
String serverPath = "";

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {



  float h = dht.readHumidity();
  float t = dht.readTemperature();
  String temp = String(t);
  String hum = String(h);
  Serial.println("+++++++++++++++");
  Serial.println(temp);
  Serial.println(hum);

  serverPath = serverName + "/update?api_key=7NRFT3XQPNM10KMC&field1=";
  serverPath += temp;
  serverPath += "&field2=";
  serverPath += hum;
  //serverPath += "\r\n";
  Serial.println(serverPath);

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Your Domain name with URL path or IP address with path
    http.begin(serverPath.c_str());

    // Send HTTP GET request
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  delay(25000);
}
