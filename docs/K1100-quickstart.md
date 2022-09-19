# Quick Start with SenseCAP K1100 - The Sensor Prototype Kit (Beta)

## Introduction

**SenseCAP K1100 - The Sensor Prototype Kit** represents Seeed Studio concentrating the essence of LoRa® communication on technology and edge intelligence products, for the easiest deploying and mastering of LoRa® and IoT applications.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/K1100/banner.png"/></div>

<p style=":center"><a href="https://www.seeedstudio.com/Seeed-Studio-LoRaWAN-Dev-Kit-p-5370.html?queryID=a88444c7c4ccfa5dddd4d2a84db3dd5e&objectID=5370&indexName=bazaar_retailer_products" target="_blank"><img src="https://files.seeedstudio.com/wiki/Seeed-WiKi/docs/images/get_one_now.png" /></a></p>

We are proud to bring you **SenseCraft**, which allows our users to experience a better way to send data to the cloud in the shortest possible time and with very little hands-on cost.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/K1100-quick-start/step.png"/></div>

This is only possible with the help of our completely in-house designed **SenseCAP** platform. SenseCAP provides an easy-to-use cloud platform. Users can scan the QR code with the **SenseCAP Mate App** to bind the device with its respective account to manage the devices, in the LoRaWAN® network coverage environment and check Sensor Node data on the SenseCAP Portal. SenseCAP Portal provides API for users to further develop based on the data on the Portal.

