//M1
const int stepPin1 = D2;
const int dirPin1 = D1; //directions pin HIGH Down
const int ON1 = D8;

//M2
const int stepPin2 = D0; //3
const int dirPin2 = D5; //1  directions pin HIGH Down
const int ON2 = D7;

//M3
const int stepPin3 = D4; //3
const int dirPin3 = D3; //1  directions pin HIGH Down
const int ON3 = D6;

#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
const char* ssid     = "C******G";
const char* password = "L********s";
#define FIREBASE_HOST "fir-c***d.firebaseio.com"
#define FIREBASE_AUTH "DZTko0w8lyt1cxv9jAKtC***ipzoN0WShYVCl9rf"
int unVar=1000;
long currentSteps1 = 30*unVar;
long currentSteps2 = 30*unVar;
long currentSteps3 = 30*unVar;
long steps1 =30*unVar;
long steps2 =30*unVar;
long steps3 =30*unVar;
int t=1;


void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin1,OUTPUT);
  pinMode(dirPin1,OUTPUT);
  pinMode(ON1,OUTPUT);
  pinMode(stepPin2,OUTPUT);
  pinMode(dirPin2,OUTPUT);
  pinMode(ON2,OUTPUT);
  pinMode(stepPin3,OUTPUT);
  pinMode(dirPin3,OUTPUT);
  pinMode(ON3,OUTPUT);
  WiFi.begin(ssid, password);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.setInt("Steps1", 30);
  Firebase.setInt("Steps2", 30);
  Firebase.setInt("Steps3", 30);
}




void All(){
  if (steps1>currentSteps1||steps2>currentSteps2|steps3>currentSteps3){
           digitalWrite(ON1,HIGH);
           digitalWrite(dirPin1,HIGH); // Down
           digitalWrite(ON2,HIGH);
           digitalWrite(dirPin2,HIGH); // Down
           digitalWrite(ON3,HIGH);
           digitalWrite(dirPin3,HIGH); // Down
           while (steps2>currentSteps2){
              digitalWrite(stepPin1,HIGH); digitalWrite(stepPin2,HIGH); digitalWrite(stepPin3,HIGH);
              delay(t);
              digitalWrite(stepPin1,LOW); digitalWrite(stepPin2,LOW); digitalWrite(stepPin3,LOW);
              delay(t);
              ++currentSteps1;++currentSteps2;++currentSteps3;}
           digitalWrite(ON1,LOW); digitalWrite(ON2,LOW); digitalWrite(ON3,LOW); }
  if (steps1<currentSteps1||steps2<currentSteps2||steps3<currentSteps3){
           digitalWrite(ON1,HIGH);
           digitalWrite(dirPin1,LOW); // Down
           digitalWrite(ON2,HIGH);
           digitalWrite(dirPin2,LOW); // Down
           digitalWrite(ON3,HIGH);
           digitalWrite(dirPin3,LOW); // Down
           while (steps2<currentSteps2){
              digitalWrite(stepPin1,HIGH); digitalWrite(stepPin2,HIGH); digitalWrite(stepPin3,HIGH);
              delay(t);
              digitalWrite(stepPin1,LOW); digitalWrite(stepPin2,LOW); digitalWrite(stepPin3,LOW);
              delay(t);
              --currentSteps1;--currentSteps2;--currentSteps3;}
           digitalWrite(ON1,LOW); digitalWrite(ON2,LOW); digitalWrite(ON3,LOW); }
  }

void first(){
  if (steps1>currentSteps1){digitalWrite(ON1,HIGH);
           digitalWrite(dirPin1,HIGH); // Down
           while (steps1>currentSteps1){
              digitalWrite(stepPin1,HIGH);
              delay(t);
              digitalWrite(stepPin1,LOW);
              delay(t);
              ++currentSteps1;}
           digitalWrite(ON1,LOW);}
  if (steps1<currentSteps1){digitalWrite(ON1,HIGH);
             digitalWrite(dirPin1,LOW); // Down
             while (steps1<currentSteps1){
              digitalWrite(stepPin1,HIGH);
              delay(t);
              digitalWrite(stepPin1,LOW);
              delay(t);
              --currentSteps1;}
             digitalWrite(ON1,LOW);}
             }
void seconds(){
    if (steps2>currentSteps2){digitalWrite(ON2,HIGH);
           digitalWrite(dirPin2,HIGH); // Down
           while (steps2>currentSteps2){
              digitalWrite(stepPin2,HIGH);
              delay(t);
              digitalWrite(stepPin2,LOW);
              delay(t);
              ++currentSteps2;}
           digitalWrite(ON2,LOW); }
  if (steps2<currentSteps2){digitalWrite(ON2,HIGH);
             digitalWrite(dirPin2,LOW); // Down
             while (steps1<currentSteps2){
              digitalWrite(stepPin2,HIGH);
              delay(t);
              digitalWrite(stepPin2,LOW);
              delay(t);
              --currentSteps2;}
             digitalWrite(ON2,LOW);}

  }
void third(){
  if (steps3>currentSteps3){digitalWrite(ON3,HIGH);
           digitalWrite(dirPin3,HIGH); // Down
           while (steps3>currentSteps3){
              digitalWrite(stepPin3,HIGH);
              delay(t);
              digitalWrite(stepPin3,LOW);
              delay(t);
              ++currentSteps3;}
           digitalWrite(ON3,LOW);}
  if (steps3<currentSteps3){digitalWrite(ON3,HIGH);
             digitalWrite(dirPin3,LOW); // Down
             while (steps3<currentSteps3){
              digitalWrite(stepPin3,HIGH);
              delay(t);
              digitalWrite(stepPin3,LOW);
              delay(t);
              --currentSteps3;}
             digitalWrite(ON3,LOW);}
  }


void loop() {
  steps1=Firebase.getInt("Steps1")*unVar;
  if(steps1!=currentSteps1){
      steps2=Firebase.getInt("Steps2")*unVar;
      steps3=Firebase.getInt("Steps3")*unVar;
        if(steps1==steps2&&steps2==steps3&&currentSteps1==currentSteps2&&currentSteps2==currentSteps3)
        {All();
          }else{first();}
    }
  steps2=Firebase.getInt("Steps2")*unVar;
  if(steps2!=currentSteps2){
      steps1=Firebase.getInt("Steps1")*unVar;
      steps3=Firebase.getInt("Steps3")*unVar;
        if(steps1==steps2&&steps2==steps3&&currentSteps1==currentSteps2&&currentSteps2==currentSteps3)
            {All();
              }else{seconds();}
    }
  steps3=Firebase.getInt("Steps3")*unVar;
  if(steps3!=currentSteps3){
      steps2=Firebase.getInt("Steps2")*unVar;
      steps1=Firebase.getInt("Steps1")*unVar;
        if(steps1==steps2&&steps2==steps3&&currentSteps1==currentSteps2&&currentSteps2==currentSteps3)
          {All();
            }else{third();}
      }

}
