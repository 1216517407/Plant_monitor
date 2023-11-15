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
- DHT from Adafruit ( [Official Document](https://github.com/Khuuxuanngoc/DHT-sensor-library) )
- DHT_U from Adafruit ( [Official Document](https://github.com/Khuuxuanngoc/DHT-sensor-library) )

### 1.3 Plant

Since you're going to make a plant monitor, plants are definitely essential!

I used hyacinths for my monitoring. Hyacinths are dormant in summer, and in the autumn or winter, they take root and sprout new shoots, but do not emerge from the soil. After a cold winter, they begin to stretch and grow to form stems and leaves in early spring (at temperatures around 4°C to 15°C) and flower mainly in March-April. Hyacinths prefer a sunny environment with at least 6 hours of direct sunlight. Adequate light supports photosynthesis, promoting growth and flowering. They are also sensitive to temperature and humidity, thriving best in warm, moist conditions. In caring for hyacinths, it's important to maintain consistent soil moisture, especially during their active growth in spring, and to provide a well-draining, loamy soil with a slightly acidic to neutral pH (6.0-7.0).


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

### 2.2 02_test_MQTT

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

### 2.3 03_test_soil_sensor

This section of the code will be used to test if the soil sensor circuit works properly.

After uploading the code to the soldered sensor, you will be able to see a line graph of the changes in the Serial Plotter if everything is working correctly.

### 2.4 04_test_DHT

This section will be used to test if the DHT sensor is working properly.

If everything is installed correctly, then the current temperature as well as the humidity will be displayed in the serial monitor.

### 2.5 05_DHT22_MQTT

The content of this section will be the **core code** of the plant monitor.

After going through the previous test files, you should be able to confirm that everything is fine with your sensor and that it works the way you envisioned. It's time to start working on the core code for our plant sensor!

Before you start, please copy the file ( that is `arduino_secrets.h` ) created when testing MQTT that stores your sensitive information to the current folder.

After compiling the code and uploading it to the ESP8266, if everything is working properly, you will be able to see that the temperature, humidity, and soil moisture data is being updated under the topic you created when you open MQTT Explorer.

Congratulations! :tada::tada::tada: This is the end of the first part about the ESP8266 setup!

Let's get into the setup of your Raspberry Pi!

## 3. Raspberry Pi Setup

### 3.1 System Setup

Download the [official imager](https://www.raspberrypi.com/software/) from the Raspberry Pi website and set the WiFi name and password in the system settings and start the SSH service. Wait patiently for the flashing to finish, then you can install the TF card in the memory card slot on the back of the Raspberry Pi.

### 3.2 Get the IP Address

The next part will be one of the **biggest difficulties** in setting up the Raspberry Pi. In this section, we need to get the IP address of the Raspberry Pi and connect remotely using SSH service.

In order to cope with different scenarios, I will document several methods that I have used myself.

1. **Easiest**: Connect the Raspberry Pi to the monitor using a cable with mini HDMI at one end to allow for GUI connectivity.
2. If you connect your Raspberry Pi to your home WiFi, then you can use [Advanced IP Scanner](https://www.advanced-ip-scanner.com/) to scan for IP addresses and corresponding devices within a certain range. It is very easy to find the IP address of the Raspberry Pi by this method.
3. On a Windows device, you can share your connected WiFi network to the Raspberry Pi via the RJ45 network cable port, at which point you just need to use a network cable to connect the network cable port on the Raspberry Pi to the network cable port on the Windows device. Once connected, open the Windows terminal and enter the following command: `arp -a` to get the IP address assigned to the Raspberry Pi. But please note that this IP is not the IP of the Raspberry Pi under WiFi, so we need to use SSH to connect to this IP address and go into the Raspberry Pi's system to manually get it connected to WiFi and use the `ifconfig` command to see the IP address after connecting to WiFi.
4. Use `ssh pi@your_host_name.local` command directly. If a Raspberry Pi with the same name exists on the same network, the connection may fail.

To this point, you should have been able to connect to your own Raspberry Pi remotely. :tada::tada::tada: Next, you will need to install some software on your Raspberry Pi to handle the data.

### 3.3 Installations

#### 3.3.1 System Upgrade

Before you install any other software, upgrade your Raspberry Pi's system to the latest version using the following command:
```
sudo apt update
sudo apt upgrade -y
sudo reboot
```

#### 3.3.2 InfluxDB Installation

You can check the official website of InfluxDB [here](https://www.influxdata.com/).

First add the Influx key to ensure secure downloads, and also add the repository to the source list for convenient downloads.

```
wget -q https://repos.influxdata.com/influxdata-archive_compat.key

echo '393e8779c89ac8d958f81f942f9ad7fb82a25e133faddaf92e15b16e6ac9ce4c influxdata-archive_compat.key' | sha256sum -c && cat influxdata-archive_compat.key | gpg --dearmor | sudo tee /etc/apt/trusted.gpg.d/influxdata-archive_compat.gpg > /dev/null

echo 'deb [signed-by=/etc/apt/trusted.gpg.d/influxdata-archive_compat.gpg] https://repos.influxdata.com/debian stable main' | sudo tee /etc/apt/sources.list.d/influxdata.list
```

With the repository added we now run another update to the package list

```
sudo apt-get update
```

The installation is finally completed with the following command:

```
sudo apt-get install influxdb2 -y
```
Finally to get InfluxDB up and running at reboot we need to enable and start it within systemctl:
```
sudo systemctl unmask influxdb.service
sudo systemctl start influxdb
sudo systemctl enable influxdb.service
```

After successfully running it, we can access InfluxDB on our computer at the following URL: `http://your_host_name.celab:8086/`

After logging in, you can use the template to create a database with information about the Raspberry Pi.

After this, a bucket named mqtt-data can be created to store the sensor data on MQTT.

#### 3.3 Telegraf and Further Installation
Please follow the steps on the [Workshop Page](https://workshops.cetools.org/codelabs/CASA0014-2-Plant-Monitor/index.html?index=..%2F..casa0014#12)