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
int lpg, co, smoke;

MQ2 mq2(Analog_Input);

// Hier die MAC Adresse des Shields eingeben
// (Aufkleber auf Rückseite)
byte mac[] = {0x90, 0xA2, 0xDA, 0x00, 0xFB, 0x80};
// Eine IP im lokalen Netzwerk angeben. 
IPAddress ip(192,168,1,1);
// Ethernet Library als Server initialisieren // Port festlegen: default "80"
EthernetServer server(80);

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
 server.begin();
 Serial.print("Server gestartet. IP: ");
 // IP des Arduino-Servers ausgeben
 Serial.println(Ethernet.localIP());
}

void loop() {
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

  // server.available() schaut, ob ein Client verfügbar ist und Daten
  // an den Server schicken möchte. Gibt dann eine Client-Objekt zurück,
  // sonst false
  EthernetClient client = server.available();
  // Wenn es einen Client gibt, dann...
  if (client) {
    Serial.println("Neuer Client");
    // Jetzt solange Zeichen lesen, bis eine leere Zeile empfangen wurde
    // HTTP Requests enden immer mit einer leeren Zeile 
    boolean currentLineIsBlank = true;
    // Solange Client verbunden 
    while (client.connected()) {
      // client.available() gibt die Anzahl der Zeichen zurück, die zum Lesen
      // verfügbar sind
      if (client.available()) {
          // Ein Zeichen lesen und am seriellen Monitor ausgeben
          char c = client.read();
          Serial.write(c);
        // In currentLineIsBlank merken wir uns, ob diese Zeile bisher leer war.
        // Wenn die Zeile leer ist und ein Zeilenwechsel (das \n) kommt,
        // dann ist die Anfrage zu Ende und wir können antworten
          if (c == '\n' && currentLineIsBlank) {
              // HTTP Header 200 an den Browser schicken
              client.println("HTTP/1.1 200 OK\nContent-Type: text/html\nConnection: close\nRefresh: 2\n<!DOCTYPE HTML>\n<html> DHT-11 (Pin A0): "); // Seite alle 25 Sekunden neu abfragen
              client.print(analogRead(A0));
              client.println("<b></b><br />\n</html>");
              break;
            }
            if (c == '\n') {
              // Zeilenwechsel, also currentLineIsBlack erstmal auf True setzen
              currentLineIsBlank = true;
            } 
            else if (c != '\r') {
              // Zeile enthält Zeichen, also currentLineIsBlack auf False setzen
              currentLineIsBlank = false;
            }
         }
      }
   // Kleine Pause
   delay(1);
   // Verbindung schliessen
   client.stop();
   Serial.println("Verbindung mit Client beendet.\n");

     display.setTextColor(WHITE);
  display.setTextSize(1);
  display.println("Initialisierung...");    // Testausgabe OLED
  delay(2000);

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
  display.println("LPG:  " + String(lpg, 0)+ " PPM");
  display.setTextSize(1);
  display.println("CO :  " + String(co, 0)+ " PPM");
  display.setTextSize(1);
  display.println("Rauch:" + String(smoke, 0)+ " PPM");
  display.display();
  delay(5000);    // Ende des zweiten Steps
 }
}
