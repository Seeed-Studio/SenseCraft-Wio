---
name: 
category: 
bzurl: 
prodimagename:
surveyurl: 
sku: 
tags:
---

<div align=center><img width = 400 src="https://files.seeedstudio.com/wiki/Wio-Terminal-Developer-for-helium/camera.jpg"/></div>

<p style=":center"><a href="https://www.seeedstudio.com/Seeed-Studio-LoRaWAN-Dev-Kit-p-5370.html" target="_blank"><img src="https://files.seeedstudio.com/wiki/Seeed-WiKi/docs/images/get_one_now.png" /></a></p>

## Introduction

Grove Vision AI Module Sensor represents a thumb-sized AI camera, customized sensor which has been already installed ML algorithm for people detection, and other customized models. Being easily deployed and displayed within minutes, it works under ultra-low power model, and provides two ways of singal transmission and multiple onboard modules, all of which make it perfect for getting started with AI-powered camera.

In this wiki, we will show you how to utilize the Grove Vision AI module Sensor connecting seeed studio XIAO Series and Arduino, to detect people, panda, and then display it on the website.

### Specification

<style type="text/css">
.tg  {border-collapse:collapse;border-spacing:0;}
.tg td{border-color:black;border-style:solid;border-width:1px;font-family:Arial, sans-serif;font-size:14px;
  overflow:hidden;padding:10px 5px;word-break:normal;}
.tg th{border-color:black;border-style:solid;border-width:1px;font-family:Arial, sans-serif;font-size:14px;
  font-weight:normal;overflow:hidden;padding:10px 5px;word-break:normal;}
.tg .tg-c3ow{border-color:inherit;text-align:center;vertical-align:top}
.tg .tg-0pky{border-color:inherit;text-align:left;vertical-align:top}
</style>
<table class="tg">
<thead>
  <tr>
    <th class="tg-c3ow">Parameters</th>
    <th class="tg-c3ow">Description</th>
    <th class="tg-c3ow">Note</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td class="tg-c3ow" rowspan="3">Connector<br></td>
    <td class="tg-0pky">Grove (Grove base for Arduino)</td>
    <td class="tg-0pky">5V Charge and Data Transmission</td>
  </tr>
  <tr>
    <td class="tg-0pky"><span style="font-weight:normal;font-style:normal;text-decoration:none">Double row 7pin socket (seeed duino XIAO)</span></td>
    <td class="tg-0pky">5V Charge and Data Transmission</td>
  </tr>
  <tr>
    <td class="tg-0pky"><span style="font-weight:400;font-style:normal">USB Type-C</span></td>
    <td class="tg-0pky">5V Charge and Firmware burn</td>
  </tr>
  <tr>
    <td class="tg-0pky">Communication Mode</td>
    <td class="tg-0pky">IIC</td>
    <td class="tg-0pky"></td>
  </tr>
  <tr>
    <td class="tg-0pky">Processor</td>
    <td class="tg-0pky">Himax HX6537-A </td>
    <td class="tg-0pky"><span style="font-weight:400;font-style:normal">400Mhz DSP (ultra low power consumption)</span></td>
  </tr>
  <tr>
    <td class="tg-0pky">Camera Sensor</td>
    <td class="tg-0pky">OV2640 chip</td>
    <td class="tg-0pky">Resolution Ratio 1600*1200</td>
  </tr>
  <tr>
    <td class="tg-0pky">Microphone</td>
    <td class="tg-0pky">MSM261D3526H1CPM chip</td>
    <td class="tg-0pky">-26dBFs sensitivity</td>
  </tr>
  <tr>
    <td class="tg-0pky">Accelerometer Sensor</td>
    <td class="tg-0pky">STLSM6DS3TR-C Sensor</td>
    <td class="tg-0pky">3D accelerometer and 3D gyroscope</td>
  </tr>
</tbody>
</table>

### Feature

- Easy-to-use AI Camera: Apply Edge Machine Learning algorithm in the camera sensor perfectly for detecting obejcts
- Plug and Play: Make toilless effort to perform full function and display within minutes
- Ultra-low Power consumption: Activate while detected objects moving for power saving
- Compact AI-powered Camera Sensor: The device itself is thumb-sized, vision-based, and suitable for Edge Intelligence
- Customized Sensor Design: Support custom ML models by users define
- Two Signal Transmission Supported: Refer to seeed studio XIAO elegant connecting and Grove base for Arduino simple connection
- Onboard Camera Sensor: Corporate OV2640 chip with 1600*1200 resolution ratio
- Onboard Microphone: Corporate MSM261D3526H1CPM chip with -26dBFs sensitivity
- Onboard Accelerometer Sensor: Corporate STLSM6DS3TR-C sensor for 3D accelerometer and 3D gyroscope detected
- Onboard 32 MB SPI Ultra Low Power Flash
- One wire for all data output: Represent Grove family simplified connection for only one Grove cable required between the sensor and the single board

