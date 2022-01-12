#include <WiFi.h>

char* ssid     = "**internet-kullanici-adi**";
char* password = "**internet-sifresi**";

WiFiServer server(80);

#define motorR1 18
#define motorR2 19
#define motorL1 22
#define motorL2 23
#define solsensor 26
#define ortasensor 27
#define sagsensor 32
#define trigpin 33
#define echopin 34
#define sagkonumsensor 15
#define solkonumsensor 35

int sure;
int mesafe;
int baslangicTime;
int i;
int k;

void setup()
{
    Serial.begin(115200);
    /*Serial.println("Restarting in 10 seconds");
    delay(30000);
    ESP.restart();*/
    pinMode(motorR1, OUTPUT);
    pinMode(motorR2, OUTPUT);
    pinMode(motorL1, OUTPUT);
    pinMode(motorL2, OUTPUT);
    pinMode(solsensor, INPUT);
    pinMode(ortasensor, INPUT);
    pinMode(sagsensor, INPUT);
    pinMode(echopin, INPUT);
    pinMode(trigpin, OUTPUT);
    pinMode(sagkonumsensor, INPUT);
    pinMode(solkonumsensor, INPUT);
    attachInterrupt(digitalPinToInterrupt(echopin),detect,CHANGE);
    digitalWrite(trigpin, LOW);
    
    delay(10);

    ledcSetup(0,5000,8);  //sol motor hız ayarı
    ledcAttachPin(21,0);  //sol motor hız ayarı
    ledcSetup(0,5000,8);  //sag motor hız ayarı
    ledcAttachPin(25,0);  //sag motor hız ayarı

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.begin();

}

int value = 0;

int bolge = 0;

