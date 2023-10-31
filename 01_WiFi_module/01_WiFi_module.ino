#include <ESP8266WiFi.h>  // Include the ESP8266WiFi library for WiFi functionality
#include <ezTime.h>  // Include the ezTime library for time-related functionality

const char* ssid     = "Your WiFi name";
const char* password = "Your WiFi's password";
Timezone GB;  // Create a Timezone object named GB

void setup() {
  Serial.begin(115200);  // Start the serial communication at 115200 baud rate
  delay(100);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);  // Begin the WiFi connection using the SSID and password

  // While the WiFi is not connected, run the following codes
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  waitForSync();  //Wait for time synchronization

  Serial.println("UTC: " + UTC.dateTime());

  GB.setLocation("Europe/London");
  Serial.println("London time: " + GB.dateTime());  

}

void loop() {
  delay(1000);
  Serial.println(GB.dateTime("H:i:s")); // UTC.dateTime("l, d-M-y H:i:s.v T")
}