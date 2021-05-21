#include <ESP8266WiFi.h>
#include <SPI.h>
#include <MFRC522.h>
#include <WiFiClientSecure.h>

#define SS_PIN 4  //D2
#define RST_PIN 5 //D1

String readString;
const char* ssid = "OnePlus 8T";
const char* password = "chandupurna";
const char* host = "script.google.com";//script.google.com:443/HTTPS
const int httpsPort = 443;
WiFiClientSecure client;
const char* fingerprint = "46 B2 C3 44 9C 59 09 8B 01 B6 F8 BD 4C FB 00 74 91 2F EF F6";
String GAS_ID = "AKfycbxSY1HeeCexZ-J8y8vyWkW7-zn0bL0HUuo5FWFpmF4DCYShX8LZiCas"; // Replace by your GAS service id

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int statuss = 0;
int out = 0;


void setup()
{
Serial.begin(9660);

WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED)
{
delay(500);

Serial.print(".");
}
//sendData(113,125); // Send test data
}

void loop()
{
//int a,b;
//a=10;
//b=9;
String a,b,c,d,e;
a = "Chandu";
b = "122004014";
c = "ECE";
d = "C";
e = "YES";


// Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.println();
  Serial.print(" UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println();
  if (content.substring(1) == "3A 67 52 A3") //change UID of the card that you want to give access
  {
    Serial.println(" Access Granted ");
    Serial.println(" Welcome Mr.Circuit ");
    delay(1000);
    Serial.println(" Have FUN ");
    Serial.println();
    statuss = 1;
    sendData(a,b,c,d,e);
}
}

void sendData(String x, String y, String z, String xy, String yz)
{
client.setInsecure();
Serial.print("connecting to ");
Serial.println(host);
if (!client.connect(host, httpsPort)) {
Serial.println("connection failed");
return;
}
if (client.verify(fingerprint, host)) {
Serial.println("certificate matches");
} else {
Serial.println("certificate doesn't match");
}
//String string_x = String(x, DEC);
//String string_y = String(y, DEC);
String string_x = x;
String string_y = y;
String string_z = z;
String string_xy = xy;
String string_yz = yz;
String url = "/macros/s/" + GAS_ID + "/exec?Name=" + string_x + "&RegistrationNumber=" + string_y + "&Program=" + string_z + "&Section=" + string_xy + "&Attendance=" + string_yz; //https://script.google.com/macros/s/yourcodehere/exec?A=1&temperature=2&humidity=3
Serial.print("requesting URL: ");
Serial.println(url);
client.print(String("GET ") + url + " HTTP/1.1\r\n" +
"Host: " + host + "\r\n" +
"User-Agent: BuildFailureDetectorESP8266\r\n" +
"Connection: close\r\n\r\n");
Serial.println("request sent");
while (client.connected()) {
String line = client.readStringUntil('\n');
if (line == "\r") {
Serial.println("headers received");
break;
}
}
String line = client.readStringUntil('\n');
/*if (line.startsWith("{"state":"success"")) {
Serial.println("esp8266/Arduino CI successfull!");
} else {
Serial.println("esp8266/Arduino CI has failed");
}*/
Serial.println("reply was:");
Serial.println("==========");
Serial.println(line);
Serial.println("==========");
Serial.println("closing connection");
delay(10000);
}
