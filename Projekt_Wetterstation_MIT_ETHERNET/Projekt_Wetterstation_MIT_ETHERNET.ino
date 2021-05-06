#include <SPI.h>
#include <Wire.h>
#include <SSD1306Ascii.h>
#include <SSD1306AsciiAvrI2c.h>
#include <MQ2Lib.h>
#include <DHT.h>
#include <Ethernet.h>

#define RST_PIN -1
#define I2C_ADDRESS 0x3C
#define DHTPIN 8
#define DHTTYPE DHT11

//defining all constants as primitive variables
char server[] = "example.com"; 
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
int Analog_Input = A0;
                                                                  
SSD1306AsciiAvrI2c oled;

DHT dht(DHTPIN, DHTTYPE);
MQ2 mq2(Analog_Input, true);
IPAddress ip(192, 168, 0, 177);
IPAddress myDns(192, 168, 0, 1);
EthernetClient client;
bool printWebData = true;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; 
  }
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1); 
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    Serial.print("1203912ß3098123123");
    Ethernet.begin(mac, ip, myDns);
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }

#if RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
#else // RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
#endif  
  
  dht.begin();
  mq2.begin();
  delay(1000);
}

void loop() {
  mq2.read(true);
  
  //defining all loop framed constants
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  float co = mq2.readCO();
  float lpg = mq2.readLPG();
  float smoke = mq2.readSmoke();
  
  //clearing and initializing the front of the oled
  oled.setFont(Adafruit5x7);
  oled.clear();
  
  //print for LPG & CO & SMOKE
  oled.println("LPG:      " + String(lpg) + " PPM");
  oled.println("CO :      " + String(co) + " PPM");
  oled.println("Rauch:    " + String(smoke) + " PPM");

  //print for TEMPERATURE 6 HUMDIDITY
  print_leertaste();
  oled.println("Temp____: " + String(temp) + "C");
  oled.println("Humidity: " + String(hum) + "%");
  
  //HTTP REQUEST to our Web-API
  Serial.print("connecting to ");
  Serial.print(server);
  Serial.println("...");
  
  if(client.connect(server, 80)){
      Serial.println("Connected to");
      Serial.println(client.remoteIP());
      client.print("GET /insert");
      client.print("?hwid=");
      client.print("1867912635");
      client.print("&temperature=");
      client.print(temp);
      client.print("&humidity=");
      client.print(hum);
      client.print("&lpg=");
      client.print(lpg);
      client.print("&co=");
      client.print(co);
      client.print("&smoke=");
      client.print(smoke);
      client.println(" HTTP/1.1");
      client.println("Host: example.com");
      client.println("Connection: close");
      client.println();
  }else{
      Serial.println("connection failed");
  }
  delay(3000);
}

void print_leertaste(){
    oled.println("---------------------");  
}
