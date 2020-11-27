#include <SPI.h>
#include <Wire.h>
#include <SSD1306Ascii.h>
#include <SSD1306AsciiAvrI2c.h>
#include <MQ2.h>
#include <DHT.h>
#include <Ethernet.h>

#define RST_PIN -1
#define I2C_ADDRESS 0x3C
#define DHTPIN 8
#define DHTTYPE DHT11

//defining all constants as primitive variables
char * server = ""; 
byte mac[] = {(byte)random(256), (byte)random(256), (byte)random(256), (byte)random(256), (byte)random(256), (byte)random(256)}; //random mac id generation
int Analog_Input = A0;

SSD1306AsciiAvrI2c oled;

DHT dht(DHTPIN, DHTTYPE);
MQ2 mq2(Analog_Input);
IPAddress ip(192,168,178,10);
EthernetClient client;

void setup() {
#if RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
#else // RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
#endif
  
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  
  dht.begin();
  mq2.begin();

  Serial.print("Client gestartet. EIGENE IP: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  //defining all loop framed constants
  float information[] = {dht.readTemperature(), dht.readHumidity(), mq2.readCO(), mq2.readLPG(), mq2.readSmoke()};

  //clearing and initializing the front of the oled
  oled.setFont(Adafruit5x7);
  oled.clear();
  
  //print for LPG & CO & SMOKE
  oled.println("LPG:  " + String((int)information[3]) + " PPM");
  print_leertaste();
  oled.println("CO :  " + String((int)information[2]) + " PPM");
  print_leertaste();
  oled.println("Rauch:" + String((int)information[4]) + " PPM");

  //print for TEMPERATURE 6 HUMDIDITY
  print_leertaste();
  oled.println("Temp____: " + String((int)information[0]) + "C");
  oled.println("Humidity: " + String((int)information[1]) + "%");

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

void print_leertaste(){
    oled.println("---------------------");  
}
