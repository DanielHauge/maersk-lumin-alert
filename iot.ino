#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <SPI.h>

// Assign output variables to GPIO pins
const int outputGreen = 5;
const int outputRed = 4;

unsigned long delayTime;

const char* ssid = "EpicStuff";
const char* password = "splendid";
bool isLock = true;


ESP8266WebServer server(80);

String GenerateMetrics() {
  String message = "";
  message += "light_measurements ";
  message += analogRead(A0);
  message += "\n";
  message += "lockStatus ";
  message += isLock;
  message += "\n";

  return message;
}

void lockUnlock() {
  isLock = !isLock;
  server.send(200, "text/plain", GenerateMetrics());
}

void lock(){
  isLock = true;
  server.send(200, "text/plain", GenerateMetrics());
}

void unlock(){
  isLock = false;
  server.send(200, "text/plain", GenerateMetrics());
}


void HandleRoot() {
  server.send(200, "text/plain", GenerateMetrics());
}
void HandleNotFound() {
  String message = "Error\n\n";
  server.send(404, "text/html", message);
}



void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;

  pinMode(outputGreen, OUTPUT);
  pinMode(outputRed, OUTPUT);
  digitalWrite(outputGreen, LOW);
  digitalWrite(outputRed, LOW);

  //Setup WIFI
  WiFi.begin(ssid, password);
  Serial.println("");

  //Wait for WIFI connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());

  server.on("/", lockUnlock);
  server.on("/lock", lock);
  server.on("/unlock", unlock);
  server.on("/metrics", HandleRoot);
  server.onNotFound(HandleNotFound);

  server.begin();
  Serial.println("HTTP server started at ip " + WiFi.localIP().toString());


  Serial.println();
}


void loop() {
  if (isLock) {
    digitalWrite(outputGreen, LOW);
    digitalWrite(outputRed, HIGH);
  } else {
    digitalWrite(outputGreen, HIGH);
    digitalWrite(outputRed, LOW);
  }

  server.handleClient();
  delay(delayTime);
}


void printValues() {
}
