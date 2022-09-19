# SenseCAP K1111 - The Sensor Prototype Kit Overview

Helium has gathered a team with a diverse, yet complementary, skillset within radio and hardware, manufacturing, distributed systems, peer-to-peer and blockchain technologies. Helium Network is essentially a decentralized, LoRaWAN-based network. LoRaWAN combines the advantages of both Wi-Fi and cellular networks. It provides an efficient, flexible and economical connectivity solution for IoT applications indoors and outdoors as well as installed in public, private or mixed network areas.

If you have a LoRaWAN-capable device and want to send small amounts of information to the internet, this does the job pretty well.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/K1111/logo.png"/></div>

## Introduction

In the digital age where IoT and big data are becoming more prevalent, organizations of all sizes want to accelerate the process of digital transformation by bringing computing to the edge. The **SenseCAP - The Sensor Prototype Kit** was designed to speed up this process. Unlike other tool kits, the SenseCAP K1100 comes with all the essential sensors and devices, including a Wio terminal, an AI vision camera, and a LoRaWAN module, allowing users of all levels and backgrounds to explore and co-create endless sensors for real-world challenges. 

There are three versions of the **SenseCAP - The Sensor Prototype Kit**, **K1101, K1100 and K1111**, and the similarities and differences between them can be found in the table below.

<table align="center">
	<tr>
		<td align="center"> </td>
	    <td align="center"><b>K1101</b></td>
	    <td align="center"><b>K1100</b></td>
		<td align="center" bgcolor="8FBC8F"><b>K1111</b></td>
	</tr>
	<tr>
		<td align="center"><b>Product Application Direction</b></td>
	    <td align="left">The K1101 kit is intended for use by Chinese users. Limited by the LoRa network in China, the application direction of this kit is mainly for <b>WiFi</b> usage.</td>
	    <td align="left">The K1100 kit is open to users worldwide. He combines the TTN and Helium platforms with deep integration of LoRaWAN, allowing you to use IoT in any area with LoRaWAN coverage. the main direction of the kit is <b>TTN</b> with a large number of application groups.</td>
	    <td align="center" bgcolor="8FBC8F">The K1111 suite is primarily aimed at deep users of Helium. The use of this kit will require that you have <b>Helium</b> network coverage nearby as well as a Helium account.</td>
	</tr>
	<tr>
		<td align="center"><b>Programming Platform Support</b></td>
	    <td align="center">Arduino</td>
	    <td align="center">Arduino</td>
	    <td align="center" bgcolor="8FBC8F">Arduino</td>
	</tr>
	<tr>
		<td align="center"><b>Quick Start Platform</b></td>
	    <td align="center">Blynk</td>
	    <td align="center">SenseCAP Console</td>
	    <td align="center" bgcolor="8FBC8F">Helium Console</td>
	</tr>
	<tr>
		<td align="center"><b>Wiki focus</b></td>
	    <td align="left">Access the IoT platform via WiFi</td>
	    <td align="left">Get the sensor values from the Wio Terminal device side and send the values to TTN via LoRaWAN</td>
	    <td align="left" bgcolor="8FBC8F">Send sensor values to Helium and integrate to each cloud platform via Helium</td>
	</tr>
	<tr>
		<td align="center"><b>Parts List</b></td>
	    <td align="left"><b>1 x</b> Wio Terminal</br><b>1 x</b> Grove LoRa E5</br><b>1 x</b> Grove Soil Moisture Sensor</br><b>1 x</b> Grove VOC and eCO2 Gas Sensor SGP30</br><b>1 x</b> Grove Temp&Humi Sensor(SHT40)</br><b>1 x</b> Grove Vision AI Module</br><b>4 x</b> Grove Cable</td>
	    <td align="left">Parts list is the same as K1101. </br><b>(In terms of software, only the K1100 supports access to the SenseCAP platform)</b></td>
	    <td align="left" bgcolor="8FBC8F">Parts list is the same as K1101.</td>
	</tr>
</table>

