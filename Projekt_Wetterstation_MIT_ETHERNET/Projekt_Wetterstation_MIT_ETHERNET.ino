// Bibliotheken für das OLED- Panel
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// Bibliothek für den MQ-2 Sensor
#include <MQ2.h>
// Bibliothek für den DHT-11 Sensor
#include <DHT.h>
// Bibiothek für das Ethernetshield
#include <Ethernet.h>

#define OLED_RESET 4 // RESET- Funktion: Nicht genutzt bei diesem Display, da keine vorhanden | Muss angegeben werden
Adafruit_SSD1306 display(OLED_RESET);


#define DHTPIN 8     // Digitaler Pin am Arduino: 8
#define DHTTYPE DHT11   // Sensortyp: DHT11
DHT dht(DHTPIN, DHTTYPE);

// Globale Vars. benötigt MQ-2
int Analog_Input = A0;    // Analoger Pin am Arduino: A0

MQ2 mq2(Analog_Input);

// Hier die MAC Adresse des Shields eingeben
// (Aufkleber auf Rückseite)
byte mac[] = {0x90, 0xA2, 0xDA, 0x00, 0xFB, 0x80};
// Eine IP im lokalen Netzwerk angeben. 
IPAddress ip(192,168,178,10);
//The char pointer contains the whole server domain as chars.
char* server= ""; 
//Defining the EthernetClient
EthernetClient client(server,80);

void setup() {
  // Serielle Datenübertragung, für den Monitor erstellen
  Serial.begin(9600);
  // mit I2C-Adresse 0x3c initialisieren
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // DHT11 - Startprozess
  dht.begin();
  // MQ-2 - Startprozess:
  mq2.begin();
  // Ethernet Verbindung und Server starten
  Ethernet.begin(mac, ip);
  
  Serial.print("Client gestartet. IP: ");
  // IP des Arduino-Servers ausgeben
  Serial.println(Ethernet.localIP());
}

void loop() {
    
}
