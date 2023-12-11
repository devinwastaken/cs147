#include <WiFiEsp.h>
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

char ssid[] = ;   // your network SSID (name)
char pass[] = ;     // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

char serverEndpoint[] = ;
int serverPort = 3000;

const int tempPin = A0;
const int photoPin = A5;

const long readInterval = 1000;
unsigned long lastReadTime = 0;
const long uploadInterval = 60000;
unsigned long lastUploadTime = 0;

float tempAvg = 0;
unsigned int tempN = 0;

float photoAvg = 0;
int photoN = 0;

WiFiEspClient client;


void setup() {
  // initialize serial for debugging
  Serial.begin(9600);
  // initialize serial for ESP module
  Serial1.begin(9600);
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }
  // you're connected now, so print out the data
  Serial.println("You're connected to the network");
  printWifiStatus();
}

void loop() {
  if (millis() - lastReadTime > readInterval) {
    doReadSensors();
    lastReadTime = millis();
  }

  if (millis() - lastUploadTime > uploadInterval) {
    uploadData();
    lastUploadTime = millis();
  }
}


void doReadSensors() {
  tempAvg = updateAvg(tempAvg, getTempC(), &tempN);
  Serial.print("Avg temp c: ");
  Serial.print(tempAvg);

  photoAvg = updateAvg(photoAvg, analogRead(photoPin), &photoN);
  Serial.print(", Avg photo: ");
  Serial.println(photoAvg);
}


float getTempC() {
  int tempVal = analogRead(tempPin);
  float voltage = (tempVal / 1024.0) * 5.0;
  return (voltage - 0.5) * 100;
}


void uploadData() {
    Serial.println("uploading data...");
    Serial.print("\tAvg temp c: ");
    Serial.println(tempAvg);
  
    photoAvg = updateAvg(photoAvg, getTempC(), &photoN);
    Serial.print("\tAvg photo: ");
    Serial.println(photoAvg);

    httpRequest();

    tempAvg = 0;
    tempN = 0;
    photoAvg = 0;
    photoN = 0;
}


float updateAvg(float currAvg, float newVal, int* n) {
  *n += 1;
  float differential = (newVal - currAvg) / *n;
  return currAvg + differential;
}


void httpRequest() {
  // close any connection before send a new request.
  // This will free the socket on the WiFi shield
  client.stop();

  // if there's a successful connection:
  if (client.connect(serverEndpoint, serverPort)) {
    Serial.println("connecting...");
    httpRequestWrite();
  } else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
}


void httpRequestWrite() {
    char temp_header[32];
    char photo_header[32];
    sprintf(temp_header, "temperature: %s", String(tempAvg).c_str());
    sprintf(photo_header, "light: %s", String(photoAvg).c_str());

    client.println("POST / HTTP/1.1");
    client.println(temp_header);
    client.println(photo_header);
    client.println();
}


void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
