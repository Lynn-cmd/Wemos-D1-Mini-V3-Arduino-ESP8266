/*simulate RC control panel
123     ↖  ↑  ↗
456 =   ←  ㊣  →
789     ↙  ↓  ↘
L = turn left; R = turn right */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <AccelStepper.h>
#ifndef STASSID
#define STASSID "CU_W"
#define STAPSK "Aa123456"
#define dirPin 5
#define stepPin 4
#define motorInterfaceType 1
#endif

// Create a new instance of the AccelStepper class:
//< Stepper Driver, 2 driver pins required
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

const char* ssid = STASSID;
const char* password = STAPSK;
ESP8266WebServer server(80);
const int led = 2;
bool running = false;

const String postForms = 
"<!DOCTYPE html>\
<html>\
<head>\
<meta charset=\"utf-8\">\
<title>Lynn's cockpit</title>\
<style>input[type=submit]{width: 100px;height: 50px;}\
form{margin:2px;display: inline}</style>\
</head>\
<body>\
<form action=\"/1/\" method=\"post\" target=\"nm_iframe\"> <input type=\"submit\" value=\"1\"></form>\   
<form action=\"/2/\" method=\"post\" target=\"nm_iframe\"> <input type=\"submit\" value=\"2\"></form>\
<form action=\"/3/\" method=\"post\" target=\"nm_iframe\"> <input type=\"submit\" value=\"3\"></form><br>\
<form action=\"/4/\" method=\"post\" target=\"nm_iframe\"> <input type=\"submit\" value=\"4\"></form>\   
<form action=\"/5/\" method=\"post\" target=\"nm_iframe\"> <input type=\"submit\" value=\"5\"></form>\
<form action=\"/6/\" method=\"post\" target=\"nm_iframe\"> <input type=\"submit\" value=\"6\"></form><br>\
<form action=\"/7/\" method=\"post\" target=\"nm_iframe\"> <input type=\"submit\" value=\"7\"></form>\   
<form action=\"/8/\" method=\"post\" target=\"nm_iframe\"> <input type=\"submit\" value=\"8\"></form>\
<form action=\"/9/\" method=\"post\" target=\"nm_iframe\"> <input type=\"submit\" value=\"9\"></form><br>\
<form action=\"/L/\" method=\"post\" target=\"nm_iframe\"> <input type=\"submit\" value=\"L\"></form>\
<form action=\"/R/\" method=\"post\" target=\"nm_iframe\"> <input type=\"submit\" value=\"R\"></form><br>\
<iframe id=\"id_iframe\" name=\"nm_iframe\" style=\"display:none;\"></iframe>\
</body>\
</html>";
 void handle5() {
   running = !running;
  digitalWrite(dirPin, HIGH); //"Set run as clockwise
 }

void handle4() {
 running = !running;
 digitalWrite(dirPin, LOW);//"Set run as anti-clockwise
Serial.println(running);

 }

void handleRoot() {
  digitalWrite(led, HIGH);
  server.send(200, "text/html", postForms);
  digitalWrite(led, LOW);
}

void handle1() {
  for(int i=0; i < 1; i++){
    digitalWrite(led, LOW);
    delay(1000);
    digitalWrite(led, HIGH);
    delay(1000);
  }
  digitalWrite(led, LOW);
}

void handle2() {
for(int i=0;i < 2; i++){
    digitalWrite(led, LOW);
    delay(1000);
    digitalWrite(led, HIGH);
    delay(1000);
  }
  digitalWrite(led, LOW);
}

void handle3() {
for(int i=0;i < 3; i++){
    digitalWrite(led, LOW);
    delay(1000);
    digitalWrite(led, HIGH);
    delay(1000);
  }
  digitalWrite(led, LOW);
 }


void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("");

  pinMode(stepPin, OUTPUT); //Step pin as output
  pinMode(dirPin,  OUTPUT); //Direcction pin as output
  digitalWrite(stepPin, LOW); // Currently no stepper motor movement
  digitalWrite(dirPin, HIGH);  

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) { Serial.println("MDNS responder started"); }
  server.on("/", handleRoot);
  server.on("/1/", handle1);
  server.on("/2/", handle2);
  server.on("/3/", handle3);
  server.on("/4/", handle4);//Stepper Motor control
  server.on("/5/", handle5);//Stepper Motor control

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  if(running)
 {
   digitalWrite(stepPin,LOW);
   Serial.println("stepPin,LOW");
 }
  else
  {
  digitalWrite(stepPin,HIGH);
  Serial.println("stepPin,HIGH");
  for(int x = 0; x < 600; x++) // Loop 200 times
  {
      digitalWrite(stepPin,HIGH); // Output high
      delayMicroseconds(500); // Wait 1/2 a ms  340--1500
      digitalWrite(stepPin,LOW); // Output low
      delayMicroseconds(500); // Wait 1/2 a ms 340--1500
    }
    delay(2000); 
  }
}