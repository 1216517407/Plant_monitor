#include <ESP8266WiFi.h> // Include the ESP8266WiFi.h library
#include <ESP8266WebServer.h> // Include the ESP8266WebServer.h library
#include <ezTime.h> // Include the ezTime.h library 
#include <PubSubClient.h> // Include the PubSubClient.h library
#include <DHT.h> // Include the DHT.h library
#include <DHT_U.h> // Include the DHT_U.h library

#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321 Define constant DHTTYPE

// Sensors - DHT22 and Nails
uint8_t DHTPin = 12;        // on Pin 2 of the Huzzah
uint8_t soilPin = 0;      // ADC or A0 pin on Huzzah
float Temperature;  // Declare variable to store temperature reading from DHT22
float Humidity;  // Declare variable to store humidity reading from DHT22
int Moisture = 1; // initial value just in case web page is loaded before readMoisture called
int sensorVCC = 13; // Define the VCC pin for the sensor
int blueLED = 2; // Define the pin connected to a blue LED
DHT dht(DHTPin, DHTTYPE);   // Initialize DHT sensor.


// Wifi and MQTT
#include "arduino_secrets.h" 
/*
**** please enter your sensitive data in the Secret tab/arduino_secrets.h
**** using format below

#define SECRET_SSID "ssid name"
#define SECRET_PASS "ssid password"
#define SECRET_MQTTUSER "user name - eg student"
#define SECRET_MQTTPASS "password";
 */

const char* ssid     = SECRET_SSID;  // Assign WiFi SSID from secret constant
const char* password = SECRET_PASS;  // Assign WiFi password from secret constant
const char* mqttuser = SECRET_MQTTUSER;  // Assign MQTT username from secret constant
const char* mqttpass = SECRET_MQTTPASS;  // Assign MQTT password from secret constant 

ESP8266WebServer server(80);  // Initialize a web server on port 80
const char* mqtt_server = "mqtt.cetools.org";  // Define MQTT server address
WiFiClient espClient;  // Create a WiFi client instance
PubSubClient client(espClient);  // Create an MQTT client instance using the WiFi client
long lastMsg = 0;  // Variable to store the timestamp of the last sent message
char msg[50];  // Buffer to store the MQTT message
int value = 0;  // Variable to store a value

// Date and time
Timezone GB;



void setup() {
  // Set up LED to be controllable via broker
  // Initialize the BUILTIN_LED pin as an output
  // Turn the LED off by making the voltage HIGH
  pinMode(BUILTIN_LED, OUTPUT);     
  digitalWrite(BUILTIN_LED, HIGH);  

  // Set up the outputs to control the soil sensor
  // switch and the blue LED for status indicator
  pinMode(sensorVCC, OUTPUT); 
  digitalWrite(sensorVCC, LOW);
  pinMode(blueLED, OUTPUT); 
  digitalWrite(blueLED, HIGH);

  // open serial connection for debug info
  Serial.begin(115200);
  delay(100);

  // start DHT sensor
  pinMode(DHTPin, INPUT);
  dht.begin();

  // run initialisation functions
  startWifi();
  startWebserver();
  syncDate();  // Synchronize the date and time

  // start MQTT server
  client.setServer(mqtt_server, 1884);
  client.setCallback(callback);  // Set the callback function for MQTT messages

}

void loop() {
  // handler for receiving requests to webserver
  server.handleClient();

  // Check if a new minute has started using the minuteChanged() function
  if (minuteChanged()) {
    readMoisture();
    sendMQTT();
    Serial.println(GB.dateTime("H:i:s")); // UTC.dateTime("l, d-M-y H:i:s.v T")
  }
  
  client.loop();  // Maintain the MQTT connection and check for incoming messages
}