!!!Tip
	Looking at the list of kits themselves, you can use all of the Wikis to complete the projects inside any of the kits you purchase. There is one exception, however, the **SenseCAP console is only available for the K1100 Global Edition**.

## <span id="jump1">Overview</span>

In this tutorial, we will describe how to use the Wio Terminal with the Grove-LoRa E5 module to connect with the Helium, visualize real-time sensor data from your Azure IoT hub in a web application.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/Wio-Terminal-Developer-for-helium/3.jpg"/></div>


Below is the navigation bar for each module.

<table align="center">
	<tr>
		<td colspan="2" bgcolor="8FBC8F"><b>Basic Chapters</b></td>
	</tr>
	<tr>
	    <td align="center"><p style="text-align:center;"><img src="https://files.seeedstudio.com/wiki/Wio-Terminal-Developer-for-helium/142.png" alt="pir" width="auto" height="230"></p></td>
	    <td align="left"><strong>Preliminary Preparation</strong></br></br>This section will cover the preparation before the tutorial starts. This includes preparing the environment for use with Arduino and Wio Terminal.</br></br><a href="https://wiki.seeedstudio.com/K1111-Preliminary-Preparation/">Jump to chapter ></a></td>
	</tr>
	<tr>
	    <td align="center"><p style="text-align:center;"><img src="https://files.seeedstudio.com/wiki/K1111/1.png" alt="pir" width="500" height="auto"></p></td>
	    <td align="left"><strong>Quick Start Guide</strong></br></br>This chapter will take you through the Helium platform to get the most intuitive visualization of your data in the shortest amount of time.</br></br><a href="https://wiki.seeedstudio.com/K1111-Quick-Start-Guide/">Jump to chapter ></a></td>
	</tr>
	<tr>
		<td colspan="2" bgcolor="8FBC8F"><b>Arduino Programming Sensors Section</b></td>
	</tr>
	<tr>
	    <td align="center"><p style="text-align:center;"><img src="https://files.seeedstudio.com/wiki/Wio-Terminal/img/WioT-Hardware-OverviewNew.png" alt="pir" width="500" height="auto"></p></td>
	    <td align="left"><strong>Wio Terminal Light Sensor & Grove LoRa E5</strong></br></br>This section describes how to use the Wio Terminal Light Sensor to acquire data and connect to the LoRa network via the Grove LoRa E5 in preparation for connecting to the Helium.</br></br><a href="https://wiki.seeedstudio.com/K1111-Light-Sensor-Grove-LoRa-E5/">Jump to chapter ></a></td>
	</tr>
	<tr>
	    <td align="cent er"><p style="text-align:center;"><img src="https://files.seeedstudio.com/wiki/Wio-Terminal/img/WT-GROVE.jpeg" alt="pir" width="500" height="auto"></p></td>
	    <td align="left"><strong>Wio Terminal IMU Sensor & Grove LoRa E5</strong></br><br>This section describes how to use the Wio Terminal IMU Sensor to acquire data and connect to the LoRa network via the Grove LoRa E5 in preparation for connecting to the Helium.</br></br><a href="https://wiki.seeedstudio.com/K1111-IMU-Sensor-Grove-LoRa-E5/">Jump to chapter ></a></td>
	</tr>
	<tr>
	    <td align="cent er"><p style="text-align:center;"><img src="https://files.seeedstudio.com/wiki/Grove_Moisture_Sensor/images/Moisture_sensor_.jpg" alt="pir" width="800" height="auto"></p></td>
	    <td align="left"><strong>Grove Soil Moisture Sensor & Grove LoRa E5</strong></br></br>This section describes how to use the Grove Soil Moisture Sensor to acquire data and connect to the LoRa network via the Grove LoRa E5 in preparation for connecting to the Helium.</br></br><a href="https://wiki.seeedstudio.com/K1111-Soil-Moisture-Sensor-Grove-LoRa-E5/">Jump to chapter ></a></td>
	</tr>
	<tr>
	    <td align="cent er"><p style="text-align:center;"><img src="https://files.seeedstudio.com/wiki/Grove-VOC_and_eCO2_Gas_Sensor-SGP30/img/IMG_0012a.jpg" alt="pir" width="180" height="auto"></p></td>
	    <td align="left"><strong>Grove VOC and eCO2 Gas Sensor & Grove LoRa E5</strong></br></br>This section describes how to use the Grove VOC and eCO2 Gas Sensor to acquire data and connect to the LoRa network via the Grove LoRa E5 in preparation for connecting to the Helium.</br></br><a href="https://wiki.seeedstudio.com/K1111-VOC-and-eCO2-Gas-Sensor-Grove-LoRa-E5/">Jump to chapter ></a></td>
	</tr>
	<tr>
	    <td align="cent er"><p style="text-align:center;"><img src="https://files.seeedstudio.com/wiki/Wio-Terminal-Developer-for-helium/SHT40.jpg" alt="pir" width="180" height="auto"></p></td>
	    <td align="left"><strong>Grove Temp&Humi Sensor (SHT40) & Grove LoRa E5</strong></br></br>This section describes how to use the Grove Temp&Humi Sensor (SHT40) to acquire data and connect to the LoRa network via the Grove LoRa E5 in preparation for connecting to the Helium.</br></br><a href="https://wiki.seeedstudio.com/K1111-Temp-Humi-Sensor-Grove-LoRa-E5/">Jump to chapter ></a></td>
	</tr>
	<tr>
	     <td align="cent er"><p style="text-align:center;"><img src="https://files.seeedstudio.com/wiki/Wio-Terminal-Developer-for-helium/camera.jpg" alt="pir" width="180" height="auto"></p></td>
	    <td align="left"><strong>Grove Vision AI Module & Grove LoRa E5</strong></br></br>This section describes how to use the Grove Vision AI Module to acquire data and connect to the LoRa network via the Grove LoRa E5 in preparation for connecting to the Helium.</br></br><a href="https://wiki.seeedstudio.com/K1111-Vision-AI-Module-Grove-LoRa-E5/">Jump to chapter ></a></td>
	</tr>
	<tr>
		<td colspan="2" bgcolor="8FBC8F"><b>Helium Section</b></td>
	</tr>
	<tr>
	    <td align="cent er"><p style="text-align:center;"><img src="https://files.seeedstudio.com/wiki/Wio-Terminal-Developer-for-helium/178.jpeg" alt="pir" width="500" height="auto"></p></td>
	    <td align="left"><strong>Helium Introduction</strong></br></br>In this chapter, we will introduce the Helium console controls that we use to get a first impression of the Helium console.</br></br><a href="https://wiki.seeedstudio.com/Helium-Introduction">Jump to chapter ></a></td>
	</tr>
	<tr>
	    <td align="cent er"><p style="text-align:center;"><img src="https://files.seeedstudio.com/wiki/Wio-Terminal-Developer-for-helium/helium.png" alt="pir" width="500" height="auto"></p></td>
	    <td align="left"><strong>Connecting to Helium</strong></br></br>This section describes how to configure Helium so that sensor data can be successfully uploaded to and displayed in Helium.</br></br><a href="https://wiki.seeedstudio.com/Connecting-to-Helium">Jump to chapter ></a></td>
	</tr>
	<tr>
		<td colspan="2" bgcolor="8FBC8F"><b>Cloud Platform Section</b></td>
	</tr>
	<tr>
	    <td align="cent er"><p style="text-align:center;"><img src="https://files.seeedstudio.com/wiki/Wio-Terminal-Developer-for-helium/azure.png" alt="pir" width="500" height="auto"></p></td>
	    <td align="left"><strong>Helium integrate into Azure IoT Hub</strong></br></br>This section describes how to configure Microsoft Azure and integrate with Helium to upload data to the Microsoft Azure platform.</br></br><a href="https://wiki.seeedstudio.com/Integrate-into-Azure-IoT-Hub">Jump to chapter ></a></td>
	</tr>
    <tr>
	    <td align="cent er"><p style="text-align:center;"><img src="https://files.seeedstudio.com/wiki/Wio-Terminal-Developer-for-helium/webapp.png" alt="pir" width="500" height="auto"></p></td>
	    <td align="left"><strong>Configuring Web APP Visualization</strong></br></br>This section describes how to visualize the data information received by Microsoft Azure using the free Web APP integration.</br></br><a href="https://wiki.seeedstudio.com/Configuring-Web-APP-Visualization">Jump to chapter ></a></td>
	</tr>
