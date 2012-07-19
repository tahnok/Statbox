int startPin = 11;
int ledPins = 3;
int incomingByte = 0;

int partyMode = 0;
int rate = 250;
int currentPin = 1;

void setup(){
  for (int i=0; i<ledPins; i++){
    pinMode(startPin + i, OUTPUT);     
  }
  
  Serial.begin(9600);
  setLight(3);
}

void loop(){
  if (Serial.available() > 0){
    incomingByte = Serial.read();
    Serial.print("I received: ");
    Serial.println(incomingByte - 48);
    setLight(incomingByte - 48);
  }
  
  if (partyMode){
    turnOffAllLights();
    int nextPin = random(1, ledPins + 1);
    if (nextPin == currentPin){
      if (nextPin == ledPins){
        nextPin--;
      } else {
        nextPin++; 
      }
    }
    currentPin = nextPin;
    digitalWrite(startPin + currentPin - 1, HIGH);
    delay(rate);
  }
}

void setLight(int id){
  turnOffAllLights();
  
  if (id == 4){
    partyMode = 1; 
  } else {
    partyMode = 0;
    digitalWrite(startPin + id - 1, HIGH);
  }
}

void turnOffAllLights(){
 for (int i=0; i<ledPins; i++){
    digitalWrite(startPin + i, LOW);
  } 
}