// This function reads the moisture level from the soil sensor
void readMoisture(){
  
  // power the sensor
  digitalWrite(sensorVCC, HIGH);  // Turn on the VCC for the sensor
  digitalWrite(blueLED, LOW);  // Turn on the blue LED (assuming common cathode connection)
  delay(100);
  // read the value from the sensor:
  // Moisture = analogRead(soilPin);
  Moisture = map(analogRead(soilPin), 0,200, 0, 100);         
  digitalWrite(sensorVCC, LOW);  // Turn off the VCC for the sensor
  digitalWrite(blueLED, HIGH);  // Turn off the blue LED
  delay(100);
  Serial.print("Wet ");
  Serial.println(Moisture);   // read the value from the nails
}

void startWifi() {
  // We start by connecting to a WiFi network
  Serial.println();  // Print a newline to the serial monitor for better readability
  Serial.print("Connecting to ");
  Serial.println(ssid);  // Print the WiFi SSID to the serial monitor
  WiFi.begin(ssid, password);  // Begin the WiFi connection using the provided SSID and password

  // check to see if connected and wait until you are
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void syncDate() {
  // get real date and time
  waitForSync();  // Wait until the time synchronization is complete
  Serial.println("UTC: " + UTC.dateTime());  // Print the current UTC date and time to the serial monitor
  GB.setLocation("Europe/London");
  Serial.println("London time: " + GB.dateTime());

}

// This function sends the sensor data to an MQTT broker
void sendMQTT() {

  if (!client.connected()) {
    // This function sends the sensor data to an MQTT broker
    reconnect();  // Attempt to reconnect the MQTT client
  }
  client.loop();  // Maintain the MQTT connection and check for incoming messages

  Temperature = dht.readTemperature(); // Gets the values of the temperature
  snprintf (msg, 50, "%.1f", Temperature);
  Serial.print("Publish message for t: ");
  Serial.println(msg);
  client.publish("student/CASA0014/plant/ucfnyz1/temperature", msg);

  Humidity = dht.readHumidity(); // Gets the values of the humidity
  snprintf (msg, 50, "%.0f", Humidity);
  Serial.print("Publish message for h: ");
  Serial.println(msg);
  client.publish("student/CASA0014/plant/ucfnyz1/humidity", msg);

  //Moisture = analogRead(soilPin);   // moisture read by readMoisture function
  snprintf (msg, 50, "%.0i", Moisture);
  Serial.print("Publish message for m: ");
  Serial.println(msg);
  client.publish("student/CASA0014/plant/ucfnyz1/moisture", msg);

}

// This function is called when a message is received on an MQTT topic the client is subscribed to
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    
    // Attempt to connect with clientID, username and password
    if (client.connect(clientId.c_str(), mqttuser, mqttpass)) {
      Serial.println("connected");
      // ... and resubscribe
      client.subscribe("student/CASA0014/plant/ucfnyz1/inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// This function starts the web server
void startWebserver() {
  // when connected and IP address obtained start HTTP server  
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");  
}

// This function handles the root URL ("/") request
void handle_OnConnect() {
  Temperature = dht.readTemperature(); // Gets the values of the temperature
  Humidity = dht.readHumidity(); // Gets the values of the humidity
  server.send(200, "text/html", SendHTML(Temperature, Humidity, Moisture));
}

// This function handles unknown URLs
void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

// This function generates an HTML string with the provided sensor data
String SendHTML(float Temperaturestat, float Humiditystat, int Moisturestat) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>ESP8266 DHT22 Report</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr += "p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<div id=\"webpage\">\n";
  ptr += "<h1>ESP8266 Huzzah DHT22 Report</h1>\n";

  ptr += "<p>Temperature: ";
  ptr += (int)Temperaturestat;
  ptr += " C</p>";
  ptr += "<p>Humidity: ";
  ptr += (int)Humiditystat;
  ptr += "%</p>";
  ptr += "<p>Moisture: ";
  ptr += Moisturestat;
  ptr += "</p>";
  ptr += "<p>Sampled on: ";
  ptr += GB.dateTime("l,");
  ptr += "<br>";
  ptr += GB.dateTime("d-M-y H:i:s T");
  ptr += "</p>";

  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
