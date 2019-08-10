
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}


void loop() {
  serial();
  while(){digitalWrite(LED_BUILTIN, LOW);}
}

void led(){
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
}
void serial(){
  if (Serial.available() > 0) {
    int in = Serial.parseInt();
    //Serial.println(in, HEX);
    if(in != 0){
      Serial.println(in);
      led();
      }
  }
}
