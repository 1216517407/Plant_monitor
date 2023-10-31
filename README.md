# Plant Monitor Project Outline

This document documents the process of the final project plant monitor in the CASA0014 Connected Environment  ( [Moodle Page](https://workshops.cetools.org/codelabs/CASA0014-2-Plant-Monitor/index.html?index=..%2F..casa0014#0) ) and highlights the key actions to help the reader or myself to reproduce the device in the future.

This is my first time to use Arduino,so there may be some mistakes, please feel free to point them out. And many thanks to all the professors and TA's for their help.

I hope it will help you as well as me in the future, **good luck**!

## 1. Equipments and Libraries Requirements

### 1.1 Equipments

The necessary equipments including circuit boards to complete this project will be included in this section.

- DHT22 temperature / humidity sensor $\times 1$ ( For more information about this sensor please check out [this document](https://arduinogetstarted.com/tutorials/arduino-dht22?utm_content=cmp-true). )
- Feather Huzzah ESP8266 Wifi $\times 1$  ( For more information about this board please check out [this document](https://learn.adafruit.com/adafruit-feather-huzzah-esp8266/overview). )
- CASA plant monitor shield $\times 1$ ( Used to put circuits together )
- $10k \Omega$ resistive $\times 2$
- $220 \Omega$ resistive $\times 1$
- $100 \Omega$ resistive $\times 1$
- Nails $\times 2$
- Micro usb to usb-b cable $\times 1$ ( Used to connect the ESP8266 to the computer )
- Cables $\times 2$
- Raspberry Pi $\times 1$
- Solder
- Soldering iron 


### 1.2 Necessary Libraries

In this section I will list the libraries that need to be installed for code reproduction.

- ESP8266WiFi
- ESP8266WebServer 
- ezTime ( [Official Document](https://github.com/ropg/ezTime) )
- PubSubClient ( [Official Document](https://pubsubclient.knolleary.net/) )
- DHT ( [Official Document](https://github.com/Khuuxuanngoc/DHT-sensor-library) )
- DHT_U ( [Official Document](https://github.com/Khuuxuanngoc/DHT-sensor-library) )


## 2. Files and Usage

:warning: Be sure to change the name of the `.ino` file inside the folder after changing the name of the folder to the **same name** as the folder to ensure that the program can run properly.

### 2.1 01_WiFi_module

This folder contains a file with the same name to test if the WiFi module ( ESP8266 ) works.

:warning: When executing this code, make sure that the ESP8266 is properly connected to your computer and can be correctly recognised by the IDE. If you can't connect correctly, click [here](https://workshops.cetools.org/codelabs/CASA0014-2-Plant-Monitor/index.html?index=..%2F..casa0014#2).

This section of the code is mainly used to test whether the ESP8266 can work correctly. Before executing the code, make sure you put your WiFi name and password into the code correctly.
```
const char* ssid     = "Your WiFi name";
const char* password = "Your WiFi's password";
```
If your ESP8266 module is working correctly, then `WiFi connected` along with your IP address  will appear in Serial Monitor. Serial Monitor will then update your local time in real time.

## 2.2 02_test_MQTT

This part of the code is used to test whether the ESP8266 can upload messages to the MQTT server.

Before executing the code, create a file named `arduino_secrets.h` in this directory to store sensitive information such as passwords.

The content and format are as follows:
```
#define SECRET_SSID "ssid name"
#define SECRET_PASS "ssid password"
#define SECRET_MQTTUSER "user name - eg student"
#define SECRET_MQTTPASS "password"
```
:warning::warning::warning: **Never upload this file to Github as it contains sensitive information.**

If your settings are correct, then after a successful execution you will receive a message that the connection is successful and the IP address assigned to the ESP8266. Also every five seconds a message will be sent to the MQTT and it will also print out the information it received. In addition, when it receives a message with the content `1`, the LED will light up.