void loop(){
        
 WiFiClient client = server.available();   // Gelen mÃ¼sterileri dinle

  if (client) {                             // Eger bir mÃ¼steri bulursan
    Serial.println("new client");           // Seri baglanti noktasinda bir mesaj yazdir
    String currentLine = "";                // Ä°stemciden gelen verileri tutmak icin bir string yap.
    while (client.connected()) {            // mÃ¼steri baglÄ±yken dongÃ¼
      if (client.available()) {             // istemciden okunacak bayt varsa,
        char c = client.read();             // sonra bir byte oku
        Serial.write(c);                    // seri monitorde yazdir.
        if (c == '\n') {                    // bayt bir yeni satir ise

          // GeÃ§erli satÄ±r boÅŸsa, art arda iki yeni satÄ±r karakteriniz vardÄ±r.
          // Ä°stemci HTTP isteÄŸinin sonu budur, bu nedenle bir yanÄ±t gÃ¶nderin:
          if (currentLine.length() == 0) {
            // HTTP baÅŸlÄ±klarÄ± her zaman bir yanÄ±t koduyla baÅŸlar (Ã¶rneÄŸin, HTTP / 1.1 200 OK)
            // ve bir iÃ§erik tÃ¼rÃ¼; bÃ¶ylece mÃ¼ÅŸteri ne geleceÄŸini bilir, sonra boÅŸ bir satÄ±r:

            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // HTTP yanÄ±tÄ±nÄ±n iÃ§eriÄŸi baÅŸlÄ±ÄŸÄ± izler:
            client.print("Click <a href=\"/A\">here</a> A bolgesine git<br>");
            client.print("Click <a href=\"/B\">here</a> B bolgesine git<br>");
            client.print("Click <a href=\"/C\">here</a> C bolgesine git<br>");
            
            // HTTP yanÄ±tÄ± baÅŸka bir boÅŸ satÄ±rla bitiyor:
            client.println();
            // while dÃ¶ngÃ¼sÃ¼nden Ã§Ä±kmak:
            break;
          } else {    // eÄŸer yeni bir hattÄ±nÄ±z varsa, currentLine Ã¶ÄŸesini temizleyin:
            currentLine = "";
          }
        } else if (c != '\r') {  // satÄ±r baÅŸÄ± karakterinden baÅŸka bir ÅŸey varsa,
          currentLine += c;      // currentLine'nin sonuna ekle
        }

  
        if(currentLine.endsWith("GET /A")){
             bolge = 1;
             while(bolge == 1) {
          Serial.println("while A");
            olcumeBasla();
            if(mesafe>20)  {               
          if(digitalRead(solsensor) == 0 && digitalRead(ortasensor) == 1 && digitalRead(sagsensor) == 0 ){
              Serial.println("while A.1");
              ileri();
              delay(60);
              if(digitalRead(solkonumsensor) == 1 && digitalRead(sagkonumsensor) == 0){
                Serial.println("while A.2");
                dur();
                bolge=0;
                Serial.println("bolge=0");
              }
          }
          if(digitalRead(solsensor) == 0 && digitalRead(ortasensor) == 0 && digitalRead(sagsensor) == 1 ){
              Serial.println("while A.3");
              sag();
              delay(60);
              if(digitalRead(solkonumsensor) == 1 && digitalRead(sagkonumsensor) == 0){
                Serial.println("while A.4");
                dur();
                bolge=0;
                Serial.println("bolge=0");
              }
          }
          if(digitalRead(solsensor) == 1 && digitalRead(ortasensor) == 0 && digitalRead(sagsensor) == 0 ){
              Serial.println("while A.5");
              sol();
              delay(60);
              if(digitalRead(solkonumsensor) == 1 && digitalRead(sagkonumsensor) == 0){
                Serial.println("while A.6");
                dur();
                bolge=0;                
                Serial.println("bolge=0");
              }
          }
      }
    }
    }
        else if(currentLine.endsWith("GET /B")){
             bolge = 2;
             while(bolge == 2) {
          Serial.println("while B");
          olcumeBasla();
          if(mesafe>20){
          if(digitalRead(solsensor) == 0 && digitalRead(ortasensor) == 1 && digitalRead(sagsensor) == 0 ){
              Serial.println("while B.1");
              ileri();
              delay(60);
              if(digitalRead(sagkonumsensor) == 1 && digitalRead(solkonumsensor) == 0 ){
                Serial.println("while B.2");
                dur();
                bolge=0;                
                Serial.println("bolge=0");
              }
          }
          if(digitalRead(solsensor) == 0 && digitalRead(ortasensor) == 0 && digitalRead(sagsensor) == 1 ){
              Serial.println("while B.3");
              sag();
              delay(60);
              if(digitalRead(sagkonumsensor) == 1 && digitalRead(solkonumsensor) == 0 ){
                Serial.println("while B.4");
                dur();
                bolge=0;               
                Serial.println("bolge=0");
              }
          }
          if(digitalRead(solsensor) == 1 && digitalRead(ortasensor) == 0 && digitalRead(sagsensor) == 0 ){
              Serial.println("while B.5");
              sol();
              delay(60);
              if(digitalRead(sagkonumsensor) == 1 && digitalRead(solkonumsensor) == 0 ){
                Serial.println("while B.6");
                dur();
                bolge=0;
                Serial.println("bolge=0");
              }
          }
          }
      }
    }
        else if(currentLine.endsWith("GET /C")){
             bolge = 3; 
             while(bolge == 3) {
          Serial.println("while C");
          olcumeBasla();
          if(mesafe>20){
          if(digitalRead(solsensor) == 0 && digitalRead(ortasensor) == 1 && digitalRead(sagsensor) == 0 ){
              Serial.println("while C.1");
              ileri();
              delay(60);
              if(digitalRead(solkonumsensor) == 1 && digitalRead(sagkonumsensor) == 1 ){
                Serial.println("while C.2");
                dur();
                bolge=0;             
                Serial.println("bolge=0");
              }
          }
          if(digitalRead(solsensor) == 0 && digitalRead(ortasensor) == 0 && digitalRead(sagsensor) == 1 ){
              Serial.println("while C.3");
              sag();
              delay(60);
              if(digitalRead(solkonumsensor) == 1 && digitalRead(sagkonumsensor) == 1 ){
                Serial.println("while C.4");
                dur();
                bolge=0;               
                Serial.println("bolge=0");
              }
          }
          if(digitalRead(solsensor) == 1 && digitalRead(ortasensor) == 0 && digitalRead(sagsensor) == 0 ){
              Serial.println("while C.5");
              sol();
              delay(60);
              if(digitalRead(solkonumsensor) == 1 && digitalRead(sagkonumsensor) == 1 ){
                Serial.println("while C.6");
                dur();
                bolge=0;               
                Serial.println("bolge=0");
              }
          }
          }
      } 
    }
        else bolge=0;
        
    }
    }
    
    // baÄŸlantÄ±yÄ± kapat:
    client.stop();
    Serial.println("client disonnected");
  }
}
void olcumeBasla()
{
baslangicTime = 0;
digitalWrite(trigpin,HIGH);
delayMicroseconds(10);
digitalWrite(trigpin,LOW);
sure = pulseIn(echopin,HIGH);
mesafe = (sure/2) / 29.1;
}
void detect ()
{
   if(mesafe <= 20){
         digitalWrite(motorR1, LOW);
         digitalWrite(motorR2, LOW);
         digitalWrite(motorL1, LOW);
         digitalWrite(motorL2, LOW);
         Serial.println("Engel algilandi");
        }
}
void ileri()
{
  digitalWrite(motorR1, HIGH);
  digitalWrite(motorR2, LOW);
  digitalWrite(motorL1, HIGH);
  digitalWrite(motorL2, LOW);
  for (i = 180; i <= 183; i += 1) {         //sol motor hız ayarı
  ledcWrite(0, i);
  delay(20);
    }
  for (k = 180; k <= 183; k += 1) {         //sag motor hız ayarı
  ledcWrite(0, k);
  delay(20);
   }
}
void sol()
{
  digitalWrite(motorR1, LOW);
  digitalWrite(motorR2, HIGH);
  digitalWrite(motorL1, HIGH);
  digitalWrite(motorL2, LOW);
  for (i = 215; i <= 217; i += 1) {    //sol motor hız ayarı
  ledcWrite(0, i);
  delay(20);
    }
  for (k = 160; k <= 163; k += 1) {         //sag motor hız ayarı
  ledcWrite(0, k);
  delay(20);
   }
}
void sag()
{
  digitalWrite(motorR1, HIGH);
  digitalWrite(motorR2, LOW);
  digitalWrite(motorL1, LOW);
  digitalWrite(motorL2, HIGH);
  for (k = 215; k <= 217; k += 1) {   //sag motor hız ayarı
  ledcWrite(0, k);
  delay(20);
   }
  for (i = 160; i <= 163; i += 1) {         //sol motor hız ayarı
  ledcWrite(0, i);
  delay(20);
    }
}
void dur()
{
  digitalWrite(motorR1, LOW);
  digitalWrite(motorR2, LOW);
  digitalWrite(motorL1, LOW);
  digitalWrite(motorL2, LOW);
}
