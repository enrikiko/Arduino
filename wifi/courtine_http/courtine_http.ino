#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h> 

const int motorStepPin = 1;
const int motorDirPin = 2; //directions pin HIGH Down
const int motorStatus = 3;

int motorDelayTime=1;
const int totalCourtainSteps = 1000;
int courtainSteps = totalCourtainSteps;
int currentCourtinSteps;
bool useOTA = false;
bool certain;
String wifiName;

const char *ssid1 = "Cuarto2.4G";
const char *password1 = "Lunohas13steps";
const char *ssid2 = "WifiSalon";
const char *password2 = "lunohas13steps";
const String deviceName = "Wemo_courtain";
const char *deviceNameHost = "Wemo_courtain";

int port = 80;

ESP8266WiFiMulti WiFiMulti;
ESP8266WebServer server(port);


void setup() {
  Serial.begin(115200);
  // Sets the two pins as Outputs
  pinMode(motorStepPin,OUTPUT);
  pinMode(motorDirPin,OUTPUT);
  pinMode(motorStatus,OUTPUT);
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid1, password1);
  WiFiMulti.addAP(ssid2, password2);
  WiFi.begin();
  WiFi.hostname(deviceName);
    while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.println("Conecting to "+WiFi.SSID()+"...");
    delay(1000);
  }
  wifiName = WiFi.SSID();
  String ip = WiFi.localIP().toString();
  Serial.println("Wifi conected:  "+wifiName);
  Serial.println("IP:             "+ip);
  enrolleDevice(ip);
  server.on("/"+deviceName+"/status/true", handleRootOn);
  server.on("/"+deviceName+"/status/false", handleRootOff);
  server.on("/"+deviceName+"/status", handleStatus);
  server.on("/"+deviceName+"/ota/true", startOTA);
  server.on("/"+deviceName+"/ota/false", stopOTA);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  while(WiFiMulti.run() == WL_CONNECTED){
    if(useOTA == true) {
      ArduinoOTA.handle();
    }
    server.handleClient();
    checkCourtainSteps();
  }
  WiFi.begin();
  while (WiFiMulti.run() != WL_CONNECTED){
    delay(1000);
  }
  wifiName = WiFi.SSID();
}

void enrolleDevice(String ip){
  boolean certain = false;
  while(!certain){
    if ((WiFiMulti.run() == WL_CONNECTED)) {
      WiFiClient client;
      HTTPClient http;
      if (http.begin(client, "http://back.app.cortijodemazas.com/device/"+deviceName+"/true/"+ip+":"+port)) {
        Serial.print("[HTTP] POST CODE: ");
        int httpCode = http.POST("");
        Serial.println(httpCode);
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

void handleRootOn() {
  certain=true;
  changeStatus(true);
  server.send(200, "application/json", "{\"status\": true}");
}

void handleRootOff() {
  certain=false;
  changeStatus(false);
  server.send(200, "application/json", "{\"status\": false}");
}

void handleStatus() {
  String state;
  if(courtainSteps==0){state="false";}
  else if(courtainSteps==totalCourtainSteps){state="true";}
  else{state=String(courtainSteps);}
  server.send(200, "application/json", "{\"status\":" + state + ",\"SSID\":\"" + wifiName + "\",\"SIGNAL\":" + WiFi.RSSI() + ",\"OTA\":" + useOTA + "}");
}

void changeStatus(boolean val){
  if(val){
    courtainSteps=totalCourtainSteps;
  }else{
    courtainSteps=0;;
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

void stopOTA(){

  server.send(200, "application/json", "{\"OTA\": false}");
  useOTA = false;

}

void startOTA(){
  //
  server.send(200, "application/json", "{\"OTA\": true}");
  delay(1000);
  ArduinoOTA.setHostname(deviceNameHost);
  ArduinoOTA.setPassword(deviceNameHost);
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    //Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    useOTA = false;
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {});
  ArduinoOTA.onError([](ota_error_t error) {
    if (error == OTA_AUTH_ERROR) {
    } else if (error == OTA_BEGIN_ERROR) {
    } else if (error == OTA_CONNECT_ERROR) {
    } else if (error == OTA_RECEIVE_ERROR) {
    } else if (error == OTA_END_ERROR) {
    }
  });
  ArduinoOTA.begin();
  useOTA = true;
}

void checkCourtainSteps(){
  while (currentCourtinSteps != courtainSteps) {
    if (currentCourtinSteps >> courtainSteps){
      downSteps();
    }else if (currentCourtinSteps << courtainSteps){
      upSteps();
    }
  }
  stopMotor();
}

void downSteps() {
  digitalWrite(motorStatus,HIGH);
  digitalWrite(motorDirPin,HIGH);
  digitalWrite(motorStepPin,HIGH);
  delay(motorDelayTime);
  digitalWrite(motorStepPin,LOW);
}

void upSteps() {
  digitalWrite(motorStatus,HIGH);
  digitalWrite(motorDirPin,LOW);
  digitalWrite(motorStepPin,HIGH);
  delay(motorDelayTime);
  digitalWrite(motorStepPin,LOW);
}

void stopMotor() {
  digitalWrite(motorStatus,LOW);
}