If you don't have LoRaWAN® coverage around you for the time being, don't worry, you can switch freely between WiFi and LoRa®, and you can also transfer data to **Ubidots**/**Microsoft Azure IoT Central** for further data processing via WiFi.

We can't wait to share our current project progress with you, and we welcome all users to experience our project!

## Overview

With IoT functionality as one of the features of the SenseCAP K1100 kit, it is necessary to take a look at the logic of its implementation before we start.

To summarise, we offer two paths for our users to experience IoT, one is LoRa® and the other is WiFi.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/K1100-quick-start/lora.png"/></div>

If you want to use LoRa®, then you will need to prepare a suitable LoRa® gateway in advance and connect the data to the SenseCAP platform by using a Grove Wio E5 module, connected to a LoRaWAN®.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/K1100-quick-start/wifi.png"/></div>

If you want to use WiFi, then you need to have stable WiFi network coverage in your environment. Simply enter your WiFi and password in the profile, along with information such as the key provided by the IoT platform, to achieve an exceptional IoT experience.

## Preliminary Preparation

Before you begin, we strongly recommend that you upgrade to the latest version of SenseCraft to ensure the most stable experience.

You can download and update the latest version in our **SenseCraft distribution** by clicking on the icon below.

<p style=":center"><a href="https://github.com/Seeed-Studio/SenseCraft/releases" target="_blank"><div align=center><img width = 300 src="https://files.seeedstudio.com/wiki/seeed_logo/github.png" /></div></a></p>

Of course, you can also download the latest .uf2 file directly by clicking here. 

<table align="center">
	<tr>
		<td align="center">SenseCraft-v0.2-20220907.uf2</td>
		<td align="center"><a href="https://files.seeedstudio.com/wiki/K1100-quick-start/SenseCraft-v0.2-20220907.uf2" target="_blank">Download</a></td>
	</tr>
</table>

After downloading SenseCraft to your local disk, please follow the instructions below to flash it into Wio Terminal.

Connect the Wio Terminal to PC and turn in ON, Enter **Bootloader Mode** by sliding down the power switch further away from "ON" position, release, slide again and release.

<div align=center><img width = 500 src="https://files.seeedstudio.com/wiki/Wio-Terminal/img/Wio-Terminal-Bootloader.png"/></div>

!!!Note
	Once Wio Terminal is in the Bootloader mode, the blue LED will start to breathe in a way that is different from blinking.

Open File Explorer on your PC and you will see a new external drive, named **Arduino**, drag the previously downloaded **.uf2** file into this **Arduino** drive.

Once the SenseCraft flash is complete, the external memory named Arduino will automatically pop up and the SenseCraft program will start working.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/K1100-quick-start/38.png"/></div>


## Navigate the UI using buttons

Before you get into learning the operating interface, you need to get used to the Button logic we have designed for Wio Terminal. In this way, you will be able to select and operate pages very smoothly according to the fixed Button logic.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/K1100-quick-start/button.png"/></div>

First are the three buttons located above the Wio Terminal. They correspond to the display screens of the three main functions. They are **Sense**, **Process** and **Uplink** respectively.

No matter where you are, when you press the three buttons at the top, you will be able to go back to these three screens.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/K1100/101.png"/></div>

Then there is the five-way directional button located at the bottom right of Wio Terminal, which allows you to perform the following operations:

- **Left/ Right:** Scroll through pages/ menus left and right
- **Middle:** Make a selection
- **Up:** Go back to the previous page

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/K1100-quick-start/40.png"/></div>

When a green box appears on the page, it indicates that the content is in the selected state.

## Page Logic

As mentioned above, we have prepared three pages for SenseCraft, representing the three main functional modules of SenseCraft, namely **Sense**, **Process** and **Uplink**.

### Sense

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/K1100-quick-start/39.png"/></div>

The main function of the **Sense** page is the sensor data display. You can observe the working status and sensor values of the built-in sensors in Wio Terminal from the Sense page.

Of course, if you try to connect the Grove sensors in the kit to the Grove connector on the **right** side of the Wio Terminal, you will find that the Wio Terminal will automatically detect the type of sensors and read their values.

### Process

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/K1100-quick-start/41.png"/></div>

The main function of the **Process** page is to show the process of data processing. We have currently developed log output for this page for the recognition and model processing of the Grove Vision AI module.

In the future, we will give Wio Terminal more powerful data filtering and processing capabilities for this page.


### Uplink

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/K1100-quick-start/42.png"/></div>

The main function of the **Uplink** page is to upload data to the cloud. Users can configure which IoT method you want to use, LoRa® or WiFi, on this page.

Here, you can freely configure your exclusive IoT features, freely switch between different networks and platforms, and create its value for this set of devices.


## Display of data from built-in sensors

After you flash **SenseCraft** to Wio Terminal and power it up, wait a few seconds and you will be able to see the values of the built-in sensors directly on the **Sense** page.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/K1100-quick-start/39.png"/></div>

Wio Terminal has three built-in sensors: a light sensor, a sound loudness sensor, and a three-axis sensor.

At this moment you do not need to do anything, the value of the built-in sensor will automatically refresh once per second, you can very naturally observe the current environment of light, loudness and three-axis data.

## Send sensor data to SenseCAP via LoRa®

**Step 1.** Press the rightmost top button to enter the **Network** tab. 

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/K1100-quick-start/42.png"/></div>

You will notice that LoRa® is already highlighted.

**Step 2.** **Middle press** to select **LoRa®** and connect **Grove-Wio E5** to the Grove connector on the left side.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/K1100-quick-start/44.png"/></div>

**Step 3.** Scroll through the LoRaWAN® frequency bands displayed on the screen and select the frequency band according to your region.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/K1100-quick-start/45.png"/></div>

Now it will prompt you to bind the Wio-E5 with the SenseCAP platform.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/K1100-quick-start/46.png"/></div>

We offer two ways to bind your device in the SenseCAP platform.

- **Option 1. Bind the device on the mobile APP**

If you want to use your cell phone to scan and bind your device, or view some of the sensor data remotely. Then please download SenseCAP Mate APP by yourself according to the following link.

- [SenseCAP Mate APP for iOS](https://apps.apple.com/cn/app/sensecap-mate/id1619944834) on App Store
- [SenseCAP Mate APP for Android](https://install.appcenter.ms/orgs/seeed/apps/sensecap-mate/distribution_groups/public) on the Google Play Store

<div align=center><img width = 100 src="https://files.seeedstudio.com/wiki/K1100/2.jpg" /></div>

Open the SenseCAP Mate APP after downloading the corresponding version of the APP according to all the mobile systems you use. if it is your first time to use the SenseCAP platform, please register an account first.

<div align=center><img width = 300 src="https://files.seeedstudio.com/wiki/K1100/7.jpg" /></div>

!!!Attention
    Please select the **Global** version when registering.

In the page of registration information filling, the fields with * are required fields, and those without * can be filled in optionally. Click the Register button after everything is completed.

<div align=center><img width = 300 src="https://files.seeedstudio.com/wiki/K1100/8.jpg" /></div>

Then please use the account you just registered to log into the APP.

Select Device directory at the bottom of the main interface, where you can see the information of all the SenseCAP devices you have bound.

<div align=center><img width = 300 src="https://files.seeedstudio.com/wiki/K1100/9.jpg" /></div>

Click the **+** in the upper right corner and select **Add device** to bind the Grove - Wio-E5.

<div align=center><img width = 400 src="https://files.seeedstudio.com/wiki/K1100-quick-start/10.jpg" /></div>

Align the box in the center of the camera with the QR code on the back of the Grove - Wio-E5 to scan the code to bind your kit.

<div align=center><img width=300 src="https://files.seeedstudio.com/wiki/K1100-quick-start/49.png"/></div>

After successful binding using the SenseCAP Mate App, you will see the sensor data displayed on the app.

<div align=center><img width = 300 src="https://files.seeedstudio.com/wiki/K1100/104.jpg" /></div>

- **Option 2. Bind the device on the SenseCAP web platform**

Please use your browser to open the [SenseCAP Console](https://sensecap.seeed.cc/portal/#/login). If this is your first time using our platform, please register an account first.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/K1100/3.png" /></div>

Then log in to the SenseCAP console using the account you just registered.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/K1100/4.png" /></div>

Select the **Dashboard** screen and click **Bind Device**.

<div align=center><img width = 600 src="https://files.seeedstudio.com/wiki/K1100/5.png" /></div>

Then on the next page you will need to fill in the **EUI**, **Code/KEY** and **Device Name**, and you can even set the location for the device. (Grove - Wio-E5 does not have a location function)

Some of the required information can be found on the sticker on the back of the Grove - Wio-E5. Please fill in the device name as you need it.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/K1100/6.png" /></div>

After filling out the above content, click the **Confirm** button below to complete the device binding.

After successful binding using the web platform you will see the sensor data displayed on the platform.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/K1100/105.png" /></div>

Once you have selected the band, you will need to wait a little while for the Grove Wio E5 to finish initialising.

<div align=center><img width=800 src="https://files.seeedstudio.com/wiki/K1100-quick-start/47.png"/></div>


If the LoRaWAN® connection to SenseCAP platform is successful, you will see the following.

<div align=center><img width=800 src="https://files.seeedstudio.com/wiki/K1100-quick-start/48.png"/></div>

If you are not connected to the Grove Wio E5 or if the LoRaWAN® is disconnected in the middle of the process, it will show a transmission failure.

<div align=center><img width=800 src="https://files.seeedstudio.com/wiki/K1100-quick-start/50.png"/></div>


## Send sensor data to Ubidots via WiFi

Here the device will use WiFi communication to send the sensor data to the cloud. Ubidots platform will be used to visualize the data coming from the sensors connected to Wio Terminal via WiFi!

**Step 1.** Register and login to Ubidots

If this is your first time using Ubidots, please go to the [Ubidots website](https://ubidots.com/) and register an account of your own.

<td align="center"><div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/k1100_ubidots/3.png"/></div></td>

Once you have registered, please login to Ubidots using your registered account.

**Step 2.** Assign your unique **Ubidots TOKEN**

Every request to Ubidots requires a TOKEN. The easiest way to get yours is clicking on **API Credentials** from the user dropdown.

Go to you user dropdown and click on API credentials:

<td align="center"><div align=center><img width = 250 src="https://files.seeedstudio.com/wiki/k1100_ubidots/4.png"/></div></td>

**Tokens**: Temporary and revocable keys to be used in your API requests. Please save the TOKEN for now, we will use it later.

<td align="center"><div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/k1100_ubidots/5.png"/></div></td>

!!!Attention
	All API calls only accept your **TOKEN**. Don’t try to use your API Key, it won’t work! You can also access all of your Ubidots API Keys section of the My Profile settings in your User Dropdown. Select **My Profile** –> **API Credentials** to review the list of API TOKENS relating to your specific Ubidots Account.

	<td align="center"><div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/k1100_ubidots/6.png"/></div></td>

**Step 3.** Be prepared with the necessary information

To establish an MQTT connection, we will need to prepare the following information in advance.

- **WiFi Name**
	- Fill in the WiFi Name to which Wio Terminal can connect. Wio Terminal will search for WiFi within its surroundings and try to connect to it.
- **Password for WiFi**
	- Fill in the password for the WiFi to which Wio Terminal can connect.
- **MQTT client name**
	- This is special because it is the ID with which your device will be identified by the broker so it **MUST** be unique. If your device tries to connect with the same ID that has already been taken by another device, the connection will be refused. Please create your own all alphanumeric 8-12+ character MQTT client name and input into the code accordingly.
	Need some help creating a unique MQTT client name, check out this [random ascii builder](https://www.random.org/strings/), or simply use the MAC address of your device as every MAC address is globally unique.

	<td align="center"><div align=center><img width = 600 src="https://files.seeedstudio.com/wiki/k1100_ubidots/7.png"/></div></td>

- **Ubidots TOKEN**
	- This is the TOKEN generated in **step 2**.
- **Device label**
	- This is the name of the device and the name of the device provided by the user will be used by Ubidots to identify the device. If the Device label don’t exist before the first dot is sent, Ubidots will create them automatically.

Please have the above ready as we will use them in the next step.

**Step 4.** Confige **config.txt**.

When we use the SenseCraft, an external drivers appears as soon as the Wio Terminal is connected to the computer.


!!!Attention
    This means that the USB stick appears when the Wio Terminal is connected to the computer, **without the need to toggle the side button twice extra**!

Normally there is a file called **config.txt** on the external drivers, so open it.

It holds the necessary information about the network connection, which we can modify to achieve automatic configuration at power-up. The contents of the **config.txt** file are in the following format.

```
SSID=WiFi_Name
PASSWORD=WiFi_Password
MQTT_CLIENT_NAME=Topic
TOKEN=Default_Token
DEVICE_LABEL=Device_Name
```

Please fill in the required fields above, corresponding to the information we obtained in **step 3**. Once completed, the contents of your config.txt file should look similar to the example below.

```
SSID=CHCK
PASSWORD=123456
MQTT_CLIENT_NAME=al49mw880j
TOKEN=BBFF-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
DEVICE_LABEL=Wio-Terminal
```

!!!Attention
	Once you have made your changes, **save** the config.txt file and **restart** Wio Terminal by flicking the side button down once.

**Step 5.** Enter **Network** tab and select **WiFi**.

<div align=center><img width=800 src="https://files.seeedstudio.com/wiki/K1100-quick-start/51.png"/></div>

Now it will wait for the configuration to load.

<div align=center><img width=800 src="https://files.seeedstudio.com/wiki/K1100-quick-start/52.png"/></div>

Once the loaded WiFi configuration is picked up, it will first connect with the WiFi network and then with the Ubidots platform. If the connection with Ubidots is successful, it will display the following.

<div align=center><img width=800 src="https://files.seeedstudio.com/wiki/K1100-quick-start/53.png"/></div>

**Step 6.** View data in Ubidots

Once the data has been successfully sent to Ubidots, you will be able to view the data information in your Ubidots device.

Ubidots has a very simple interface and if you want to view information about your device, you can select **Devices** right at the top of the dashboard.

<td align="center"><div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/k1100_ubidots/16.png"/></div></td>

<td align="center"><div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/k1100_ubidots/11.png"/></div></td>

Click on your device name here to be able to see all the history tabs.

<div align=center><img width=800 src="https://files.seeedstudio.com/wiki/K1100-quick-start/54.png"/></div>

!!!Note
    The free version of Ubidots only supports the creation of **one TOKEN** and **ten labels**. Once you have more than ten labels, the labels you have uploaded as well as the data will be discarded.

!!!Note
    If you would like to use Ubidots for further independent development, please refer to our further [tutorials on Ubidots](https://wiki.seeedstudio.com/Getting_started_with_Ubidots/).


## Connect the other Grove sensors in the kit

In addition to the built-in sensors, the possibilities of SenseCraft are endless. The Grove sensor in the kit is also allowed to access the Wio Terminal and is automatically recognised.

### Plug a Grove sensor into Wio Terminal

In the current version of SenseCraft we only support the simultaneous connection of one sensor for use. (except for the Grove Wio E5)

When connecting, you can use the Grove cable provided in the kit to connect one of the sensors you want to use to the Grove connector on the bottom right of the Wio Terminal.

<div align=center><img width=800 src="https://files.seeedstudio.com/wiki/K1100-quick-start/55.png"/></div>

The diagram above shows the Grove Vision AI as an example, indicating how the sensors in the kit are connected. Of course, the same applies to other sensors. (except for the Grove Wio E5)

!!!Attention
	Do not connect the Grove sensor in the kit to the Grove connector on the left side of the Wio Terminal. The Grove connector on the left is currently designed for the connection of the Grove Wio E5.

### View Grove sensor values

Once you have connected the Grove sensor you will be able to view the Grove sensor values in the Sense page.

You just need to press the **right** arrow button under the **Sense** page until the value of the external sensor appears. Usually, the value of the external sensor will be after the **IMU sensor**.

<div align=center><img width=800 src="https://files.seeedstudio.com/wiki/K1100-quick-start/56.png"/></div>

### Uploading data from Grove sensors to the cloud

The detection of the sensor by SenseCraft is fully automatic, so we don't need the user to do anything extra. Of course, this all includes the uploading of data after the newly inserted Grove sensor.

- Similarly, if you want to send data from your Grove sensors over LoRaWAN®, you just need to **connect your Grove Wio E5 on the left side** at the same time.

<div align=center><img width=800 src="https://files.seeedstudio.com/wiki/K1100-quick-start/57.png"/></div>

Then, follow the steps in **[Send sensor data to SenseCAP via LoRa®](https://wiki.seeedstudio.com/K1100-quickstart/#send-sensor-data-to-sensecap-via-lora)**.

- If you want to send data out via WiFi, then you do not need to connect anything else, please continue to refer to the contents of **[Send sensor data to Ubidots via WiFi](https://wiki.seeedstudio.com/K1100-quickstart/#send-sensor-data-to-ubidots-via-wifi)** for action.

<div align=center><img width=800 src="https://files.seeedstudio.com/wiki/K1100-quick-start/58.png"/></div>

## Advanced Play

We have designed a number of very interesting and advanced ways to play with SenseCraft, and you can get a quick overview and use these features with this section.

### Vision AI real-time analysis

This is a feature designed for Grove Vision AI. Users can observe the running log of Vision AI under this interface, which is convenient for users to observe the recognition of Vision AI in real time, adjust the camera screen, etc.

**Step 1.** Connect the Grove Vision AI

Please connect your Grove Vision AI to the Grove connector on the **right** side of the Wio Terminal.

<div align=center><img width=800 src="https://files.seeedstudio.com/wiki/K1100-quick-start/55.png"/></div>

**Step 2.** Access the Vision AI real-time analysis interface

Please click on the second button above the Wio Terminal to access the **Process** screen.

The default selection under the Process screen is **Vision AI real-time analysis**, at which point we simply **middle press** on the 5-way button to enter.

<div align=center><img width=800 src="https://files.seeedstudio.com/wiki/K1100-quick-start/59.png"/></div>

Then you will be able to see Vision AI log on this page.

<div align=center><img width=800 src="https://files.seeedstudio.com/wiki/K1100-quick-start/61.png"/></div>

### TinyML Example

This is a feature designed for Wio Terminal in embedded machine learning. Under this page, users can scan into the TinyML series of courses we have prepared to experience the powerful machine learning capabilities of Wio Terminal & Vision AI.

**Step 1.** Please click on the second button above the Wio Terminal to access the **Process** screen.

<div align=center><img width=800 src="https://files.seeedstudio.com/wiki/K1100-quick-start/60.png"/></div>

**Step 2.** Access the TinyML Example interface

Press the right arrow of the five-way button to select TinyML Example. Simply **middle press** on the 5-way button to enter.

<div align=center><img width=800 src="https://files.seeedstudio.com/wiki/K1100-quick-start/62.png"/></div>



Stay tuned for more features of SenseCraft!

<!-- ## Display data from built-in sensors

### Sensor Overview

As soon as you turn on the Wio Terminal, the following will be displayed.

<div align=center><img width=400 src="https://files.seeedstudio.com/wiki/K1100-quick-start/2.png"/></div>

This is first page under the **Sense** tab. In this tab, you have an overview of the data from the built-in sensors and Grove sensors connected to the Wio Terminal. The first page will display the data from built-in sensors such as **light sensor, sound sensor and IMU (3-axis accelerometer)**. Also, you will notice that the **Light** column is already highlighted in this page.

### Graph Visualization

- **Step 1.** Since the light column is already highlighted, **middle press** on the 5-way button to enter graph visualization mode for the data from the light sensor.

<div align=center><img width=400 src="https://files.seeedstudio.com/wiki/K1100-quick-start/3.png"/></div>

- **Step 2.** Now **press up** to go back to the previous page and **press right** to scroll through the other sensors. For example, if you press right once, it will select the sound sensor as follows.

<div align=center><img width=400 src="https://files.seeedstudio.com/wiki/K1100-quick-start/4.png"/></div>

- **Step 3.** After that, you can enter graph visualization mode for that sensor as well by pressing the middle button.

<div align=center><img width=400 src="https://files.seeedstudio.com/wiki/K1100-quick-start/5.png"/></div>

You can repeat the same selection process for the remaining sensors displayed on the **first page of Sense tab**.

## Display data from external Grove sensors

Initially, it is expected to support all the Grove modules which comes with the K1100 Kit.

- **Step 1.** In the first page of the **Sense** tab, press **right button** three times to enter the second page.

<div align=center><img width=400 src="https://files.seeedstudio.com/wiki/K1100-quick-start/11.png"/></div>

- **Step 2.** Plug-in a Grove sensor to the **Grove port on the right side** of the Wio Terminal and the data from the connected sensor can be displayed on the screen.

<div align=center><img width=400 src="https://files.seeedstudio.com/wiki/K1100-quick-start/29.png"/></div>

As soon as the sensor is connected, sensor data will be displayed on the screen. Data visualization can also be done just like before.

<div align=center><img width=400 src="https://files.seeedstudio.com/wiki/K1100-quick-start/12.png"/></div>

## Visualize results from Grove - Vision AI Module

Enter the **Process** tab by pressing the top middle button.

<div align=center><img width=400 src="https://files.seeedstudio.com/wiki/K1100-quick-start/28.png"/></div>

This is first page under the **Process** tab. In this tab, real-time analysis of the Vision AI module can be done.

More examples of Wio Terminal applications using TinyML can be found in the **TinyML Example** under the **Process** section.

<div align=center><img width=400 src="https://files.seeedstudio.com/wiki/K1100-quick-start/37.png"/></div>

## Connect to LoRaWAN®

So far we have only visualized the sensor data locally on the Wio Terminal and saved the data to a connected TF card as well. Now we will move one step further where we will send the sensor data to the cloud via LoRaWAN®!

Here we will use the Seeed SenseCAP platform to visualize the data coming from the sensors connected to Wio Terminal via LoRaWAN®!

- **Step 1.** Confige **config.txt**.

When we use the SenseCraft firmware, a USB stick appears as soon as the Wio Terminal is connected to the computer.

!!!Attention
    This means that the USB stick appears when the Wio Terminal is connected to the computer, **without the need to toggle the side button twice extra**!

Normally there is a file called **config.txt** on the USB stick (if you don't have it, create it yourself), so open it.

It holds the necessary information about the network connection, which we can modify to achieve automatic configuration at power-up.

The contents of the **config.txt** file are in the following format.

```
FREQUENCY=<LORA_FREQUENCY>
```

An explanation of the above is set out below.

- If you choose to connect to **LoRa®**, the following are required.
    - **FREQUENCY**: Here is an indication of the frequency band of your chosen LoRa® network. Where the number **1** indicates **EU868** and the number **2** indicates **US915**.

For example, if I want to use the EU868 band, then my content in config.txt would be:

```
FREQUENCY=1
```

!!!Tip
    Don't forget to save the contents once the configuration is complete.



## Connect to WiFi (Ubidots)

Here the device will use WiFi communication to send the sensor data to the cloud. Ubidots platform will be used to visualize the data coming from the sensors connected to Wio Terminal via WiFi!

- **Step 1.** Register and login to Ubidots

If this is your first time using Ubidots, please go to the [Ubidots website](https://ubidots.com/) and register an account of your own.

<td align="center"><div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/k1100_ubidots/3.png"/></div></td>

Once you have registered, please login to Ubidots using your registered account.

- **Step 2.** Assign your unique **Ubidots TOKEN**

Every request to Ubidots requires a TOKEN. The easiest way to get yours is clicking on “API Credentials” from the user dropdown.

Go to you user dropdown and click on API credentials:

<td align="center"><div align=center><img width = 250 src="https://files.seeedstudio.com/wiki/k1100_ubidots/4.png"/></div></td>

**Tokens**: Temporary and revocable keys to be used in your API requests. Please save the TOKEN for now, we will use it later.

<td align="center"><div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/k1100_ubidots/5.png"/></div></td>

!!!Attention
	All API calls only accept your **TOKEN**. Don’t try to use your API Key, it won’t work! You can also access all of your Ubidots API Keys section of the My Profile settings in your User Dropdown. Select **My Profile** –> **API Credentials** to review the list of API TOKENS relating to your specific Ubidots Account.

	<td align="center"><div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/k1100_ubidots/6.png"/></div></td>

- **Step 3.** Be prepared with the necessary information

To establish an MQTT connection, we will need to prepare the following information in advance.

- **WiFi SSID**
	- Fill in the WiFi SSID to which Wio Terminal can connect. Wio Terminal will search for WiFi within its surroundings and try to connect to it.
- **Password for WiFi SSID**
	- Fill in the password for the WiFi SSID to which Wio Terminal can connect.
- **Ubidots TOKEN**
	- This is the TOKEN generated in **step 2**.
- **Device label**
	- This is the name of the device and the name of the device provided by the user will be used by Ubidots to identify the device. If the Device label don’t exist before the first dot is sent, Ubidots will create them automatically.
- **MQTT client name**
	- This is special because it is the ID with which your device will be identified by the broker so it **MUST** be unique. If your device tries to connect with the same ID that has already been taken by another device, the connection will be refused. Please create your own all alphanumeric 8-12+ character MQTT client name and input into the code accordingly.
	Need some help creating a unique MQTT client name, check out this [random ascii builder](https://www.random.org/strings/), or simply use the MAC address of your device as every MAC address is globally unique.

	<td align="center"><div align=center><img width = 600 src="https://files.seeedstudio.com/wiki/k1100_ubidots/7.png"/></div></td>

Please have the above ready as we will use them in the next step.

- **Step 4.** Confige **config.txt**.

When we use the SenseCraft firmware, a USB stick appears as soon as the Wio Terminal is connected to the computer.

!!!Attention
    This means that the USB stick appears when the Wio Terminal is connected to the computer, **without the need to toggle the side button twice extra**!

Normally there is a file called **config.txt** on the USB stick (if you don't have it, create it yourself), so open it.

It holds the necessary information about the network connection, which we can modify to achieve automatic configuration at power-up.

The contents of the **config.txt** file are in the following format.

```
SSID=<YOUR_SSID>
PASSWORD=<YOUR_SSID_PASSWORD>
MQTT_CLIENT_NAME=<YOUR_MQTT_CLIENT_NAME>
TOKEN=<YOUR_UBIDOTS_TOKEN>
DEVICE_LABEL=<YOUR_DEVICE_NAME>
```

Please fill in the required fields above, corresponding to the information we obtained in **step 3**. Once completed, the contents of your config.txt file should look similar to the example below.

```
SSID=CHCK
PASSWORD=123456
MQTT_CLIENT_NAME=al49mw880j
TOKEN=BBFF-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
DEVICE_LABEL=Wio-Terminal
```

- **Step 5.** Enter **Network** tab and select **WiFi**.

<div align=center><img width=400 src="https://files.seeedstudio.com/wiki/K1100-quick-start/21.png"/></div>

Now it will wait for the configuration to load.

<div align=center><img width=400 src="https://files.seeedstudio.com/wiki/K1100-quick-start/22.png"/></div>

Once the loaded WiFi configuration is picked up, it will first connect with the WiFi network and then with the Ubidots platform. If the connection with Ubidots is successful, it will display the following.

<div align=center><img width=400 src="https://files.seeedstudio.com/wiki/K1100-quick-start/23.png"/></div>

If the signal strengh between the Wio Terminal and the WiFi router is weak, it will show a **low signal intensity** warning.

<div align=center><img width=400 src="https://files.seeedstudio.com/wiki/K1100-quick-start/24.png"/></div>

Also, if the above WiFi connection fails, it will show the following.

<div align=center><img width=400 src="https://files.seeedstudio.com/wiki/K1100-quick-start/25.png"/></div>

If you want to disconnect from the LoRaWAN® connection, you can click up arrow key for return.

!!!Note
    If you would like to use Ubidots for further independent development, please refer to our further [tutorials on Ubidots](https://wiki.seeedstudio.com/Getting_started_with_Ubidots/).


## Functions to implement

Eventhough the above interface has a lot more pages, the functions are not implemented yet. However, they will be implemented in the near future.

The following is a list of functions that will be added to the interface:

- Save all the sensor data to a connected SD card **(Under Development)**
- UI and operation optimisation **(Under Development)**


### Implementation expections

The following is an overview of how the above-mentioned functions are expected to look like .

#### Save to TF Card (Under Development)

- **Step 1.** When in graph visualization mode as described before, **middle press** again to view this page.

<div align=center><img width=400 src="https://files.seeedstudio.com/wiki/K1100-quick-start/6.png"/></div>

- **Step 2.** This is where the data from the sensor can be saved to an SD card. First insert an SD card to the Wio Terminal.

- **Step 3.** After that, press the **middle button** to select **Save to TF card** and it will show the message **Saving has been started**. Once this message dissappears, the saving is finished and it will save the data as a **.csv file**.

<div align=center><img width=400 src="https://files.seeedstudio.com/wiki/K1100-quick-start/7.png"/></div>

<div align=center><img width=400 src="https://files.seeedstudio.com/wiki/K1100-quick-start/8.png"/></div>

If the storage on the TF card is full, it will notify as follows.

<div align=center><img width=400 src="https://files.seeedstudio.com/wiki/K1100-quick-start/9.png"/></div>
-->


## What can I do next？

Congratulations on successfully complete the steps listed in this quick starter guide, we just observed the data transferred from sensor device into SenseCAP Console / Ubidots view in least amount of time!


Next, if you want to dive further into sensor programming as well as LoRaWAN®, cloud platforms and more, continue with our [SenseCAP - The Sensor Prototype Kit Overview](https://wiki.seeedstudio.com/K1100-Getting-Started/).

<td align="center"><div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/K1101/41.png"/></div></td>



## Resources

- [GitHub][Seeed_Arduino_K1100 Source Code](https://github.com/Seeed-Studio/Seeed_Arduino_K1100)

## Troubleshooting

> Q1: Why does the data content from the SenseCAP platform differ from the data displayed in the Wio Terminal?

A: Because the data is read every five minutes and then sent out within three minutes. So the sensor value obtained may be the value read once in some five-minute interval with some delay. Whereas the data from Wio Terminal is in real time, which may lead to inconsistencies between the two.

> Q2: Why can't I connect to LoRaWAN®?

A: If you always can't connect to LoRaWAN®, please make sure that there is a strong enough signal and stable Helium network coverage within the surrounding environment. We recommend that you have a Helium gateway of your own to experience the full process. The Grove - Wio-E5 is only a prototype, so its antenna strength is not comparable to that of a large LoRa® device.
Moreover, this may also be related to the instability of the Helium network, which has been partially unresponsive since July 2022, and for which we can only wait for the official fix of Helium.

> Q3: Why is it that sometimes some sensor data in a set of content does not appear on SenseCAP?

A: It could be that Grove - Wio-E5 is experiencing packet loss. In general, packet loss is a normal situation and does not need to be overly concerned. If there is a very severe packet loss, you can try to restart Wio Terminal.
If you have very high requirements for real-time data and stability, you may want to check out the industrial-grade SenseCAP series devices.

> Q4: I did not purchase the kit for K1101, can I use the firmware to experience this routine?

A: If you are not a customer who purchased this kit, you may not be able to experience full access to SenseCAP. The Grove - Wio-E5 in the kit is a special edition, and products purchased separately do not have access to SenseCAP.

> Q5: If I use another program, can I recover the firmware?

A: You can burn this firmware for the Wio Terminal at any time, and in theory you can always use this routine as long as it does not destroy the original firmware of the Grove - Wio-E5. The method to burn the firmware is as follows.
Slide the switch twice very quickly, as followed:

<div align=center><img width = 400 src="https://files.seeedstudio.com/wiki/Wio-Terminal/img/Wio-Terminal-Bootloader.png"/></div>

At this point a disk called **Arduino** will appear on your computer, just copy the firmware (.uf2 file) to this disk. Once the firmware is installed, the disk will be automatically ejected and the Wio Terminal will reboot into the Setup Band interface.

> Q6: If I change the Dev EUI, App EUI, App Key of Grove - Wio-E5, is there a way to retrieve it?

A: We have opened up the enquiry interface to our users, so that when you have changed the information on the three codes to suit your project, you can also access them at any time with the sticker information on the back of the Grove - Wio-E5.

```
https://sensecap.seeed.cc/makerapi/device/view_device_info?nodeEui=<your_EUI>&deviceCode=<your_Code>
```

Please use the **EUI** and **Code** information marked on the back of the Grove - Wio-E5 to fill in the corresponding position on the above website to find out the corresponding three codes for your device by opening the website in your browser.

<div align=center><img width=800 src="https://files.seeedstudio.com/wiki/K1100-quick-start/36.png"/></div>

**Please take good care of the three codes, any loss caused by the loss of the codes will be the responsibility of the user.**

> Q7: Is it possible to use SenseCAP's DevEUI, AppEUI and AppKey on other platforms? For example Helium or TTN?

A: The answer to this is no. SenseCAP's triple code cannot be used to bind to other platforms, otherwise there is a high risk of data anomalies on different platforms.

## Tech Support

Please do not hesitate to submit the issue into our [forum](https://forum.seeedstudio.com/).


> **Upgradable to Industrial Sensors**
SenseCAP K1100 serves as a gateway to the higher-end SenseCAP LoRaWAN® series of industrial outdoor sensors. Users can prototype multiple AIoT applications with this kit, after which they can scale them up to the robust, long-life, and industrial SenseCAP sensors.

<div align=center><img width = 800 src="https://media-cdn.seeedstudio.com/media/wysiwyg/__16582252472624.png"/></div>



## Statement

- The LoRa® Mark is a trademark of Semtech Corporation or its subsidiaries.
- LoRaWAN® is a mark used under license from the LoRa Alliance®.
