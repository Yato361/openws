// Bibliotheken für das OLED- Panel
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MQ2.h>
#include <DHT.h>
#include <Ethernet.h>

#define OLED_RESET 4
#define DHTPIN 8
#define DHTTYPE DHT11

//defining all constants as primitive variables
char * server = ""; 
byte mac[] = {(byte)random(256), (byte)random(256), (byte)random(256), (byte)random(256), (byte)random(256), (byte)random(256)}; //random mac id generation
int Analog_Input = A0;

Adafruit_SSD1306 display(OLED_RESET);
DHT dht(DHTPIN, DHTTYPE);
MQ2 mq2(Analog_Input);
IPAddress ip(192,168,178,10);
EthernetClient client;

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  dht.begin();
  mq2.begin();

  Serial.print("Client gestartet. EIGENE IP: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  //defining all loop framed constants
  float information[] = {dht.readTemperature(), dht.readHumidity(), mq2.readCO(), mq2.readLPG(), mq2.readSmoke()};

  //print for LPG & CO & SMOKE
  resetDisplay();
  display.println("LPG:  " + String((int)information[3]) + " PPM");
  display.setTextSize(1);
  display.println("CO :  " + String((int)information[2]) + " PPM");
  display.setTextSize(1);
  display.println("Rauch:" + String((int)information[4]) + " PPM");
  display.display();
  
  //DELAY
  delay(500);

  //print for TEMPERATURE 6 HUMDIDITY
  resetDisplay();
  display.println("Temperatur:       " + String((int)information[0]) + "C");
  display.setTextSize(1);
  display.println("Luftfeuchtigkeit: " + String((int)information[1]) + "%");
  display.display();

  //HTTP REQUEST to our Web-API
  if(client.connect(server, 80)){
      Serial.println("connected");
      client.println("GET /insert");
      client.print("?temperature=");
      client.print(information[0]);
      client.print("&humidity=");
      client.print(information[1]);
      client.print("&lpg=");
      client.print(information[3]);
      client.print("&co=");
      client.print(information[2]);
      client.print("&smoke=");
      client.print(information[4]);
      client.print("HTTP/1.0");
      client.println();
  }else{
      Serial.println("connection failed");
  }

  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
}

void resetDisplay(){
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(1, 0);
}
