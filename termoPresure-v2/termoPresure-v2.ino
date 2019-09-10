#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

int port = 80;

float h;
float t;

const char *ssid1 = "Cuarto2.4G";
const char *password1 = "Lunohas13steps";
const char *ssid2 = "WifiSalon";
const char *password2 = "lunohas13steps";

IPAddress ipDevice(192, 168, 1, 120);
IPAddress dns(80, 58, 61, 250);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
String certain;

ESP8266WiFiMulti WiFiMulti;
ESP8266WebServer server(port);


void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid1, password1);
  WiFiMulti.addAP(ssid2, password2);
  //WiFi.config(ipDevice, dns, gateway, subnet);
  WiFi.begin();

  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  String ip = WiFi.localIP().toString();
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.print("IP:");
  Serial.println(ip);

  dht.begin();

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);
  getWeatherInfo();
  sendInfo();
  digitalWrite(LED_BUILTIN, HIGH);
  delay(3000);
  certain=true;
}

void getWeatherInfo(){
  h = dht.readHumidity();
  t = dht.readTemperature();
}

void sendInfo(){
 Serial.print(F("Humidity: "));
 Serial.print(h);
 Serial.print(F("%  Temperature: "));
 Serial.print(t);
 WiFiClient client;
 HTTPClient http;
 if (http.begin(client, "http://192.168.1.50:8000/set/"+String(t)+"/"+String(h))){
  int httpCode = http.GET();
  if (httpCode > 0) {
     Serial.println("HTTP request sent");};
  }
 }
