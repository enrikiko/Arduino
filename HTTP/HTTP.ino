#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char *ssidNew = "WifiEnrique";
const char *passwordNew = "password";
const char* yourSSID = "VilaVeronika1"; 
const char* yourPASS = "Julinka12"; 

String url = "http://us-central1-sim808-gps.cloudfunctions.net/v1/set/";
String first = "null";
String second = "null";

int led = 2;

ESP8266WebServer server(80);

String setURL(String url, String a, String b){
  String URL = url + a + "/" + b;
  return URL;
  }
  
void http(){
  WiFi.begin(yourSSID, yourPASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
    Serial.println("Waiting for connection");
  }
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(setURL(url, first, second));
    Serial.println("HTTP sent");
    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload);
    }
    http.end();
  }
 }

void setWifi(){
  WiFi.softAP(ssidNew, passwordNew);
  IPAddress myIP = WiFi.softAPIP();
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.on("/404", handleException);
  server.on("/enrique", handleEnrique);
  server.on("/html", handleHtml);
  server.begin();
  }


void blind() {
  digitalWrite(led, LOW);   
  delay(200);                       
  digitalWrite(led, HIGH);
}
 
void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
  WiFi.softAP(ssidNew, passwordNew);
  IPAddress myIP = WiFi.softAPIP();
  setWifi();
  server.send(200, "text/html", "<h1>Wellcome to wemos</h1>");
}

void loop() {
 
 server.handleClient();
}
