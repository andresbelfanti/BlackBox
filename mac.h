WiFiEventHandler stationConnectedHandler;
WiFiEventHandler wifiStaDisconnectHandler;

char red[60][50];
char pass[50];
bool looper = true;
int count = 0;
int largoList;
char APname[40];



//=========mac a string=======================
String macToString(const unsigned char* mac) {
  char buf[20];
  snprintf(buf, sizeof(buf), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(buf);
}
//=========================================
int ips() { //identifica el ultimo numero de la IP
  IPAddress ipno;
  ipno = server.client().remoteIP();
  return (int(ipno[3]));  // ultima parte de la ip en forma de int
}
//==========================================

boolean checker(String macDev) {
 File dataFile = SD.open("macs.txt", FILE_READ);
 boolean check=false;

  while (dataFile.available()&&check==false) {
    String macSaved = dataFile.readStringUntil('\n'); // avanza en la lista??
    macSaved.remove(macSaved.length()-1); // ESTO???? remueve \n
    Serial.print(macSaved); Serial.print("  ===  "); Serial.println(macDev);
      if (macDev == macSaved) { // CHEQUEAR SI LA COMPARACION RESULTA
          check=true;
          break;
        }
  }
    Serial.println("end of file");
    dataFile.close();
    return(check);
}
//=======================================================
void listName(){
  boolean again = false;
  int n;
  File redFile = SD.open("red.txt", FILE_READ);
  while (redFile.available()) {
    String  numberR = redFile.readStringUntil('\n');
    numberR.toCharArray(pass, 50);
    for (int i = 0; i <= 50; i++) {
      red[n][i] = pass[i];
    }
    n++;
  }
  largoList=n;
  Serial.print(n);
  Serial.println(" nets names");
   redFile.close();
}

void netName() {
      if(count<largoList){
        count += 1;
      }
      else{count=0;}

    strcpy(APname, red[count] ); // esta  funcion reasigna un pointer
    Serial.println(APname);

    WiFi.persistent(true);
    WiFi.mode(WIFI_AP);

    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0)); //falta cant max de conexiones!!
    WiFi.softAP(APname);
    dnsServer.start(DNS_PORT, "*", apIP);
  }

//========conexion y desconexion de dispositivos==========================
  void onStationConnected(const WiFiEventSoftAPModeStationConnected & evt) {
    String macDev = macToString(evt.mac); //devuelve la ma como string (2 funciones anidadas)
    Serial.print("Station connected====>");
    Serial.println(macDev);
    mac = macDev; //asigna a variable global


  }

  void wifiStaDisconnect(const WiFiEventSoftAPModeStationDisconnected & evt) {
    String mac = macToString(evt.mac);
    for (int i = 0; i <= 4; i++) {
      Serial.println(list[i]);
      if (list[i] == mac) { // borra la mac de la lista de activos
          Serial.println("Match!");
        list[i] = "no";
        break;
      }
      else{
        Serial.println("no match");
      }
    }
    Serial.println("disconected");
  }
