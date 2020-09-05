#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
const char* yourSSID = "VilaVeronika1"; 
const char* yourPASS = "Julinka12"; 
const char* url = "https://us-central1-sim808-gps.cloudfunctions.net/setposition/kike/1/2";
 
void setup() {
 
  Serial.begin(115200);                                  //Serial connection
  WiFi.begin(yourSSID, yourPASS);   //WiFi connection
 
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
 
    delay(500);
    Serial.println("Waiting for connection");
 
  }
 
}
 
void loop() {
 
 if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
 
   HTTPClient http;    //Declare object of class HTTPClient
 
   http.begin(url);      //Specify request destination
   http.addHeader("Content-Type", "text/plain");  //Specify content-type header
 
   int httpCode = http.POST("Message from ESP8266");   //Send the request
   String payload = http.getString();                  //Get the response payload
 
   Serial.println(httpCode);   //Print HTTP return code
   Serial.println(payload);    //Print request response payload
 
   http.end();  //Close connection
 
 }else{
 
    Serial.println("Error in WiFi connection");   
 
 }
 
  delay(30000);  //Send a request every 30 seconds
 
}