</table>

## Preliminary Preparation

### Connection

In this routine, we need to connect to a nearby LoRa gateway with the help of Grove LoRa E5. We need to configure the Grove port on the right side of the Wio Terminal as a soft serial port to receive AT commands.

<div align=center><img width = 600 src="https://files.seeedstudio.com/wiki/Wio-Terminal-Developer-for-helium/wiolora.jpg"/></div>

!!!Note
    Why not use the Grove port on the left?
    > The Grove interface on the left is IIC capable, and we use the IIC interface for most sensors, so keeping it is a better solution.


### Software preparation

**Step 1.** You need to Install an Arduino Software.

<p style=":center"><a href="https://www.arduino.cc/en/Main/Software" target="_blank"><div align=center><img width=600 src="https://files.seeedstudio.com/wiki/Seeeduino_Stalker_V3_1/images/Download_IDE.png" /></div></a></p>

**Step 2.** Launch the Arduino application.

<div align=center><img width=800 src="https://files.seeedstudio.com/wiki/seeed_logo/arduino.jpg"/></div>

**Step 3.** Add Wio Terminal to the Arduino IDE.

Open your Arduino IDE, click on `File > Preferences`, and copy below url to Additional Boards Manager URLs:

```
https://files.seeedstudio.com/arduino/package_seeeduino_boards_index.json
```

