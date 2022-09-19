# Soil moisture monitoring alarm system

In this wiki, we will show you how to unitize the Wio Terminal to link Grove - Wio-E5 and Grove - Soil Moisture Sensor which are in the kit, connecting them with Helium and Azure IoT Central platforms, to build a real-time backyard potting monitor and watering-reminder, for a glimpse of Internet of Things.

## Prerequisites

Before everything starts, we should meet the following prerequisites:

### Materials Required

<table align="center">
	<tr>
	    <td align="center"><div align=center><img width = 210 src="https://files.seeedstudio.com/wiki/Wio-Terminal/img/WT-GROVE.jpeg"/></div></td>
	    <td align="center"><div align=center><img width = 210 src="https://files.seeedstudio.com/products/113020091/%E5%AE%98%E7%BD%91/11302009_Preview-34.png"/></div></td>
        <td align="center"><div align=center><img width = 310 src="https://files.seeedstudio.com/wiki/Grove_Moisture_Sensor/images/Moisture_sensor_.jpg"/></div></td>
	</tr>
	<tr>
	    <td align="center">Wio Terminal</td>
	    <td align="center">Grove - Wio-E5</td>
        <td align="center">Grove Soil Moisture Sensor</td>
	</tr>
</table>

**Software**

- <a href="https://www.arduino.cc/" target="_blank"><span>Arduino</span></a>

We will upload programming to Wio Terminal through Arduino. If this is your first time using Arduino with Wio Terminal, we highly recommend you to refer to <a href="https://wiki.seeedstudio.com/Preliminary-Preparation/" target="_blank"><span>Getting Started with Wio Terminal</span></a>.

<div align=center><img width = 600 src="https://files.seeedstudio.com/wiki/Wio-Terminal/img/selectBoard.png"/></div>

**Others**

- Potting

In this kickstart demo, we are using a soil moisture sensor. Therefore a potting with deep enough soil will be the best.

<div align=center><img width = 300 src="https://files.seeedstudio.com/wiki/LoraWANdevKit/lorawandevkit32.png"/></div>

### Helium Registration for Connect

<a href="https://console.helium.com/" target="_blank"><span>Helium</span></a> is a global, distributed network of Hotspots that create public, long-range wireless coverage for LoRaWAN® enabled IoT devices. 

It allows us to send a certain amount of data through LoRa® for free and we will connect it to the Wio Terminal. All the information we need to connect the Wio Terminal are all presented **in the first page** after account registration:

<div align=center><img width = 600 src="https://files.seeedstudio.com/wiki/Wio-Terminal-Developer-for-helium/125.png"/></div>

!!!Note
    The device usually takes nearly 20 minutes to be really. It is recommanded to be added at the very beginning.

### Azure IoT Registration for Notice

<a href="https://portal.azure.com/" target="_blank"><span>Azure IoT Central</span></a> is a managed service hosted in the cloud that acts as a central message hub for communication between an IoT application and its attached devices. 

It can help us to manage LoRa® data and do some actions for one week free at the beginning . We will integrate it with Helium to **monitor data** and set up a auto **potting watering-reminder**.

<div align=center><img width = 700 src="https://files.seeedstudio.com/wiki/LoraWANdevKit/lorawandevkit30.png"/></div>

## Soil Moisture Monitor Getting Started

In this quick demo we only require a simple LoRa® connection witn Grove - Soil Moisture Sensor, for the puporse of auto soil moisture monitor and alert.

### Equipment Initiation

- **Step 1**. Use Grove cable to link everything together where the Grove - Soil Moisture Sensor on the **left** of the Wio Terminal and Grove - Wio-E5 is on the **right**.

!!!Note
    The position where the Grove - Wio-E5 is will make a difference to the uploaded code.

- **Step 2**. Connect a PC and the Wio Terminal with a Type-C cable.

<div align=center><img width = 400 src="https://files.seeedstudio.com/wiki/LoraWANdevKit/lorawandevkit33.jpg"/></div>

### Connect Wio Terminal with Helium

- **Step 1**. Open the <a href="https://console.helium.com/" target="_blank"><span>Helium</span></a> page on the PC and clilck "Devices" on the left column. **Add a new device**, then check the information below(example):

