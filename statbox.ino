#include <SPI.h>
#include <Ethernet.h>
#include <avr/wdt.h>

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x7D, 0xEA};
byte ip[] = { 192, 168, 10, 70 };
char server[] = "stat-tron.herokuapp.com";
EthernetClient client;

int startPin = 5;
int ledPins = 3;
int PARTY_MODE = 4;

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
  wdt_enable(WDTO_8S);
}

void loop() {
  wdt_reset();
  int status = getStatus();
  Serial.println(status);
  int counter = 0;
  while(counter < 30){ //500 ms * 30 = 15 seconds
    Serial.println("loop");
    if(status == PARTY_MODE){
      setLight(random(1,4));
    } else {
      setLight(status);
    }
    counter++;
    delay(500);
    wdt_reset();
  }
  Serial.println("out of loop");
}

int getStatus() {
  if (client.connect(server, 80)) {
    client.println("GET /devconfive HTTP/1.1");
    client.println("Host: stat-tron.herokuapp.com");
    client.println("User-Agent: Arduino");
    client.println("Connection: close");
    client.println();

    while(!client.available()){};
    Serial.println("client available");
    wdt_reset();
    for (int i = 0; i < 9; ++i) {
      client.read();
    } //read 10 chars

    int response = client.read() -48;

    client.flush();
    client.stop(); //close the connection and flush

    return response -1;
  } else {
    client.stop();
  }
  return 4;
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
