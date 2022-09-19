# Quick Start With SenseCAP

## Introduction

**SenseCAP K1100 LoRaWAN® Prototype Kit** represents the seeed studio concentrating the essence of **LoRa® communication technology** and **edge intelligence products**, for the easiest deploying and mastering LoRa® connection and IoT application.

We did our best to get the suite into our **SenseCAP** platform. SenseCAP provides an easy-to-use cloud platform. Users can scan the QR code with the SenseCAP App to bind the device with its respective account to manage the devices, and check Sensor Node data on the SenseCAP Portal. SenseCAP Portal provides API for users to further develop based on the data on the Portal.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/K1100/0.png" /></div>

For a rush to get started, the kit goes up right away to view the data content of the Wio Terminal's built-in sensors (sound, brightness and tri-axis) and Vision AI, an amazing low-power embedded machine learning recognition of the human body, in the cloud. All you need to do is use the SenseCAP Mate APP to scan the QR code on the back of the bundled Grove - Wio-E5, connect the sensors to the Wio Terminal, and open the SenseCAP cloud platform to see the sensor data information. (Data update frequency is 5 minutes)

Let's start this surprise experience now!

## Prerequisites

Before everything starts, we should meet the following prerequisites:

### Materials Required

We have written a brand new firmware for the kit. It is able to display the value of the built-in sensor of Wio Terminal and as well as the result value of Vision AI Module, also you should be able to connect to SenseCAP cloud via LoRaWAN® gateway. Therefore, Let's take out the following items from the kit and getting ready for the new experience.

<table align="center">
	<tr>
	    <td align="center"><div align=center><img width = 210 src="https://files.seeedstudio.com/wiki/Wio-Terminal/img/WT-GROVE.jpeg"/></div></td>
	    <td align="center"><div align=center><img width = 210 src="https://files.seeedstudio.com/products/113020091/%E5%AE%98%E7%BD%91/11302009_Preview-34.png"/></div></td>
      <td align="center"><div align=center><img width = 170 src="https://files.seeedstudio.com/wiki/Wio-Terminal-Developer-for-helium/camera.jpg"/></div></td>
	</tr>
	<tr>
	    <td align="center">Wio Terminal</td>
	    <td align="center">Grove - Wio-E5</td>
      <td align="center">Grove Vision AI Module</td>
	</tr>
</table>

After preparing the above modules, please connect the modules to the Wio Terminal as shown in the figure below. Grove - Wio-E5 please connect to the Grove interface on the **right side** of Wio Terminal and Vision AI connect to the Grove interface on the **left side**.

<div align=center><img width = 600 src="https://files.seeedstudio.com/wiki/K1100/aivision.jpg" /></div>

In addition to the necessary hardware mentioned above, to fully experience this tutorial you will need to have stable **Helium network coverage**, which may additionally require a Helium gateway.