## Hardware Overview

We assume you might want to be aware of some basic parameters of the product. The following tables present information about the characteristics and pinout of Grove Vision AI Module Sensor.

| Characteristic                         | Value     | Unit      |
| :-------:                              | :-------: | :-------: |
| Operating Voltage                      |   5    |     V       |
| Rate   |    115200     |          |
| I2C Interface                          | seeed studio XIAO & Arduino   | -         |
| Power Supply | dual 7-pin connector & Tpye-C    | -         |
| Downloading & Firmware Burn Interface | Type-C    | -         |
| Dimensions                             |           | mm        |

### Pinout Overview

<div align=center><img width = 600 src="https://files.seeedstudio.com/wiki/Grove_AI_Module/pinout1.png"/></div>

- 2 —— BL702
- 3 —— MIC MSM261D3526H1CPM
- 4 —— 6-axis LSM6DS3TR-C
- 5 —— SPI Flash
- 9 —— USB type C
- 10 —— Dual 7-pin female socket
- 11 —— **Reset button**
- 12 —— **BOOT button**
- 13 —— **USER button**

<div align=center><img width = 600 src="https://files.seeedstudio.com/wiki/Grove_AI_Module/pinout2.png"/></div>

- 1 —— HX6537-A
- 6 —— Camera Connector
- 7 —— DC-DC Chip
- 8 —— Grove Connector
- 14 —— Power Light
- 15 —— Burning indicator light

## Getting Started

We will show you the basic function about the module, and then introduce you the customized way that you can build the ML model of your own. But before we fully apply the module to our projects, it will take us serval steps to get the module ready.

### Preparation

**Hardware**

- Grove Vision AI Module Sensor
- **Windows host** PC (Win10 tested)
- Type-C cable

### Customize Trainning Model

The seeed studio has provided pre-trained people detected model and will provide more models in the foreseeable future. You can directly use them to quickly get to know AI camera.

