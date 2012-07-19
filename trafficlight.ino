#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x7D, 0xEA};
char server[] = "http://devconfive.herokuapp.com";
EthernetClient client;

int getStatus(void);
int contactServer(void);
int parseResponse(void);

void setup() {
  Serial.begin(9600);
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
  }
}

void loop() {
  Serial.println("begin loop");
  int a = getStatus();
  Serial.println(a);
  delay(10000);
}

int getStatus() { //returns status code or -1 if failure
  if(contactServer() == -1){
    return -1;
  }
  while(!client.connected()); //wait until connected
  return parseResponse();
}

int contactServer(){
  if (client.connect(server, 80)) {
    client.println("GET /index.json HTTP/1.1");
    client.println("Host: devconfive.herokuapp.com");
    client.println("User-Agent: Arduino");
    client.println();
    Serial.println("connected with server");
    return 1;
  }
  Serial.println("Didn't connect to server");
  return -1;
}

int parseResponse(){
  if (client.available()) {
    Serial.println("client is available");
    for (int i = 0; i < 9; ++i) { client.read(); } //read 10 chars
    int response = 0;
    for (int i = 0; i < 3; ++i)
    {
      char c = client.read();
      response = response * 10 + (c - 48); //ascii to int
    }
    client.stop();
    return response;
  }
  Serial.println("client is NOT available");
  return -1;
}