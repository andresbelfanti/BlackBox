#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SPI.h>
#include <SD.h>

String mac; //variable global tambien utiizada en las funciones
//ACA====
String list[255]; // almacena las MAC - chequear formatos y strings a ver si anda<<<<<<<<<

int ledpin = 15;                           // light connected to digital PWM pin 9
int period = 2000;
unsigned long currentMillis;
unsigned long previousMillis;
unsigned long interval = 300000;

const byte        DNS_PORT = 53;          // Capture DNS requests on port 53
IPAddress         apIP(192, 168, 0, 1);    // Private network for server
DNSServer         dnsServer;              // Create the DNS object
static bool hasSD = false;
ESP8266WebServer server(80);


#include "head.h";
#include "mac.h";
#include "filer.h";

//SETUP====================================
void setup(void) {
  int i = 0;
  Serial.begin(9600);

  if (SD.begin(SS)) {
    hasSD = true;
  }
  while (!SD.begin(4)) {
    Serial.println("Card failed, or not present");
      analogWrite(ledpin, 255);
      delay(250);
      analogWrite(ledpin, 0);
      delay(250);
   }
  Serial.println("card initialized.");

  //=====================================================
  listName();
  netName();
  stationConnectedHandler = WiFi.onSoftAPModeStationConnected(&onStationConnected); //FUNCION EN mac.h
  wifiStaDisconnectHandler = WiFi.onSoftAPModeStationDisconnected(wifiStaDisconnect);//FUNCION EN mac.h

  //=============se supone que entre las dos funciones no hay delay
  server.onNotFound([]() {
    int  ip = ips(); //chequea ip del cliente

    // si no estaba conectado previamente=============
    if (list[ip] == String("no") || list[ip] == 0) { //si la ip no esta en "list"
      list[ip] = mac; // asigna la mac a la ip
      Serial.print("online:::::  "); Serial.print(ip); Serial.print("=>"); Serial.println(list[ip]);
      handleWebRequests();
    }

    // si ya estaba conectada la ip chequea mac=====================
    if (list[ip] != "no" || list[ip] != 0) { // si la list tiene un num
      if (checker(list[ip]) == false) { // si la mac no está rergistrada
        handleWebRequests();
      }
      //si la mac ya estaba conectada=======================
      else {
        Serial.println("kick");
        server.client().stop(); //ESTO FUNCIONA?????? buscar otra forma de denegar
      }
    }

  });
  // FIN VOID NOT FOUND====================

  // respuesta al mensaje===========================
  server.on("/msg", handle_msg);
  //========================================
  server.begin();
} //fin setup==================================

void loop(void) {
  dnsServer.processNextRequest();
  server.handleClient();
  analogWrite(ledpin, 128 + 127 * cos(2 * PI / period * millis())); //acorte esto -funciona?

  currentMillis = millis(); // grab current time

  if ((unsigned long)(currentMillis - previousMillis) >= interval) {
    netName();
    previousMillis = millis();
  }
}
