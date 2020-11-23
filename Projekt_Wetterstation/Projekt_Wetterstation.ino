#include <SPI.h>
#include <Wire.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MQ2.h>

#define OLED_RESET 4 // RESET- Funktion: Nicht genutzt bei diesem Display, da keine vorhanden | Muss angegeben werden
Adafruit_SSD1306 display(OLED_RESET);

#define DHTPIN 8     // Digitaler Pin am Arduino: 8
#define DHTTYPE DHT11   // Sensortyp: DHT11
DHT dht(DHTPIN, DHTTYPE);

// Globale Vars. benötigt MQ-2
int Analog_Input = A0;    // Analoger Pin am Arduino: A0
int lpg, co, smoke;

MQ2 mq2(Analog_Input);

void setup() {
  // mit I2C-Adresse 0x3c initialisieren
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // DHT11 - Startprozess
  dht.begin();
  // MQ-2 - Startprozess:
  Serial.begin(9600);
  mq2.begin();
}

void loop() {
  /*test*/
  // DHT11- Daten
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  // MQ-2 - Daten
  float* values = mq2.read(true); // für serielle Werte auf false setzen 
  //lpg = values[0];
  float lpg = mq2.readLPG();
  //co = values[1];
  float co = mq2.readCO();
  //smoke = values[2];
  float smoke = mq2.readSmoke();
/*
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.println("Initialisierung...");    // Testausgabe OLED
  delay(2000);
*/
  // Grundeinstellung + leeres Display    DHT11- Teil
  display.clearDisplay();
  // Textfarbe setzen | AUCH BEI MONOCHROMEN OLEDs
  display.setTextColor(WHITE);
  // Textgröße einstellen ("1" empfohlen)
  display.setTextSize(1);
  // Cursorposition einstellen
  display.setCursor(1, 0);
  // Daten des Sensors auf dem OLED ausgeben
  display.println("Temperatur:       " + String(t, 0) + "C");    // Ausgabe Temperatur
  display.setTextSize(1);
  display.println("Luftfeuchtigkeit: " + String(h, 0) + "%");  // Ausgabe Luftfeuchtigkeit
  display.display();
  delay(5000);     // Ende des ersten Steps

  // Grundeinstellung + leeres Display    MQ-2 Teil
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(1, 0);
  display.println("LPG:  " + String(lpg, 0) + " PPM");
  display.setTextSize(1);
  display.println("CO :  " + String(co, 0) + " PPM");
  display.setTextSize(1);
  display.println("Rauch:" + String(smoke, 0)+ " PPM");
  display.display();
  delay(5000);    // Ende des zweiten Steps
}
