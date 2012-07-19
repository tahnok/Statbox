#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x7D, 0xEA};
char server[] = "stat-tron.herokuapp.com";
EthernetClient client;

int startPin = 5;
int ledPins = 3;

void setup() {
  for (int i=0; i<ledPins; i++){
    pinMode(startPin + i, OUTPUT);
    digitalWrite(startPin +i, HIGH);
  }
  Serial.begin(9600);
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
  } else {
    Serial.println("connected!");
  }
}

void loop() {
  int status = getStatus();
  Serial.println(status);
  setLight(status);
  delay(15000); //15 second delay
}

int getStatus() {
  if (client.connect(server, 80)) {
    client.println("GET /devconfive HTTP/1.1");
    client.println("Host: stat-tron.herokuapp.com");
    client.println("User-Agent: Arduino");
    client.println("Connection: close");
    client.println();

    while(!client.available()){}; //TODO timeout?

    for (int i = 0; i < 9; ++i) {
      client.read();
    } //read 10 chars

    int response = client.read() -48;

    client.flush();
    client.stop(); //close the connection and flush

    return response;
  } else {
    client.stop();
  }
  return -1;
}

void setLight(int id){
  turnOffAllLights();
  digitalWrite(startPin + id - 1, HIGH);
}

void turnOffAllLights(){
 for (int i=0; i<ledPins; i++){
    digitalWrite(startPin + i, LOW);
  }
}
