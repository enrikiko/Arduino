#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>

const char *ssid = "iPhone";
const char *password = "123443211";
const char *ssid2 = "VilaVeronika";
const char *password2 = "Julinka12";
String deviceName = "Enrique1";
int port = 80;
IPAddress ipDevice(10, 0, 0, 200);
IPAddress gateway(10, 0, 0, 138);
IPAddress subnet(255, 0, 0, 0);
String certain;

ESP8266WiFiMulti WiFiMulti;
ESP8266WebServer server(port);


void setup() {

  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);
  WiFiMulti.addAP(ssid2, password2);
  //WiFi.config(ipDevice, gateway, subnet);
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

  int pins[] = {16,5,4,0,2,14,12,13,15};

  for (int i = 0; i < sizeof(pins); i++){
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], HIGH);
  }

  setIp(ip);

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/set/multi", handleStatus);
  server.on("/info", handleInfo);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {

  server.handleClient();
  //MDNS.update();

}


void setIp(String ip){
  boolean certain = false;
  while(!certain){
    if ((WiFiMulti.run() == WL_CONNECTED)) {
      WiFiClient client;
      HTTPClient http;
      Serial.print("[HTTP] begin...\n");
      if (http.begin(client, "http://88.8.67.178:8000/new/"+deviceName+"/true/"+ip+":"+port)) {
        Serial.print("[HTTP] GET CODE: ");
        int httpCode = http.GET();
        if (httpCode > 0) {
          Serial.println(httpCode);
          if (httpCode == 200 ) {
            certain = true;
            Serial.print("[HTTP] GET BODY: ");
            Serial.println(http.getString());
          }
        } else {
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
      } else {
        Serial.printf("[HTTP} Unable to connect\n");
      }
    }
    delay(1000);
  }
}

void handleInfo() {
  server.send(200, "application/json", certain);
}


void handleStatus() {
  StaticJsonDocument<100> doc;
  DeserializationError error = deserializeJson(doc, server.arg(0));
  setPin(doc);
  certain=server.arg(0);
  server.send(200, "application/json", server.arg(0));
}

void setPin(StaticJsonDocument<100> doc){
  int pins[9] = {16,5,4,0,2,14,12,13,15};
  int tmpVal;
  for (int i = 0; i < 9; i++){
    tmpVal=doc[String(pins[i])];
    digitalWrite(pins[i], !tmpVal);
  }
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
