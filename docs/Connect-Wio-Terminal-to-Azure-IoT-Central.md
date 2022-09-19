# Connect Wio Terminal to Microsoft Azure IoT Central

<p style="text-align:center;"><img src="https://files.seeedstudio.com/wiki/Azure_IoTc_WT/WT_client_send.png" alt="pir" width="1200" height="auto"></a></p>

## Introduction 
In this tutorial, we will walk you through the process of connecting the Wio Terminal to Microsoft Azure IoT Central and send telemetry data from the onboard sensors/ hardware on the Wio Terminal such as the 3-axis accelerometer, light sensor, 3 buttons to Microsoft Azure IoT Central. Then you will be able to visualize the sensor data on interactive dashboards. Also you will be able to use Azure IoT Central to control hardware such as beeping the onboard buzzer on the Wio Terminal. Microsoft Azure IoT Central supports HTTP, MQTT and AMQP protocols for communication, but, however we will be using the MQTT protocol in this tutorial.

### What is Microsoft Azure?

[Microsoft Azure](https://azure.microsoft.com) is Microsoft's public cloud computing platform. You can use Microsoft Azure to build, test, deploy, and manage applications and services through Microsoft-managed data centers. 

Also, it provides a range of cloud services, including compute, analytics, storage and networking. Microsoft Azure provides software as a service (SaaS), platform as a service (PaaS), Infrastructure as a service (IaaS) and serverless. Finally, it supports many different programming languages, tools and frameworks.

### What is Microsoft Azure IoT?

[Microsoft Azure IoT](https://azure.microsoft.com/en-us/overview/iot) is a collection of Microsoft-managed cloud services that connect, monitor, and control billions of IoT assets. It includes security and operating systems for devices and equipment, along with data and analytics that help businesses to build, deploy and manage IoT applications.

<p style="text-align:center;"><img src="https://files.seeedstudio.com/wiki/Azure_IoTc_WT/Azure_IoT.png" alt="pir" width="1200" height="auto"></a></p>

### What is Microsoft Azure IoT Central?

[Microsoft Azure IoT Central](https://azure.microsoft.com/en-us/services/iot-central) is a fully managed global IoT SaaS (software as a service) solution that makes it easy to connect, monitor and manage your IoT assets at scale. It is highly secure, scales with your business as it grows, ensures that your investments are repeatable and integrates with your existing business apps. It also bridges the gap between your business applications and IoT data. Finally it offers centralized management to reconfigure and update your devices.

### What is IoT Plug and Play?

[IoT Plug and Play](https://docs.microsoft.com/en-us/azure/iot-pnp) enables solutions builders to integrate smart devices with their solutions without any manual configuration. At the core of IoT Plug and Play, is a device model that a device uses to advertise its capabilities to an IoT Plug and Play-enabled application. It contains:

- Properties: represents the read-only or writable state of a device or other entity 
- Telemetry: data sent by a device
- Commands: describes a function or operation that can be done on a device

IoT Plug and Play certified devices eliminate the hassle of configuring devices in Azure IoT Central, such as creating templates and adding features and interfaces.

### IoT Plug and Play Certified Devices

IoT Plug and Play Certified Devices are devices listed in the [Azure Certified Device Catalog](https://devicecatalog.azure.com) with the IoT Plug and Play badge.

[Wio Terminal](https://www.seeedstudio.com/Wio-Terminal-p-4509.html) is an IoT Plug and Play Certified Device.

<p style="text-align:center;"><a href="https://devicecatalog.azure.com/devices/8b9c5072-68fd-4fc3-8e5f-5b15e3a20bd9"><img src="https://files.seeedstudio.com/wiki/Wio-Terminal/img/Wio-Terminal-Wiki.jpg" alt="pir" width="650" height="auto"></a></p>

To be IoT Plug and Play Certified, you will need to clear a few criteria, one of which is to publish a DTDL (Digital Twins Definition Language) model that defines the capabilities of the device to [Azure/ iot-plugandplay-models (DMR)](https://github.com/Azure/iot-plugandplay-models) on GitHub. 

This allows cloud services that use IoT Plug and Play Certified Devices to learn about device capabilities from this repository.

<p style="text-align:center;"><img src="https://files.seeedstudio.com/wiki/Azure_IoTc_WT/PnP_devices.png" alt="pir" width="850" height="auto"></a></p>

## Connecting Wio Terminal to Microsoft Azure IoT Central via MQTT

As explained before, we will be using MQTT for the communication between the Wio Terminal and Microsoft Azure IoT Central. However, you may use the HTTP bridge as well, if that is your requirement.

<p style="text-align:center;"><img src="https://files.seeedstudio.com/wiki/Azure_IoTc_WT/WT_client_send.png" alt="pir" width="1200" height="auto"></a></p>

<p style="text-align:center;"><img src="https://files.seeedstudio.com/wiki/Azure_IoTc_WT/WT_client_receive.png" alt="pir" width="1200" height="auto"></a></p>

### Microsoft Azure IoT Central Set Up 

First, you need to visit Microsoft Azure IoT Central, log in to your Microsoft account and create a new application for your project.

**Step 1.** Log in to Azure IoT Central.

Please go to the official [Azure IoT Central](https://apps.azureiotcentral.com/home) website, click **Build** from the navigation menu on the left, and click **Custom apps**.

If you are not already logged into Azure IoT Central, you will be asked to complete a registration/login at this point.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/k1100_azure_central/1.png"/></div>


**Step 2.** Fill in **Application name** and choose **Standard 0** under the **Pricing plan**. Application URL will be created automatically when you fill in application name. In this example, we will accomplish this task at minimal cost.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/k1100_azure_central/2.png"/></div>

!!!Note
    If you are a new user of Azure IoT Central, we recommend that you select **Free** as this will not consume your fees.
    <div align=center><img width = 500 src="https://files.seeedstudio.com/wiki/k1100_azure_central/9.png"/></div>

**Step 3.** Click **Create** to create the new application. Now you have successfully set up Azure IoT Central!

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/k1100_azure_central/3.png"/></div>

### Get the information to connect from Azure IoT Central

Once we have created a new application according to the steps above, we go to the **Permissions** screen in the left-hand toolbar.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/k1100_azure_central/10.png"/></div>

You will then see that there are two Enrollment groups here.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/k1100_azure_central/11.png"/></div>

The choice we need to make is **SAS-IoT-Devices**.

Here we need the information there.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/k1100_azure_central/12.png"/></div>

```
ID scope
Primary key
```

Please make a note of this information, which we will use in the next steps.

### Set Up Wio Terminal 

**Step 1.** Download the firmware to your computer

Our second firmware release, SenseCraft, will support Azure IoT Central in the easiest way possible.

Under the **Latest release**, click on **SenseCraft-vx.x.uf2** to download the .uf2 file.

<p style=":center"><a href="https://github.com/Seeed-Studio/SenseCraft/releases" target="_blank"><div align=center><img width = 300 src="https://files.seeedstudio.com/wiki/seeed_logo/github.png" /></div></a></p>


**Step 2.** Upload the firmware to Wio Terminal

Connect the Wio Terminal to PC and turn in ON, Enter **Bootloader Mode** by sliding down the power switch further away from "ON" position, release, slide again and release.

<p style="text-align:center;"><img src="https://files.seeedstudio.com/wiki/Wio-Terminal/img/Wio-Terminal-Bootloader.png" alt="pir" width="500" height="auto"></a></p>

!!!Note
    Once Wio Terminal is in the Bootloader mode, the blue LED will start to breathe in a way that is different from blinking.

Open File Explorer on your PC and you will see a new external drive, named **Arduino**, drag the previously downloaded **.uf2 file** into this **Arduino drive**. 

Now we have successfully uploaded the firmware into the Wio Terminal.

**Step 3.** Wi-Fi and Azure IoT Configuration

When we use the **SenseCraft** firmware, a USB stick appears as soon as the Wio Terminal is connected to the computer.

!!!Attention
    This means that the USB stick appears when the Wio Terminal is connected to the computer, **without the need to toggle the side button twice extra**!

At this point, there will be a **config.txt** file inside the USB stick. Please open it with a suitable txt editor and add the required configuration to it.

```
SSID=<YOUR-WIFISSD>                   // Put your WifiSSID here
PASSWORD=<YOUR-WIFI-PASSWORD          // Put your wifi password here
ID_SCOPE=<YOUR-DEVICE-ID-SCOPE>       // Put your device ID here
PRIMAEY_KEY=<YOUR-DEVICE-PRIMAEY_KEY> // Put your device Primary key here
DEVICE_NAME=<YOUR-DEVICE_NAME>        // Put your device name here
```

!!!Note
    Where **DEVICE_NAME** is a freely fillable item according to your needs. Once you have successfully connected to Azure IoT Central, you will see the device with that name, appearing inside the **Device**.
    <div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/k1100_azure_central/13.png"/></div>


Once you have completed the above, please save it.


### Display Telemetry Data on Microsoft Azure IoT Central 

**Step 1.** View data dashboard

We will move on to displaying the incoming telemetry data from the 3-axis accelerometer, light sensor and 3 buttons of the Wio Terminal on Azure IoT Central Dashboard.

Open Azure IoT Central Dashboard that you visited before. Click on **Devices** from the left navigation menu. You will see **Seeed Wio Terminal** appear under Devices. Click on it.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/k1100_azure_central/14.png"/></div>

Click on the entry with the **device name** that you configured before. 

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/k1100_azure_central/13.png"/></div>

Now you will be able to visualize the data from the onboard 3-axis accelerometer on an interactive dashboard.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/k1100_azure_central/8.png"/></div>

This is the default view and we need to make some changes to display the other telemetry data as well.

**Step 2.** Configure the template

Click on **Device templates** from the left navigation menu and click **Seeed Wio Terminal** to configure the template.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/k1100_azure_central/15.png"/></div>

Click on **Overview** on the left navigation menu.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/k1100_azure_central/16.png"/></div>

Collapse **select a telemetry** drop-down menu and select the telemetry that you want to visualize. 

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/k1100_azure_central/17.png"/></div>

Click **Add tile** and you will see the tile added into the Azure IoT Central Dashboard.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/k1100_azure_central/18.png"/></div>

**Note:** You can resize or change the visualization of the tiles according to your preference

<div align=center><img width = 400 src="https://files.seeedstudio.com/wiki/k1100_azure_central/19.png"/></div>

Click **Save**.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/k1100_azure_central/20.png"/></div>

Click **Publish**.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/k1100_azure_central/21.png"/></div>

**Step 3.** Observe changes in data

Go back to Azure IoT Central dashboard and you will be able to visualize all the data coming in from the Wio Terminal.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/k1100_azure_central/23.png"/></div>

You can also click on the **Raw data** tab to view all the telemetry data in real-time.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/k1100_azure_central/22.png"/></div>
