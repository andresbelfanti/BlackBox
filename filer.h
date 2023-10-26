#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

//==============================================
bool loadFromSdCard(String path) {

  String dataType = "text/plain";
  if (path.endsWith("/")) path += "index.htm";
  if (path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
  else if (path.endsWith(".htm")) dataType = "text/html";
  else if (path.endsWith(".css")) dataType = "text/css";
  else if (path.endsWith(".js")) dataType = "application/javascript";
  else if (path.endsWith(".jpg")) dataType = "image/jpeg";


  File dataFile = SD.open(path, FILE_READ);
  if (dataFile.isDirectory()) {
    path += "/index.htm";
    dataType = "text/html";
    dataFile = SD.open(path, FILE_READ);
  }
  if (!dataFile) {
    return false;
  }
  if (server.hasArg("download")) dataType = "application/octet-stream";
  if (server.streamFile(dataFile, dataType) != dataFile.size()) {
    Serial.println("!");
  }
  dataFile.close();
  return true;
}
//==================================================
String mensaje() {
  String numberR;
  String msg;
  File dataIndex = SD.open("indexer.txt");
  numberR = dataIndex.readStringUntil('\n');
  dataIndex.close();
  int indexxx = int(random(0, numberR.toInt()));
  File dataFile = SD.open("datalog.txt");
  for (int i = 0; i <= indexxx; i++) { // esto hace un for en base al numero random que tiene como limite el index
    msg = "";
    msg = dataFile.readStringUntil('\n');
   }
  Serial.println(msg);
  return (page+String(msg)+"</h2> </body> </html>");
}
//===============================================
void handleWebRequests() {
  if (loadFromSdCard(server.uri())) return;
  loadFromSdCard("index.htm");
}
//=================================================
void handle_msg() {
  if(checker(list[ips()])==false){ //COMO LE ASIGNO LA IP QUE HACE EL REQUEST ACA??<<<<<<<<<<<<<<<<<<<<<<<########

    int ip = ips();//cone esta funcion se cehquea la ip que envia el mensaje
    String msg = server.arg("msg"); // recibe y escribe el msg del cliente
    Serial.println(msg);
    File dataFile = SD.open("datalog.txt", FILE_WRITE);
    dataFile.println(msg);
    dataFile.close();

//========================devuelve el mensaje
    server.send(200, "text/html", mensaje());

//================== aumenta el index
//esta funcion va despues para que mensaje() no pueda devolver lo mismo
    File dataIndex = SD.open("indexer.txt");
    String numberS = dataIndex.readStringUntil('\n');
    int number = numberS.toInt();
    number = number + 1;
    dataIndex = SD.open("indexer.txt", FILE_WRITE);
    dataIndex.seek(0);
    dataIndex.println(number);
    dataIndex.close();

//==========escribe la mac====================
//*aca hay un problema como funciona la list?
    Serial.print("writing mac");Serial.print("=====>"); Serial.println(String(list[ip]));
    dataFile = SD.open("macs.txt", FILE_WRITE);
    dataFile.println(String(list[ip]));
    dataFile.close();
    Serial.println("list========");
for(int i; i<=255; i++){
  Serial.println(list[i]);
}
 Serial.println("===========");

 for(int i=0; i<4;i++){
   digitalWrite(ledpin, HIGH);
   delay(50);
   digitalWrite(ledpin, LOW);
   delay(50);
   server.client().stop();
    }
  }
}
