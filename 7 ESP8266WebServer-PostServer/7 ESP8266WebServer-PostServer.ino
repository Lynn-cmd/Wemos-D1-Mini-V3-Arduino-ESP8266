#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "CU_W"
#define STAPSK "Aa123456"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;
ESP8266WebServer server(80);
const int led = 2;

const String postForms = "<html>\
  <head>\
    <title>ESP8266 Web Server POST handling</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>POST plain text to /postplain/</h1><br>\
    <form method=\"post\" enctype=\"text/plain\" action=\"/postplain/\">\
      <input type=\"text\" name=\'{\"hello\": \"world\", \"trash\": \"\' value=\'\"}\'><br>\
      <input type=\"submit\" value=\"Submit\">\
    </form>\
    <h1>POST form data to /postform/</h1><br>\
    <form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/postform/\">\
      <input type=\"text\" name=\"hello\" value=\"world\"><br>\
      <input type=\"submit\" value=\"Submit\">\
    </form>\
  </body>\
</html>";

void handleRoot() {
  digitalWrite(led, HIGH);
  server.send(200, "text/html", postForms);
  digitalWrite(led, LOW);
}

void handlePlain() {
  if (server.method() != HTTP_POST) {
    digitalWrite(led, HIGH);
    server.send(405, "text/plain", "Method Not Allowed");
    digitalWrite(led, LOW);
  } else {
    digitalWrite(led, HIGH);
    server.send(200, "text/plain", "POST body was:\n" + server.arg("plain"));
    digitalWrite(led, LOW);
  }
}

void handleForm() {
  if (server.method() != HTTP_POST) {
    digitalWrite(led, HIGH);
    server.send(405, "text/plain", "Method Not Allowed");
    digitalWrite(led, LOW);
  } else {
    digitalWrite(led, HIGH);
    String message = "POST form was:\n";
    for (uint8_t i = 0; i < server.args(); i++) { message += " " + server.argName(i) + ": " + server.arg(i) + "\n"; }
    server.send(200, "text/plain", message);
    digitalWrite(led, LOW);
  }
}

void handleNotFound() {
  digitalWrite(led, HIGH);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) { message += " " + server.argName(i) + ": " + server.arg(i) + "\n"; }
  server.send(404, "text/plain", message);
  digitalWrite(led, LOW);
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("");

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

  server.on("/postplain/", handlePlain);

  server.on("/postform/", handleForm);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}