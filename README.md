# Plant Monitor Project Outline

This document documents the process of the final project plant monitor in the CASA0014 Connected Environment and highlights the key actions to help the reader or myself to reproduce the device in the future.

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


## 2. Files