We would recommend the [SenseCAP M2 Data-Only](https://www.seeedstudio.com/sensecap-m2-data-only-lorawan-indoor-gateway-sx1302-eu868-p-5339.html?queryID=53b0b5aecf873b03e23871586dc7986b&objectID=5339&indexName=bazaar_retailer_products), which is a gateway that has been tested to work consistently and is a great product.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/K1100/1.png" /></div>


### Software Preparation

If you want to use your cell phone to scan and bind your device, or view some of the sensor data remotely. Then please download SenseCAP Mate APP by yourself according to the following link.

- [SenseCAP Mate APP for iOS](https://apps.apple.com/cn/app/sensecap-mate/id1619944834) on App Store
- [SenseCAP Mate APP for Android](https://install.appcenter.ms/orgs/seeed/apps/sensecap-mate/distribution_groups/public) on the Google Play Store

<div align=center><img width = 150 src="https://files.seeedstudio.com/wiki/K1100/2.jpg" /></div>

## Quick Start Steps

### Step 1. Binding Device

We offer two ways to bind your device in the SenseCAP platform.

- Option 1. Bind the device on the web side.

Please use your browser to open the [SenseCAP console](https://sensecap.seeed.cc/portal/#/login). If this is your first time using our platform, please register an account first.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/K1100/3.png" /></div>

Then log in to the SenseCAP console using the account you just registered.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/K1100/4.png" /></div>

Select the **Dashboard** screen and click **Bind Device**.

<div align=center><img width = 600 src="https://files.seeedstudio.com/wiki/K1100/5.png" /></div>

Then on the next page you will need to fill in the **EUI**, **Code** and **Device Name**, and you can even set the location for the device. (Grove - Wio-E5 does not have a location function)

Some of the required information can be found on the sticker on the back of the Grove - Wio-E5. Please fill in the device name as you need it.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/K1100/6.png" /></div>

After filling out the above content, click the **Confirm** button below to complete the device binding.

- Option 2. Bind the device on the mobile APP.

In the prep, open the SenseCAP Mate APP after downloading the corresponding version of the APP according to all the mobile systems you use. if it is your first time to use the SenseCAP platform, please register an account first.

<div align=center><img width = 300 src="https://files.seeedstudio.com/wiki/K1100/7.jpg" /></div>

!!!Attention
    Please select the **Global** version when registering.

In the page of registration information filling, the fields with * are required fields, and those without * can be filled in optionally. Click the Register button after everything is completed.

<div align=center><img width = 300 src="https://files.seeedstudio.com/wiki/K1100/8.jpg" /></div>

Then please use the account you just registered to log into the APP.

Select Device directory at the bottom of the main interface, where you can see the information of all the SenseCAP devices you have bound.

<div align=center><img width = 300 src="https://files.seeedstudio.com/wiki/K1100/9.jpg" /></div>

Click the **+** in the upper right corner and select **Add device** to bind the Grove - Wio-E5.

<div align=center><img width = 300 src="https://files.seeedstudio.com/wiki/K1100/10.jpg" /></div>

Align the box in the center of the camera with the QR code on the back of the Grove - Wio-E5 to scan the code to bind your kit.

<div align=center><img width = 300 src="https://files.seeedstudio.com/wiki/K1100/11.jpg" /></div>

After successful binding, you will see your device in the **Device** directory. If the device is not powered on, it should be **offline** at this time.

### Step 2. Select the frequency band to connect to LoRaWAN®.

Power up the Wio Terminal with each sensor connected and turn it on (Toggle the side button to the center to turn on). 

At this time, in the main interface, the selection of LoRaWAN® band will appear.

<div align=center><img width = 400 src="https://files.seeedstudio.com/wiki/K1100/12.jpg" /></div>

The three buttons on the top of Wio Terminal correspond to the selection of three different LoRaWAN® bands, from left to right: 

- **Button A** - **EU868** band  (The EU868 band is commonly used in the European region.)
- **Button B** - **US915** band  (The US915 band is commonly used in North America.)
- **Button C** - **AU915** band  (The AU915 band is commonly used in Australia Region.)

!!!Tip
    In addition to choosing a band based on your country and region, you also need to consider the band based on the gateway you are using. We recommend that you **choose the same band as your gateway**, otherwise it may lead to network access failure or increased packet loss, poor signal, etc.

### Step 3. Observe your sensor data

In this firmware release (V1.0), we are able to see the values of Wio Terminal's built-in sensors - loudness, brightness, and tri-axis - as well as data from the Vision AI module in the kit, from the device side as well as from the SenseCAP console.

<div align=center><img width = 400 src="https://files.seeedstudio.com/wiki/K1100/13.jpg" /></div>

On the device side, we can see the current band in the top left corner of the screen, and the data sending and LoRaWAN® networking status in the top right corner - red light means not yet connected to Helium LoRaWAN®, green light means successfully connected.

The center of the screen displays the data of each sensor. The left side shows the number of bodies recognized by the Vision AI module using the original human model and the average confidence level. The right side shows the numerical information of the built-in sensors of the Wio Terminal.

All information on the device side is in real time.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/K1100/14.png" /></div>

in the SenseCAP console. We can see the sensor data we received via LoRaWAN® in the data list section.

!!!Note
	In the SenseCAP console, the meaning of the data about Vision AI is as follows.

	<div align=center><img width = 500 src="https://files.seeedstudio.com/wiki/K1100/17.png" /></div>

	Vision AI can recognize and send the confidence level of ten objects at maximum, which are numbered No.01-4175, No.02-4176, ...... and No.10-4184 in order.

	The integer part of the data is the number of the identified object. For example, in this firmware, the integer digit is always 1, which means that the content of this data is for a person and the recognition is to a person. The decimal places indicate the confidence level that the object was recognized.

	For example, No.01-4175: 1.54, No.02-4176: 1.5, means that in this data reporting, two people were identified, one with 54% confidence level and the other with 50% confidence level.

This data is uploaded to the SenseCAP platform at a frequency of every **five minutes**. Surprisingly, you can use this sending frequency completely **free of charge for one year** from the time you bundle your device. Even if you use it for longer than a year, you can continue to use SenseCAP in the future at a rate of $5 a year.

If you need to learn more about using the SenseCAP platform and related content, please refer to the [SenseCAP Documentation Center](https://sensecap-docs.seeed.cc/index.html).

## What's Next?

Congratulations, you have successfully implemented all the steps to access the SenseCAP platform.

Next, if you want to dive further into sensor programming as well as LoRaWAN®, cloud platforms and more, continue with our [SenseCAP - The Sensor Prototype Kit Overview](https://wiki.seeedstudio.com/K1100-Getting-Started/).

<td align="center"><div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/K1101/41.png"/></div></td>

## Resources

- [Firmware] [K1100_LoRaWAN_Kit_V1](https://files.seeedstudio.com/wiki/K1100/K1100_LoRaWAN_Kit_V1.uf2)


## Troubleshooting

> Q1: Why does the data content from the SenseCAP platform differ from the data displayed in the Wio Terminal?

A: Because the data is read every five minutes and then sent out within three minutes. So the sensor value obtained may be the value read once in some five-minute interval with some delay. Whereas the data from Wio Terminal is in real time, which may lead to inconsistencies between the two.

> Q2: If I don't have a Helium gateway around, how do I experience getting started quickly?

A: If you don't currently have network coverage with Helium, you can also use Wio Terminal's WiFi feature to display data in the cloud, which can be found in this [WiKi](https://wiki.seeedstudio.com/K1101-Quick-Start-Guide/).

> Q3: Why do I only get the red indicator?

A: If the light is always red, please make sure that there is a strong enough signal and stable Helium network coverage within the surrounding environment. We recommend that you have a Helium gateway of your own to experience the full process. 
Moreover, this may also be related to the instability of the Helium network, which has been partially unresponsive since July 2022, and for which we can only wait for the official fix of Helium.

> Q4: Why does the green light turn red in the course of normal use?

A: This could be one of any number of reasons. The first could be a problem with the gateway, maybe the gateway is down, etc. Try restarting the gateway. The second possibility is that packet loss has occurred, Grove - Wio-E5 may not be able to send some data smoothly due to performance reasons, and the red light may be on at this time, which can generally be solved by restarting Wio Terminal.

> Q5: Why is it that sometimes some sensor data in a set of content does not appear on SenseCAP?

A: As explained in Q3, it could be that Grove - Wio-E5 is experiencing packet loss. In general, packet loss is a normal situation and does not need to be overly concerned. If there is a very severe packet loss (red light all the time), you can try to restart Wio Terminal.
If you have very high requirements for real-time data and stability, you may want to check out the industrial-grade SenseCAP series devices.

> Q6: I did not purchase the kit for K1101, can I use the firmware to experience this routine?

A: If you are not a customer who purchased this kit, you may not be able to experience full access to SenseCAP. The Grove - Wio-E5 in the kit is a special edition, and products purchased separately do not have access to SenseCAP.

> Q7: Is the code of the firmware open source?

A: The code is not open source.

> Q8: If I use another program, can I recover the firmware?

A: You can burn this firmware for the Wio Terminal at any time, and in theory you can always use this routine as long as it does not destroy the original firmware of the Grove - Wio-E5. The method to burn the firmware is as follows.
Slide the switch twice very quickly, as followed:

<div align=center><img width = 400 src="https://files.seeedstudio.com/wiki/Wio-Terminal/img/Wio-Terminal-Bootloader.png"/></div>

At this point a disk called **Arduino** will appear on your computer, just copy the firmware (.uf2 file) to this disk. Once the firmware is installed, the disk will be automatically ejected and the Wio Terminal will reboot into the Setup Band interface.

> Q9: If I change the Dev EUI, App EUI, App Key of Grove - Wio-E5, is there a way to retrieve it?

A: If you need to change the above three code information for Grove - Wio-E5 in subsequent more in-depth studies, this is irreversible. A changed Grove - Wio-E5 cannot be reconnected to the SenseCAP platform, where it will always go offline.

## Tech Support

Please do not hesitate to submit the issue into our [forum](https://forum.seeedstudio.com/).


> **Upgradable to Industrial Sensors**
SenseCAP K1100 serves as a gateway to the higher-end SenseCAP LoRaWAN® series of industrial outdoor sensors. Users can prototype multiple AIoT applications with this kit, after which they can scale them up to the robust, long-life, and industrial SenseCAP sensors.

<div align=center><img width = 800 src="https://media-cdn.seeedstudio.com/media/wysiwyg/__16582252472624.png"/></div>

## Statement

- The LoRa® Mark is a trademark of Semtech Corporation or its subsidiaries.
- LoRaWAN® is a mark used under license from the LoRa Alliance®.