- [People Face Detected Model](https://files.seeedstudio.com/wiki/Grove_AI_Module/grove_ai_camera_v01-00-0x10000000.uf2)
- [People Face & Body](https://files.seeedstudio.com/wiki/Grove_AI_Module/People_Body_Face_Detect.zip)

If you want to customize your own firmware, you can refer to <a href="https://github.com/Seeed-Studio/yolov5-swift" target="_blank"><span>here</span></a> which is powered by YOLOV5.

### Burn Training Model into the Module

The training models are all represented as ".uf2" file, which is what we need.

- **Step 1**. Connect module to the host PC with a Type-C cable and **double click** BOOT button on the module.

<div align=center><img width = 500 src="https://files.seeedstudio.com/wiki/Grove_AI_Module/GroveAI05.png"/></div>

There will be a "GROVEAI" disk pop up.

<div align=center><img width = 150 src="https://files.seeedstudio.com/wiki/Grove_AI_Module/GroveAI08.png"/></div>

- **Step 2**. Drag the firmware file (customized or downloaded ".uf2" file) to the disk.

<div align=center><img width = 450 src="https://files.seeedstudio.com/wiki/Grove_AI_Module/GroveAI09a.png"/></div>

We can see that Work LED on the module flash in speed, that means the process is ongoing. After the disk disapper, the process of burnning firmware is finished.

<div align=center><img width = 350 src="https://files.seeedstudio.com/wiki/Grove_AI_Module/GroveAI10.png"/></div>

## Arduino Library Overview

!!!Tip
    If this is your first time using Arduino, we highly recommend you to refer to [Getting Started with Arduino](https://wiki.seeedstudio.com/Getting_Started_with_Arduino/).

The code we use here provides serval methods of classification and selectable models, including customized models. The default we select here is object detection method and pre-trained model.

*seeed studio will develop more methods and models for the foreseeable future, stay tuned with us.*

<p style=":center"><a href="https://github.com/limengdu/Seeed-Grove-Vision-AI-Moudle" target="_blank"><div align=center><img width = 300 src="https://files.seeedstudio.com/wiki/seeed_logo/DOWNLOAD.png" /></div></a></p>

### Restore factory firmware

- **Step 1**. Download the [firmware](https://files.seeedstudio.com/wiki/Grove_AI_Module/grove_ai_camera_v01-00-0x10000000.uf2) on the windows PC. This firmware we are going to burn into the module is a pre-trained ML model.

- **Step 2**. Download and open "BLDevCube.exe" software, select "BL702/704/706”, and then click "Finish”

<div align=center><img width = 250 src="https://files.seeedstudio.com/wiki/Grove_AI_Module/GroveAI01a.png"/></div>

- **Step 3**. Click “View”, choose “MCU" first. Move to “Image file”, click “Browse” and select the firmware you just downloaded.

<div align=center><img width = 500 src="https://files.seeedstudio.com/wiki/Grove_AI_Module/GroveAI02a.png"/></div>

- **Step 4**. Install [Python3](https://files.seeedstudio.com/wiki/Grove_AI_Module/python-3.8.5.exe) on the PC, and then install Playsound & Pyserial. 

<div align=center><img width = 350 src="https://files.seeedstudio.com/wiki/Grove_AI_Module/GroveAI03.png"/></div>

After install Python3, you can open the **Windows CMD** and then excute:

<div align=center><img width = 100 src="https://files.seeedstudio.com/wiki/Grove_AI_Module/GroveAI04.png"/></div>

```
pip3 install playsound
pip3 install pyserial
```

- **Step 5**. Make sure there are no other devices connect to the PC. Then hold the Boot button on the module, connect it to the PC.

<div align=center><img width = 350 src="https://files.seeedstudio.com/wiki/Grove_AI_Module/GroveAI05.png"/></div>

We can see 5V light and 3.3V LED light are lighting on the back of the module, then loose the Boot button.

<div align=center><img width = 350 src="https://files.seeedstudio.com/wiki/Grove_AI_Module/GroveAI06.png"/></div>

- **Step 6**. Back to the BLDevCube software on the PC, click "Refresh" and choose a proper port. Then click “Open UART” and ”Creat&Program”, wait for the process done.

<div align=center><img width = 500 src="https://files.seeedstudio.com/wiki/Grove_AI_Module/GroveAI07.png"/></div>

- **Step 7**. Pull out the module and then reconnect it to PC. Dobule click **Boot button** on the module and soon there will be a "GROVEAI" disk pop up.

<div align=center><img width = 100 src="https://files.seeedstudio.com/wiki/Grove_AI_Module/GroveAI08.png"/></div>

Drag the firmware file (downloaded or customized in step 1) to the disk.

<div align=center><img width = 350 src="https://files.seeedstudio.com/wiki/Grove_AI_Module/GroveAI09a.png"/></div>

We can see that Work LED on the module flash in speed, that means the process is ongoing. After the disk disapper, the process of burnning firmware is finished.

<div align=center><img width = 350 src="https://files.seeedstudio.com/wiki/Grove_AI_Module/GroveAI10.png"/></div>

### Function

Before we get started developing a sketch, let's look at the available functions of the library.

1. `if (ai.begin(ALGO_OBJECT_DETECTION, MODEL_EXT_INDEX_1))`: —— This is where we can select our methods of classification and the models.

You can change `ALGO_OBJECT_DETECTION` to `ALGO_OBJECT_COUNT` or `ALGO_IMAGE_CLASSIFICATION`. 

Meanwhile, if you also burn your customized model(firmware) into the module, you can change `MODEL_EXT_INDEX_1` to `MODEL_EXT_INDEX_2` or 3, 4, as you named it.

2. `object_detection_t data`: This is the struct dataset where the result output. 

If you change the classification method, you need to change `object_detection_t` correspondingly to `image_classification_t` or `object_count_t`.

3. `uint8_t len = ai.get_result_len()` The "len" here means how many human face it detects.

4. `object_detection_t data` The data here represent a **struct** format and the tpye is "object_detection_t" that is pre-defined.

Meanwhile, if you choose other methods of classification, you need to change it to `image_classification_t data` or `object_count_t data`.

5. `Serial.print(data.confidence)` The **struct** format can not be directly used to print, it should point to the specific type which is defined in the head file.

```C++
typedef struct
{
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
    uint8_t confidence;
    uint8_t target;
} object_detection_t;
```

Hint: The "confidence" we want to print here shows how much "confidence" the camera has to detect the object.

### Library Installation

Since we have downloaded the zip Library, open your Arduino IDE, click on **Sketch > Include Library > Add .ZIP Library**. Choose the zip file you just downloaded，and if the library install correct, you will see **Library added to your libraries** in the notice window. Which means the library is installed successfully.

<div align=center><img width = 600 src="https://files.seeedstudio.com/wiki/Get_Started_With_Arduino/img/Add_Zip.png"/></div>

### Library Upgrading

In the foreseeable future, we will optimize and upgrade the product library for more interesting function. According to the library installation methods provided above, we here introduce you the way to upgrade.

We will update the link when the library is optimized. You can **delete the original library folder** in your computer's folder, then download the latest version, unzip it and put it in the Arduino IDE library directory. (**...\Arduino\libraries. ....** is the path you setup Arduino)

<div align=center><img width = 600 src="https://files.seeedstudio.com/wiki/Get_Started_With_Arduino/img/Location_lib.png"/></div>

## seeed studio XIAO / Seeeduino(Arduino UNO) Example

Now that we have our library installed and the firmware burned. we can now run some examples with Grove AI module sensor on seeed studio XIAO BLE Sense and seeeduino V4.2 to see how it behaves.

**Step 1 (seeed studio XIAO).** Parpare a Type-C cable and connect it to one seeed studio XIAO Series board. Plug it pin by pin into the Grove AI Module and use another Type-C cable to connect the module.

Both Type-C cable should be connected with the PC. In the end, the direction of the Type-C connector on the module should be the same as Type-C connector on the seeed studio XIAO samd21. For instance:

<div align=center><img width = 350 src="https://files.seeedstudio.com/wiki/Grove_AI_Module/GroveAI11.JPG"/></div>

**Step 1 (Seeeduino V4.2).** Parpare a Grove cable and connect it to one Arduino board. Use another Type-C cable to connect the module.

<div align=center><img width = 250 src="https://files.seeedstudio.com/wiki/Grove_AI_Module/GroveAI13.png"/></div>

**Step 2.** You need to Install an Arduino Software.

<p style=":center"><a href="https://www.arduino.cc/en/Main/Software" target="_blank"><div align=center><img width = 600 src="https://files.seeedstudio.com/wiki/Seeeduino_Stalker_V3_1/images/Download_IDE.png" /></div></a></p>

**Step 3.** Launch the Arduino application.

<div align=center><img width = 600 src="https://files.seeedstudio.com/wiki/seeed_logo/arduino.jpg"/></div>

**Step 3.** The code is Select your development board model and add it to the Arduino IDE.

- If you want to use **seeed studio XIAO SAMD21** for the later routines, please refer to [this tutorial](https://wiki.seeedstudio.com/Seeeduino-XIAO/#software) to finish adding.

- If you want to use **seeed studio XIAO RP2040** for the later routines, please refer to [this tutorial](https://wiki.seeedstudio.com/XIAO-RP2040-with-Arduino/#software-setup) to finish adding.

- If you want to use **seeed studio XIAO BLE** for the later routines, please refer to [this tutorial](https://wiki.seeedstudio.com/XIAO_BLE/#software-setup) to finish adding.

- If you want to use **Seeeduino** for the later routines, please refer to [this tutorial](https://wiki.seeedstudio.com/Seeed_Arduino_Boards/) to finish adding.

- If you want to use **Arduino Uno** for the later routines, please refer to [this tutorial](https://wiki.seeedstudio.com/Upload_Code/) to finish adding.

### Demo 1 Human Detection with seeed studio XIAO BLE Sense / Seeeduino V4.2

In this demo, we will detect human face and count how many people the module detects on both <a href="https://www.seeedstudio.com/Seeed-XIAO-BLE-Sense-nRF52840-p-5253.html" target="_blank"><span>seeed studio XIAO BLE Sense</span></a> and Seeeduino V4.2 (Arduino UNO). Meanwhile, seeed studio provives a [website](https://files.seeedstudio.com/grove_ai_vision/index.html) to display what the module sees.

#### **Additional Materials Required**

For seeed studio XIAO BLE Sense

|    seeed studio XIAO BLE Sense          | Grove AI Camera  |
|--------------|--------------|
|<div align=center><img width = 210 src="https://files.seeedstudio.com/wiki/XIAO-BLE/102010469_Front-14.jpg"/></div>| <div align=center><img width = 210 src="https://files.seeedstudio.com/wiki/Wio-Terminal-Developer-for-helium/camera.jpg"/></div> |
|[**Get ONE Now**](https://www.seeedstudio.com/Seeed-XIAO-BLE-Sense-nRF52840-p-5253.html)|[**Get ONE Now**](https://www.seeedstudio.com/Seeed-Studio-LoRaWAN-Dev-Kit-p-5370.html)

For Seeeduino V4.2

| Seeeduino V4.2 | Base Shield | Grove AI Camera|
|--------------|-------------|-----------------|
|![enter image description here](https://files.seeedstudio.com/wiki/wiki_english/docs/images/seeeduino_v4.2.jpg)|![enter image description here](https://files.seeedstudio.com/wiki/wiki_english/docs/images/base_shield.jpg)|<div align=center><img width = 210 src="https://files.seeedstudio.com/wiki/Wio-Terminal-Developer-for-helium/camera.jpg"/></div>
|[Get ONE Now](https://www.seeedstudio.com/Seeeduino-V4.2-p-2517.html)|[Get ONE Now](https://www.seeedstudio.com/Base-Shield-V2-p-1378.html)|[Get ONE Now](https://www.seeedstudio.com/Seeed-Studio-LoRaWAN-Dev-Kit-p-5370.html)|

#### Code Uploaded

```c++
#include "Seeed_Arduino_GroveAI.h"
#include <Wire.h>

GroveAI ai(Wire);
uint8_t state = 0;
void setup()
{
  Wire.begin();
  Serial.begin(115200);
  
   Serial.println("begin");
  if (ai.begin(ALGO_OBJECT_DETECTION, MODEL_EXT_INDEX_1)) // Object detection and pre-trained model 1
  {
    Serial.print("Version: ");
    Serial.println(ai.version());
    Serial.print("ID: ");
    Serial.println( ai.id());
    Serial.print("Algo: ");
    Serial.println( ai.algo());
    Serial.print("Model: ");
    Serial.println(ai.model());
    Serial.print("Confidence: ");
    Serial.println(ai.confidence());
    state = 1;
  }
  else
  {
    Serial.println("Algo begin failed.");
  }
}

void loop()
{
  if (state == 1)
  {
    uint32_t tick = millis();
    if (ai.invoke()) // begin invoke
    {
      while (1) // wait for invoking finished
      {
        CMD_STATE_T ret = ai.state(); 
        if (ret == CMD_STATE_IDLE)
        {
          break;
        }
        delay(20);
      }

     uint8_t len = ai.get_result_len(); // receive how many people detect
     if(len)
     {
       int time1 = millis() - tick; 
       Serial.print("Time consuming: ");
       Serial.println(time1);
       Serial.print("Number of people: ");
       Serial.println(len);
       object_detection_t data;       //get data

       for (int i = 0; i < len; i++)
       {
          Serial.println("result:detected");
          Serial.print("Detecting and calculating: ");
          Serial.println(i+1);
          ai.get_result(i, (uint8_t*)&data, sizeof(object_detection_t)); //get result
  
          Serial.print("confidence:");
          Serial.print(data.confidence);
          Serial.println();
        }
     }
     else
     {
       Serial.println("No identification");
     }
    }
    else
    {
      delay(1000);
      Serial.println("Invoke Failed.");
    }
  }
  else
  {
    state == 0;
  }
}
```

Open the serial monitor and set baud rate as 115200 and the result of people detection should be showed continuously. Meanwhile, the image that is captured by the module also will display on the website. The successful ongong output should be like:

<div align=center><img width = 350 src="https://files.seeedstudio.com/wiki/Grove_AI_Module/GroveAI12.png"/></div>

On the display website, you can see that two faces are framed with dual number. Number 0 means the human face that module detected, and the other number means the confidence.

## Trouble Shooting

1. What is the main method of connection?

- a. Link the microcontroller and the module first.
- b. Connect the microcontroller with host PC second.
- c. Connect the module with host PC at last.

2. Why can't I see the image display on the website? Nor the detecting?

- a. Is the module heated? Connect the module only will cost heat problem and let the module not functioning for a moment.
- b. Is connecting after the correct method?

## Tech Support

Please do not hesitate to submit the issue into our [forum](https://forum.seeedstudio.com/).

<br /><p style="text-align:center"><a href="https://www.seeedstudio.com/act-4.html?utm_source=wiki&utm_medium=wikibanner&utm_campaign=newproducts" target="_blank"><img src="https://files.seeedstudio.com/wiki/Wiki_Banner/new_product.jpg" /></a></p>