<div align=center><img width = 500 src="https://files.seeedstudio.com/wiki/LoraWANdevKit/lorawandevkit3a.png"/></div>

- **Step 2**. Download the Arduino code below and open it with Arduino.

<p style=":center"><a href="https://github.com/limengdu/Seeed-Studio-LoRaWAN-Dev-Kit/tree/main/sensor/Soil-moisture-send-data" target="_blank"><div align=center><img width = 300 src="https://files.seeedstudio.com/wiki/seeed_logo/DOWNLOAD.png" /></div></a></p>

- **Step 3**. Find the specific code line and fill the Helium connecting information we find into the code line where we should know **we only change the letters and number**.

<div align=center><img width = 700 src="https://files.seeedstudio.com/wiki/LoraWANdevKit/lorawandevkit4a.png"/></div>

- **Step 4**. Select correct Borad and the Port. We can proceed to upload it by clicking the button in the upper left corner.

<div align=center><img width = 700 src="https://files.seeedstudio.com/wiki/LoraWANdevKit/lorawandevkit3.png"/></div>

- **Step 5**. Open the "Serial Monitor" on the upper right of Arduino, and we can see that the connection is done and the data has been sent.

<div align=center><img width = 700 src="https://files.seeedstudio.com/wiki/LoraWANdevKit/lorawandevkit5a.png"/></div>

- **Step 6**. Move back to the Helium website and check the "Devices". At the bottom we can see that indeed some data have been received.

<div align=center><img width = 400 src="https://files.seeedstudio.com/wiki/Wio-Terminal-Developer-for-helium/7.jpg"/></div>

Because of the characteristic of LoRa® communication, the presentation of data is an array of numbers and letters that we may feel confused. Hence, we need to decode it for the true information.

<div align=center><img width = 700 src="https://files.seeedstudio.com/wiki/Wio-Terminal-Developer-for-helium/172.png"/></div>

### Received Data Decoded in Helium

- **Step 1**. Click the "Function" on the left colmon of the page, and add a custom function.

<div align=center><img width = 600 src="https://files.seeedstudio.com/wiki/Wio-Terminal-Developer-for-helium/184.png"/></div>

- **Step 2**. Copy the code below and add it to the "CUSTOM SCRIPT", then save it.

```cpp
function Decoder(bytes, port) {
  var decoded = {};
  if (port == 8) {
    decoded.soil = bytes[0]<<8 | bytes[1];
  }
  return decoded;
}
```

<div align=center><img width = 700 src="https://files.seeedstudio.com/wiki/LoraWANdevKit/lorawandevkit8.png"/></div>

- **Step 3**. Click "Flows" on the left and drag the "Devices" and "Functions" block into the palette. Link them together as below:

<div align=center><img width = 700 src="https://files.seeedstudio.com/wiki/LoraWANdevKit/lorawandevkit9.png"/></div>

This helps us to translate our data to be readable.

- **Step 4(optional)**. Move back to "Function" page, and we can <a href="https://wiki.seeedstudio.com/Connecting-to-Helium/#upload-code-send-data-to-helium" target="_blank"><span>get the input hexadecimal data</span></a> from the device. Select port as '8' and click the "RUN" button.

<div align=center><img width = 400 src="https://files.seeedstudio.com/wiki/Wio-Terminal-Developer-for-helium/185.png"/></div>

We now can read our data, but the process is a little complicated and not easy to use. How can we do to make the data to be presented continously?

### Integrate Azure IoT Platform

We can integrate another platform **Azure IoT** to present and manage our data, even do some actions.

- **Step 1**. Open the Helium page first, click "Integrations" on the left colmon of the page, and select "Azure IoT Central".

<div align=center><img width = 700 src="https://files.seeedstudio.com/wiki/LoraWANdevKit/lorawandevkit15.png"/></div>

- **Step 2**. Creat a new application on the <a href="https://apps.azureiotcentral.com/" target="_blank"><span>Azure IoT Central</span></a>. And then integrate the Azure IoT Central in the Helium by filling the required information below:

<div align=center><img width = 700 src="https://files.seeedstudio.com/wiki/LoraWANdevKit/lorawandevkit16.png"/></div>

- **Step 3**. Click the "Flows" on the left colmon of our page, drag the block of the Azure IoT Central and link it after the customized function, the order should be like:

