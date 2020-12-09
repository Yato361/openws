#include <SPI.h>
#include <Wire.h>
#include <SSD1306Ascii.h>
#include <SSD1306AsciiAvrI2c.h>
#include <MQ2Lib.h>
#include <Adafruit_BMP280.h> //
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
Adafruit_BMP280 bmp; //

DHT dht(DHTPIN, DHTTYPE);
MQ2 mq2(Analog_Input, true);
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
  bmp.begin(0x76); //

  Serial.print("Client gestartet. EIGENE IP: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  mq2.read(true);
  
  //defining all loop framed constants
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  float co = mq2.readCO();
  float lpg = mq2.readLPG();
  float smoke = mq2.readSmoke();
  float pressure = (bmp.readPressure()/100);
  float altitude = (bmp.readAltitude(1019.66));
  
  //clearing and initializing the front of the oled
  oled.setFont(Adafruit5x7);
  oled.clear();
  
  //print for LPG & CO & SMOKE
  oled.println("LPG:      " + String(lpg) + " PPM");
  oled.println("CO :      " + String(co) + " PPM");
  oled.println("Rauch:    " + String(smoke) + " PPM");

  //print for TEMPERATURE & HUMDIDITY
  print_leertaste();
  oled.println("Temp____: " + String(temp) + "C");
  oled.println("Humidity: " + String(hum) + "%");
  oled.println("Pressure: " + String(pressure) + "hPa");
  oled.println("Altitude: " + String(altitude) + "m");
  delay(2500);
  
  //HTTP REQUEST to our Web-API
  if(client.connect(server, 80)){
      Serial.println("connected");
      client.println("GET /insert");
      client.print("?temperature=");
      client.print(temp);
      client.print("&humidity=");
      client.print(hum);
      client.print("&lpg=");
      client.print(lpg);
      client.print("&co=");
      client.print(co);
      client.print("&smoke=");
      client.print(smoke);
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
