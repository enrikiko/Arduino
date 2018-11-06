#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char *ssidNew = "WifiEnrique";
const char *passwordNew = "password";
const char* ssid = "VilaVeronika1"; 
const char* password = "Julinka12"; 
const char* url = "https://us-central1-sim808-gps.cloudfunctions.net/setposition/kike/1/2";

ESP8266WebServer server(80);

void handleRoot() {
server.send(200, "text/html", "<h1>You are connected</h1>");
Serial.println("HTTP request! Root");
}

void handleLoco() {
server.send(200, "text/html", "<h1>Que dice loco!</h1>");
Serial.println("HTTP request! Loco");
}

void setup() {
Serial.begin(115200);
delay(333);
Serial.print("Wifi connection created: " );
Serial.println(ssid);
Serial.print("Password: " );
Serial.println(password);
WiFiClientSecure client;
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
  delay(500);
}
client.print(url);
Serial.print("Use this URL : ");
Serial.print("http://");
Serial.print(WiFi.localIP());
Serial.println("/"); 

WiFi.softAP(ssidNew, passwordNew);

IPAddress myIP = WiFi.softAPIP();

Serial.print("AP IP address: ");

Serial.println(myIP);

server.on("/", handleRoot);

server.on("/loco", handleLoco);

server.begin();

Serial.println("HTTP server started");

}

void loop() {
server.handleClient();
}