<div align=center><img width = 450 src="https://files.seeedstudio.com/wiki/LoraWANdevKit/lorawandevkit17.png"/></div>

- **Step 4**. Move to the "Azure IoT Central" page and select the "Devices" on the left. We can see a device named after some ramdon numbers pop up.

<div align=center><img width = 700 src="https://files.seeedstudio.com/wiki/LoraWANdevKit/lorawandevkit18.png"/></div>

- **Step 5**. Select the device and check the "Raw data" information like the figure below shows. The information contains all the data form Helium, including the decoded data:

<div align=center><img width = 700 src="https://files.seeedstudio.com/wiki/LoraWANdevKit/lorawandevkit19.png"/></div>

We have our continous data displaying, but is it possible to display them in a sketch and maybe do some actions with the data?

### Further Function in Azure IoT Central

We can manage all the data from the Helium and choose the ones that we care about to build an auto email alert based on it.

- **Step 1**. Click the "Unformated data" on the "Devices" page and creat an auto device template.

<div align=center><img width = 700 src="https://files.seeedstudio.com/wiki/LoraWANdevKit/lorawandevkit20.png"/></div>

- **Step 2**. Move to the "Device template" and we can see the template includes and make the same type data to be sperately stored in each block:

<div align=center><img width = 700 src="https://files.seeedstudio.com/wiki/LoraWANdevKit/lorawandevkit21.png"/></div>

- **Step 3**. Select the "Overview" and we can see that there has been automatically created some sketches that show the data value.

<div align=center><img width = 300 src="https://files.seeedstudio.com/wiki/LoraWANdevKit/lorawandevkit21a.png"/></div>

Change the soil moisture data we want to display and rename the sketch.

<div align=center><img width = 700 src="https://files.seeedstudio.com/wiki/LoraWANdevKit/lorawandevkit22.png"/></div>

Remember to click **"Save"** and then click **"Pubilsh"** to save the template after we finished:

<div align=center><img width = 700 src="https://files.seeedstudio.com/wiki/LoraWANdevKit/lorawandevkit23.png"/></div>

- **Step 4**. Move to the "Rules" on the left and creat a new one.

<div align=center><img width = 700 src="https://files.seeedstudio.com/wiki/LoraWANdevKit/lorawandevkit24.png"/></div>

- **Step 5**. Fill the information below to create a auto email alert:

The device template(usually the default):

<div align=center><img width = 700 src="https://files.seeedstudio.com/wiki/LoraWANdevKit/lorawandevkit25.png"/></div>

Set up the type of the data and the value we want:

<div align=center><img width = 700 src="https://files.seeedstudio.com/wiki/LoraWANdevKit/lorawandevkit11.png"/></div>

Fill the email that is registered in the Azure IoT Central and the note we want to send:

<div align=center><img width = 500 src="https://files.seeedstudio.com/wiki/LoraWANdevKit/lorawandevkit12.png"/></div>

**Result**

- Move back to the "Devices" on the left and select the device, we can see the data have been displayed in a sketch.

<div align=center><img width = 700 src="https://files.seeedstudio.com/wiki/LoraWANdevKit/lorawandevkit31.png"/></div>

- Meanwhile, if the value we set has reached the threshold, there will be continous emails we set up before will be sent to the email.

<div align=center><img width = 700 src="https://files.seeedstudio.com/wiki/LoraWANdevKit/lorawandevkit26.png"/></div>

The contents of the email are like:

<div align=center><img width = 500 src="https://files.seeedstudio.com/wiki/LoraWANdevKit/lorawandevkit27.png"/></div>

Remember to water the potting!

Thanks!

## Tech Support

Please do not hesitate to submit the issue into our [forum](https://forum.seeedstudio.com/).

<p style="text-align:center"><a href="https://www.seeedstudio.com/act-4.html?utm_source=wiki&utm_medium=wikibanner&utm_campaign=newproducts" target="_blank"><img src="https://files.seeedstudio.com/wiki/Wiki_Banner/new_product.jpg" /></a></p>

## Statement

- The LoRa® Mark is a trademark of Semtech Corporation or its subsidiaries.
- LoRaWAN® is a mark used under license from the LoRa Alliance®.