<div align=center><img width=800 src="https://files.seeedstudio.com/wiki/Wio-Terminal/img/Boardurl.png"/></div>

Click on `Tools > Board > Board Manager` and Search **Wio Terminal** in the Boards Manager.

<div align=center><img width=800 src="https://files.seeedstudio.com/wiki/Wio-Terminal/img/addBoard.png"/></div>


**Step 4.** Select your board and port

You'll need to select the entry in the `Tools > Board` menu that corresponds to your Arduino. Selecting the **Wio Terminal**.

<div align=center><img width=800 src="https://files.seeedstudio.com/wiki/Wio-Terminal/img/selectBoard.png"/></div>

Select the serial device of the Wio Terminal board from the `Tools -> Port` menu. This is likely to be COM3 or higher (COM1 and COM2 are usually reserved for hardware serial ports). To find out, you can disconnect your Wio Terminal board and re-open the menu; the entry that disappears should be the Arduino board. Reconnect the board and select that serial port.

!!!Tip
	For Mac User, it will be something like `/dev/cu.usbmodem141401`.

	If you are not able to upload the sketch, mostly it's because Arduino IDE was not able to put Wio Terminal to bootloader mode. (Because MCU was halted or your program handling USB) Workaround is putting your Wio Terminal to bootloader mode manually.

	<div align=center><img width=400 src="https://files.seeedstudio.com/wiki/Wio-Terminal/img/Wio-Terminal-Bootloader.png"/></div>


### About Grove LoRa E5

Grove LoRa-E5 embedded with LoRa-E5 STM32WLE5JC, powered by ARM Cortex M4 ultra-low-power MCU core and LoRa SX126x, is an easy-to-use wireless radio module, supporting LoRaWAN protocol on the EU868 & US915 frequency and (G)FSK, BPSK, (G)MSK, LoRa modulations.

Grove - LoRa-E5 can endow your development boards' strong features of ultra-long transmitting range, great performance, and high efficiency by easiliy plug and play with Grove connector on board. By connecting Grove - LoRa-E5 to your development boards, your devices is able to communicate with and control LoRa-E5 conveniently by AT command through UART connection. 

<div align=center><img width= 800 src="https://files.seeedstudio.com/wiki/LoRa-E5_Development_Kit/hardware%20overview/4081615359627_.pic_hd.jpg"/></div>

