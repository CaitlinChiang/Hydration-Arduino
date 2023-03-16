#include <WiFiNINA.h>
#include "arduino_secrets.h"
#include "Firebase_Arduino_WiFiNINA.h"

#define FIREBASE_HOST "pds2023-b02f8-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "kejP5nK2Z45u5v6A6ZTRl7HPfejcpvghx3T1Npv1"
FirebaseData firebaseData;
String path = "/NEW";

const int analogInPin = A0;
int sensorValue = 0;
  
// WIFI CONFIGURATION
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // the Wifi radio's status

void setup() {
  Serial.begin(9600);
  while (!Serial);
 
  while (status != WL_CONNECTED) {
    connectToWifi();
  }

  Serial.println("You're connected to the network!");
  printWifiData();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, SECRET_SSID, SECRET_PASS);
  Firebase.reconnectWiFi(true); // RECONNECT WHEN WIFI IS DOWN
  
  Serial.println("Setup complete!");
}

void loop() {
  sensorValue = analogRead(analogInPin);
  
  Serial.print("sensor = ");
  Serial.print(sensorValue);
  Serial.print("\n");

  writeToFirebase(sensorValue, "/currentLevel");
    
  delay(2);
}

void connectToWifi(){
    Serial.print("Connecting to network: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);

    delay(10000);
}

void printWifiData() {
  Serial.println("----------------------------------------");
  Serial.println("Board Information:");
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  Serial.println();
  Serial.println("Network Information:");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
  Serial.println("----------------------------------------");
}

void writeToFirebase(int t, String subpath){
   if (Firebase.setInt(firebaseData, path + subpath, t))
    {
      Serial.println("New entry logged");
    }
    else
    {
      Serial.println("ERROR : " + firebaseData.errorReason());
      Serial.println();
    }
}
