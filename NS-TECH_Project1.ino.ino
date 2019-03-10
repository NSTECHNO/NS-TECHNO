#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>

/*
   This sample sketch demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
int inputPin=15;  //choose pin for IRsensor
int val=0;          

static const int RXPin = 12, TXPin = 13;
static const uint32_t GPSBaud = 9600;

// repace your wifi username and password
const char* ssid     = "NS-TECH";
const char* password = "nstech@123";

const char* server = "api.thingspeak.com";
String apiKey = "Y8NVL0YGDRW5HWU62";    

//unsigned long myChannelNumber = Replace it with your thingspeak channel number;
//const char * myWriteAPIKey = "Replace it with your Thingspeak write api key";

// The TinyGPS++ object
TinyGPSPlus gps;
WiFiClient  client;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud);
  Serial.println(F("DeviceExample.ino"));
  Serial.println(F("A simple demonstration of TinyGPS++ with an attached GPS module"));
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
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
  Serial.print("Netmask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
  
  pinMode (LED_BUILTIN,OUTPUT);  
  pinMode (inputPin,INPUT);
  
}

void loop()
{
   val = digitalRead(inputPin);  //read input value
   if (val==HIGH)
 {
  digitalWrite(LED_BUILTIN,LOW);
   Serial.println("Led low");//turn LED OFF
  
 }
    else
   {
   digitalWrite(LED_BUILTIN,HIGH);
    Serial.println("Led high");//LED ON
      }
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}
void displayInfo()
{
 // Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {

    double latitude = (gps.location.lat());
    double longitude = (gps.location.lng());
    
    String latbuf;
    latbuf += (String(latitude, 6));
    Serial.println(latbuf);

    String lonbuf;
    lonbuf += (String(longitude, 6));
    Serial.println(lonbuf);

      if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            //https://api.thingspeak.com/update?api_key=VPJIUIG40BV2JPNQ&field1=latbuf&field2=lonbuf 
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(latbuf);
                             postStr +="&field2=";
                             postStr += String(lonbuf);
                           postStr +="&field3=";
                             postStr +=String(val);
                             //postStr += "\r\n\r\n";
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);

                              Serial.println(latbuf);
                              Serial.println(lonbuf);
                              Serial.println(val);
                      }
        client.stop();

    //ThingSpeak.setField(1, latbuf);
    //ThingSpeak.setField(2, lonbuf);
    //ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);  
    delay(300000);
    
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}