Please check out this article [LoRapedia](https://www.seeedstudio.com/blog/2020/08/03/lorapedia-an-introduction-of-lora-and-lorawan-technology/), which introduces LoRa and LoRaWAN to learn about LoRaWAN and LoRa technology in detail.

LoRa-E5 series has a built-in AT command firmware, which supports LoRaWAN Class A/B/C protocol and a wide frequency plan: EU868/US915/AU915/AS923/KR920/IN865. With this AT command firmware, users can easily and quickly build their prototype or application.

The AT command firmware contains a bootloader for DFU and the AT application. The "PB13/SPI_SCK/BOOT" pin is used to control LoRa-E5 to stay in the bootloader or jump to the AT application. When PB13 is HIGH, the module will jump to AT application after reset, with a default baud rate of 9600. When PB13 is LOW (press the "Boot" button on LoRa-E5 Dev Board or LoRa-E5 mini), the module will stay in the bootloader, and keep transmitting "C" character every 1S at baud rate 115200.

- The specification of the Grove LoRa E5 AT command can be found [here](https://files.seeedstudio.com/products/317990687/res/LoRa-E5%20AT%20Command%20Specification_V1.0%20.pdf).

- More information on the use of Grove LoRa E5 can be found in this [wiki](https://wiki.seeedstudio.com/Grove_LoRa_E5_New_Version/).

### Course Code Library

All libraries and code for this course can be found at the following address Before the course starts, you can clone the repo to your own computer for reference and learning.

<p style=":center"><a href="https://github.com/limengdu/Seeed-Studio-LoRaWAN-Dev-Kit" target="_blank"><div align=center><img width = 300 src="https://files.seeedstudio.com/wiki/seeed_logo/github.png" /></div></a></p>

### Helium LoRaWAN

For more details, please read [Helium LoRaWAN: Public Infrastructure with Private Network Servers](https://blog.helium.com/helium-lorawan-public-infrastructure-with-ultimate-privacy-9132560e4ae6).

## Clarifications on architecture decisions

We are assuming the need for real-time streaming data is high (Default Sensor frequency is to send the data payload every **15 second**. In general, we do not have such a high frequency of sending, but you can customize the number of times you send data to save the resources needed to send data in the process of later use.), because we are using LoRaWAN, so we can safely push this data to Helium and then visualize from the real-time streaming data. We could alternately push the data from sensor to Web APP visual if needed.

So for the next step, please move to the **Preliminary Preparation**. We assume that you are using a brand new computer and have not yet configured the Wio Terminal environment, then you can follow the steps above.

<table>
	<tr>
	    <td align="center"><p style="text-align:center;"><img src="https://files.seeedstudio.com/wiki/Wio-Terminal-Developer-for-helium/142.png" alt="pir" width="300" height="auto"></p></td>
	    <td align="left"><strong>Preliminary Preparation</strong></br></br>This section will cover the preparation before the tutorial starts. This includes preparing the environment for use with Arduino and Wio Terminal.</br></br><a href="https://wiki.seeedstudio.com/K1111-Preliminary-Preparation/">Jump to chapter ></a></td>
	</tr>
</table>

After completing the preparation, if you want to get started quickly, you can refer to the **Quick Start Guide** section to experience your first data upload.

<table>
    <tr>
	    <td align="center"><p style="text-align:center;"><img src="https://files.seeedstudio.com/wiki/K1111/1.png" alt="pir" width="600" height="auto"></p></td>
	    <td align="left"><strong>Quick Start Guide</strong></br></br>This chapter will take you through the Helium platform to get the most intuitive visualization of your data in the shortest amount of time.</br></br><a href="https://wiki.seeedstudio.com/K1111-Quick-Start-Guide/">Jump to chapter ></a></td>
	</tr>
</table>

Then, depending on which sensor you want to use, you can complete the operation of reading the data from the sensor, processing it and sending it to Helium. Please make your selection in the [Overview](#jump1) section.