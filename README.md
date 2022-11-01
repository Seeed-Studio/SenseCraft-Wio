
# SenseCraft - The Project of No-Code Smart Sensor Builder

## Description 

SenseCraft is an open source software platform to build smart sensors with no-code. It delivers a complete out-of-the-box solution to sense the real-world, process data and send the data to the cloud in the easiest and fastest way possible with no coding experience at all! It is now compatible with Wio Terminal, and we hope it will be open and inclusive to support hardware platforms such as M5 Stack, XIAO and MCU with screen on the basis of community contribution.

With this toolkit, we’ll break down the process of displaying sensor data into 3 actions:

- Sense: Connect all the sensors to supported hardware platform 
- Process: Data handling and Machine Learning
- Uplink (Network): Sending data to the clould via WiFi/ LoRaWAN 

![](https://files.seeedstudio.com/wiki/K1100-quick-start/34.png)

## Supported hardware platforms

- [Wio Terminal](https://www.seeedstudio.com/Wio-Terminal-p-4509.html) (pilot testing hardware platform)
- [M5 Stack](https://m5stack.com) (to-be-compatible)
- [XIAO Series](https://www.seeedstudio.com/xiao-series-page) with Screens  (to-be-compatible)

Currently we have implemented several functions and they have already been tested to work well on the pilot testing hardware platform Wio Terminal. We will continue to further expand the possibilities of the current solution and ultimately end up with a more polished solution to serve the open source community. So, please stay tuned for our next update. 

## Quick start with Wio Terminal

- Step 1: Download the latest **.uf2** file [from here](https://github.com/Seeed-Studio/SenseCraft/releases)

- Step 2: Connect Wio Terminal to PC and enter bootloader mode on the Wio Terminal

<div align=center><img width=450 src="https://files.seeedstudio.com/wiki/K1100-quick-start/35.png"/></div>

- Step 3: Drag-and-drop the .uf2 file into the USB drive which appears on PC

<div align=center><img width=180 src="https://files.seeedstudio.com/wiki/K1100-quick-start/33.png"/></div>

To get a sneak peak about how to use this at the current state, please follow [this wiki](https://wiki.seeedstudio.com/K1100-quickstart)

## Functions list

<table style="undefined;table-layout: fixed; width: 2623px">
<colgroup>
<col style="width: 29px">
<col style="width: 334px">
<col style="width: 1767px">
<col style="width: 493px">
</colgroup>
<thead>
  <tr>
    <th>No</th>
    <th>Function</th>
    <th>Description</th>
    <th>State</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td>1</td>
    <td>Basic interaction logic of the user interface</td>
    <td>Switch between the homepage of Sense, Process, and Uplink (Network) by pressing the top 3 buttons. Press LEFT and RIGHT to select different options, press UP to return, and press ENTER to confirm. DOWN key is undefined</td>
    <td rowspan="14">Implemented<br></td>
  </tr>
  <tr>
    <td>2</td>
    <td>Real-time data display</td>
    <td>On the Sense homepage, the instantaneous data from different sensors can be updated and displayed in real-time with units</td>
  </tr>
  <tr>
    <td>3</td>
    <td>TinyML Tutorial link</td>
    <td>Display a QR code to let users jump to the wiki to follow the tutorial</td>
  </tr>
  <tr>
    <td>4</td>
    <td>LoRa® Communication (SenseCAP)</td>
    <td>All data is sent to the cloud via LoRa®</td>
  </tr>
  <tr>
    <td>5</td>
    <td>Automatic recognition of sensor connections</td>
    <td>When any sensor in the kit is connected, the screen shows that the sensor is connected and you can see the real-time data with units</td>
  </tr>
  <tr>
    <td>6</td>
    <td>Grove - Vision AI Module Logs</td>
    <td>Scroll real-time training logs of Grove - Vision AI Module with timestamps on the screen</td>
  </tr>
  <tr>
    <td>7</td>
    <td>Connect to Ubidots via WiFi</td>
    <td>The user fills in the WiFi account password and ubidots binding code through a config file, and then Wio Terminal automatically identifies the information and binds the device to the specified Ubidots account</td>
  </tr>
  <tr>
    <td>8</td>
    <td>LoRa® communication information panel display</td>
    <td>When the LoRa is successfully connected to send data, there is an information panel that displays the signal strength and the number of successful packets in real-time</td>
  </tr>
  <tr>
    <td>9</td>
    <td>Connect to Azure IoT Central via WiFi</td>
    <td>Users can connect to the Azure IoT Central platform via WiFi and display data. Learn more <a href="https://wiki.seeedstudio.com/Connect-Wio-Terminal-to-Azure-IoT-Central">here</a>.</td>
  </tr>
  <tr>
    <td>10</td>
    <td>Live Sensor Line Chart</td>
    <td>Display line charts in real-time by switching different sensors left and right. For sensors with multiple data, one line is currently shown.</td>
  </tr>
  <tr>
    <td>11</td>
    <td>Save sensor data to SD card</td>
    <td>The user has a switch under each sensor, which can save the sensor data to an SD card when turned on</td>
  </tr>
  <tr>
    <td>12</td>
    <td>Display the current firmware version</td>
    <td>When Wio Terminal is turned on, the current firmware version is displayed</td>
  </tr>
  <tr>
    <td>13</td>
    <td>Automatically rotate the screen</td>
    <td>When Wio Terminal is held upside down, it will automatically rotate the screen orientation display. (only consider up and down, not left and right)</td>
  </tr>
  <tr>
    <td>14</td>
    <td>Disconnect secondary confirmation interface</td>
    <td>On the information display page after successfully connecting to LoRa or WiFi, a discontent button is provided. After clicking, a pop-up window appears to confirm whether it is disconnected or not.</td>
  </tr>
  <tr>
    <td>15</td>
    <td>Add threshold function (user can choose when the sensor value is greater or less than the set value and then choose to upload or not upload data)</td>
    <td>The user can set in a sensor when the value is greater than/smaller than the threshold and then choose to send/not send the data to the cloud to complete a local data filtering function</td>
    <td rowspan="3">To-be-developed functions that we’d love to invite the community to contribute</td>
  </tr>
  <tr>
    <td>16</td>
    <td>Grove sensor support continues to be added based on classification and popularity from the current 200 Grove Sensors</td>
    <td>In addition to the 4 supported sensors (Grove VOC and eCO2 Gas Sensor, Grove - Soil Moisture Sensor, Grove Temp&amp;Humi Sensor, Grove Vision AI Sensor), continue to add more Grove sensor support</td>
  </tr>
  <tr>
    <td>17</td>
    <td>Other functional and optimization suggestions</td>
    <td>Provide optimization and bug feedback, and then classify the problems according to the problem. We try to fix the functions we wrote ourselves. For other suggestions and time-consuming problems raised by the community, we encourage the community to offer rewards to solve them.</td>
  </tr>
</tbody>
</table>

> Note: The LoRa® Mark is a trademark of Semtech Corporation or its subsidiaries.

Before moving further, please check [this wiki](https://wiki.seeedstudio.com/K1100-quickstart) to have a glimpse of this project in it’s current state and experience it right away!

## Would you like to contribute to this project? 

You are all invited to experience this toolkit by downloading the Release v0.3. Feel free to propose any other functions that you’d love to have by raising an Issue. Hunting the bugs by raising issues is much appreciated. A big shout-out in advance to those who will contribute to the project.

## Development Steps

In order to get started with the development, you first need to setup a development environment on your PC. After you are done with the development, you need to compile the codes and upload to the pilot testing hardware platform Wio Terminal. All these will be explained below.

### Set up environment

Here we use the PlatformIO as the debut integrated development environment (We’re open to other IDEs and feel free to offer your suggestions). PlatformIO supports some of the most popular IDEs and text editors as an extension/ plugin. Moreover, we will use PlatformIO extension with Microsoft Visual Studio.

- Step 1: Download and install Microsoft Visual Studio Code by following [this link](https://code.visualstudio.com)
- Step 2. Open VS Code, click Extensions from the left navigation pane, type platformio in the search bar and click Install

![](https://files.seeedstudio.com/wiki/K1100-quick-start/31.png)

### Build 

Now we will build the project for Wio Terminal 
 
- Step 1: Clone the following GitHub repo

```sh
https://github.com/Seeed-Studio/SenseCraft
```

- Step 2: Go into the project root directly and type the following to build the project

```sh
pio run -v
```

### Upload 

Now we will upload the project into Wio Terminal
 
- Step 1: Connect Wio Terminal to PC via USB Type-C cable 
- Step 2: Go into the previous project root directly and type the following to upload the project into Wio Terminal

```sh
pio run -t upload
```

## Get in touch

Have any questions? Would like to discuss something in depth? [Join us on Discord](https://discord.seeed.cc) (#iot-into-the-wild channel)!

## Useful links

To get started with this project, here is a list of links that we think miaght be useful.  

- [SenseCAP K1100 - The Sensor Prototype Kit Overview]( https://wiki.seeedstudio.com/K1100-Getting-Started)
- [Train and Deploy Your Own AI Model onto SenseCAP K1100](https://wiki.seeedstudio.com/Train-Deploy-AI-Model-A1101-Grove-Vision-AI)
- [Wio Terminal Getting Started Wiki](https://wiki.seeedstudio.com/Wio-Terminal-Getting-Started)
- [IoT for Beginner Courses by Microsoft](https://github.com/microsoft/IoT-For-Beginners)
