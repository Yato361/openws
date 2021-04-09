# Open-Weather-Station
(EN) A small arduino project to gain enviromental information. \
(DE) Ein kleines Arduino Projekt um Informationen aus der Umgebung zu sammeln.

# Dokumentation Inhaltsverzeichnis
* API
* Datenbanken
* Arduino Software
* Arduino Hardware
* Schaltplan vom Arduino
* Installation vom Arduino
* Installation der API
* Website (In planung)

# API 
Wir haben uns überlegt eine API anzulegen, um die gesammelten Informationen zu verarbeiten. \
Derzeit werden Informationen wie Luftfeuchtigkeit, Temparatur, Kohlenstoffmonoxid, Rauchpartikel und Liquefied Petroleum Gase gesammelt. \
Auch haben wir einige weitere Funktionalitäten eingebaut um z.B. auch die Sicherheit der Datenbanken zu gewährleisten. \
Hierzu gehören HWID Authentifikationen und MySQL Prepared Statements. 

Die API wurde aus vier Teilen gebaut.
1. HWID Authentifikation
2. Fehlerprüfung der gesendeten Informationen
3. Datenbank Eintrag
4. Rückmeldung mit Response-Codes

Derzeit haben wir nur 5 Response-Codes eingebaut.
1. *SUCCESSFULL* mit dem Response-Code '0'
2. *UNFORMATTED_TYPE_ERROR* mit dem Response-Code '1'
3. *NOT_COMPLETED_ERROR* mit dem Response-Code '2'
4. *NO_RIGHTS_ERROR* mit dem Response-Code '3'
5. *EXCEPTION_ERROR* mit dem Response-Code '4'

Jedoch kann das Arduino derzeit diese Response-Codes noch nicht verarbeiten. \
Sie ist somit nur als Debugfeature.

Die HWID Authentifikation geschieht parallel mit der Fehlerprüfung der jeweiligen Informationen. \
Sollte z.B. ein Response-Code ausgeworfen werden, welches alles andere als *SUCCESSFULL* ist, wird kein Datenbankeintrag erfolgen. \
Dies ist ebenfalls ein Sicherheitsfeature um bei der automatischen Auswertung, welches ebenfalls noch in Planung ist, keine Schwierigkeiten hervorzurufen.

# Datenbanken
Derzeit benutzen wir das DBMS MySQL und/oder MariaDB. \
Aufgrund dessen, dass die API auf einer RaspberryPi ausgeführt wird, mussten wir MariaDB bevorzugen.

Derzeit ist die Datenbankstruktur nur auf 8 Spalten beschränkt.
* ID_PRIMARY
* HWID
* UNIX_TIMESTAMP
* TEMP
* HUMIDITY
* LPG
* CO
* SMOKE

Es ist noch in Planung zwei weitere Spalten einzubauen (Luftdruck und Höhe)

Beispiel Eintrag
![](https://github.com/Yato361/weather-station/blob/main/DB_BEISPIEL_EINTRAG.png)

Beispiel Query \
INSERT INTO \`arduino_station_weather_information\`(\`HWID\`, \`UNIX_TIMESTAMP\`, \`TEMP\`, \`HUMIDITY\`, \`LPG\`, \`CO\`, \`SMOKE\`) VALUES (1867912635, 1611193524, 24.3, 46, 0, 0, 0.01)

Datenbank Struktur: \
![](https://github.com/Yato361/weather-station/blob/main/DB_STRUCTURE_PICTURE.png)

# Arduino Software
Das Arduino wurde so Programmiert, dass sie Informationen aus der Umgebung sammelt, sie auf einem Bildschirm ausgibt und dem Server schickt. \
Bei dem starten des Arduinos wird dieser zuerst Variablen wie `server`, `mac` und `IPAdress` etc. definieren, um in der nächsten Phase, der Initialisierung des Ethernetshields, keine probleme hervorzurufen.

Zunächst werden die Module Ethernetshield, MQ2 und DHT im `setup` Funktion initialisiert. Um das Debuggen zu ermöglichen wird hierbei auch `Serial` verwendet.
```c
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
```

In der `loop` Funktion des Projektes werden Umgebungsinformationen gesammelt, die wiederum in die zugehörigen Datentypen verpackt und daraufhin gespeichert werden. \
Zunächst wird ein `GET Request` gebildet. Hierbei werden wichtige Informationen wie das HWID und die Umgebungsinformationen hinzugefügt. \

```c
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
      client.println("Host: pi.0unknownuser.de");
      client.println("Connection: close");
      client.println();
  }else{
      Serial.println("connection failed");
  }
  delay(3000);
}
```
Das vom Server gesendete Response-Code wird vom Client ignoriert. \
Es steht in Planung, die Response-Codes gelistet auf einem OLED Bildschirm anzuzeigen.

Derzeit steht im `loop` ein drei Sekunden delay, um ein mögliches unerwünschtes `Database-Flooding` zu verhindern. 

# Arduino Hardware

Das Arduino Hardware besteht aus folgenden Komponenten

* MQ2 Sensor
  * Misst die Werte: LPQ (Liquified Petroleum Gas), CO (Kohlenstoff Monoxid), Rauchpartikel
* DHT11 Sensor
  * Misst die Werte: Temperatur, Luftfeuchtigkeit
* OLED 128x64 Pixel 0,96 Zoll
  * Das OLED Display wird dazu verwendet die aktuellen Werte der Sensoren vereinfacht auszugeben.
* Ethernet- Shield W5100
  * Das Ethernet- Shield wird auf dem Arduino Uno montiert, um eine Netzverbindung zu ermöglichen.
* Arduino Uno
  * Der Mikrokontroller sammelt und wertet alle Informationen aus und schickt die HTTP Anfragen. 
* 26 Überbrückungskabel, ein Steckbrett und ein 10kOhm Widerstand

# Schaltplan vom Arduino
![](https://github.com/Yato361/weather-station/blob/main/ARDUINO_SCHALTPLAN.png)

# Installation vom Arduino
1. Bauen Sie das Schaltplan nach.
2. Brennen Sie die Software auf das Arduino. Passen Sie bitte die Variablen `server[]`, `mac[]` und `ip` an. Link vom Arduino-Software: https://github.com/Yato361/openws/blob/main/Projekt_Wetterstation_MIT_ETHERNET/Projekt_Wetterstation_MIT_ETHERNET.ino
3. Verbinden Sie nun mithilfe des Ethernetkabels, das Arduino mit Ihrem Router.

# Installation vom DOTNET API
(Derzeit in Bearbeitung)

# Website (In Planung)
