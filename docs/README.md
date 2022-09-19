<style>
td, th {
   border: none!important;
}

.item a {
    text-decoration: none;
    display: inline-block;
    /* border: 1px solid #CCC; */
    /* font-size: 15px; */
}

.item ul{
    list-style-type: none;
    width: 100%;
}

.item {
    margin-left: 0;
}

.item ul li {
    display: inline-block;
}

.headline::before{
    content: '';
    border-top: 6px solid transparent;
    border-right: 6px solid #7cb342;
    position: relative;
    left: -2px;
}
.partner{

}

</style>

# SenseCAP - The Sensor Prototype Kit Overview

In the digital age where IoT and big data are becoming more prevalent, massive data collection through sensors is required to facilitate digital transformation. Under such a background, SenseCAP K1100 comes with all the essential sensors and devices, including a Wio Terminal, an AI Vision Sensor, and a LoRa®-enabled module - that together allows users of all backgrounds and levels to create DIY sensors for data collection and solving real-world challenges.

<div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/K1100/banner.png"/></div>

**SenseCAP - The Sensor Prototype Kit with LoRa® and AI** enables you to rapidly digitize the world using LoRaWAN® and solve real-world challenges. With this plug-and-play toolkit, anyone can add AI to their edge devices and unlock the potential of AIoT.

<p align=center><a href="https://www.seeedstudio.com/Seeed-Studio-LoRaWAN-Dev-Kit-p-5370.html?queryID=a88444c7c4ccfa5dddd4d2a84db3dd5e&objectID=5370&indexName=bazaar_retailer_products" target="_blank"><img src="https://files.seeedstudio.com/wiki/Seeed-WiKi/docs/images/get_one_now.png" /></a></p>

## Catalogue

<div class="category">
    <h3>
        <span class="headline" id="Quick Start">Quick Start</span>
    </h3>
    <div class="item">
        <ul>
            <li><a href="https://wiki.seeedstudio.com/K1100-quickstart/" target="_blank">Quick Start With SenseCAP - K1100</a>
        </ul>
    </div>
	<h3>
         <span class="headline" id="Microsoft Azure IoT">Microsoft Azure IoT</span>
    </h3>
    <div class="item">
        <ul>
            <li><a href="https://wiki.seeedstudio.com/K1100_SenseCAP_to_Azure_IoT_Central/" target="_blank">Connect SenseCAP to Azure IoT Central via Node-RED (SenseCAP)</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/Connect-Wio-Terminal-to-Azure-IoT-Central/" target="_blank">Connect Wio Terminal to Microsoft Azure IoT Central (WiFi)</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/Wio-Terminal-Azure-IOT/" target="_blank">Connect the Wio Terminal to Microsoft Azure IoT Hub (WiFi)</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/Integrate-into-Azure-IoT-Hub/" target="_blank">Integrate into Azure IoT Hub (Helium)</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/Develop-in-Microsoft-Azure-IoT-Central/" target="_blank">Develop in Microsoft Azure IoT Central</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/Configuring-Web-APP-Visualization/" target="_blank">Configure Web APP Visualization</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/K1100_Azure_to_PowerBI/" target="_blank">Microsoft Azure IoT Data visualisation with Power BI</a></li>
        </ul>
        <ul>
            <li><a href="https://github.com/microsoft/IoT-For-Beginners" target="_blank">Microsoft IoT Hub for Beginners Course</a></li>
        </ul>
    </div>
    <h3>
        <span class="headline" id="SenseCAP">SenseCAP</span>
    </h3>
    <div class="item">
        <ul>
            <li><a href="https://sensecap-docs.seeed.cc/introduction.html" target="_blank">SenseCAP Documentation Centre</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/K1100_sensecap_node-red/" target="_blank">SenseCAP Console with Node-RED Quick Start</a>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/K1100_SenseCAP_to_Azure_IoT_Central/" target="_blank">Connect SenseCAP to Azure IoT Central via Node-RED</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/K1100_SenseCAP_to_influxdb/" target="_blank">Connect SenseCAP to InfluxDB via Node-RED</a></li>
        </ul>
    </div>
    <h3>
        <span class="headline" id="Arduino Programming">Sensors Wiki</span>
    </h3>
    <div class="item">
        <ul>
            <li><a href="https://wiki.seeedstudio.com/Wio-Terminal-Light/" target="_blank">Wio Terminal Light Sensor</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/Wio-Terminal-IMU-Overview/" target="_blank">Wio Terminal IMU Sensor</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/Grove-Moisture_Sensor/" target="_blank">Grove Soil Moisture Sensor</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/Grove-VOC_and_eCO2_Gas_Sensor-SGP30/" target="_blank">Grove VOC and eCO2 Gas Sensor (SGP30)</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/Grove-SHT4x/" target="_blank">Grove Temp&Humi Sensor (SHT40)</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/Grove-Vision-AI-Module/" target="_blank">Grove Vision AI Module</a></li>
        </ul>
    </div>
    <h3>
        <span class="headline" id="Arduino Programming">Sensor & Helium/TTN Arduino Programme</span>
    </h3>
    <div class="item">
        <ul>
            <li><a href="https://wiki.seeedstudio.com/K1100-Light-Sensor-Grove-LoRa-E5/" target="_blank">Wio Terminal Light Sensor</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/K1100-IMU-Sensor-Grove-LoRa-E5/" target="_blank">Wio Terminal IMU Sensor</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/K1100-Soil-Moisture-Sensor-Grove-LoRa-E5/" target="_blank">Grove Soil Moisture Sensor</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/K1100-VOC-and-eCO2-Gas-Sensor-Grove-LoRa-E5/" target="_blank">Grove VOC and eCO2 Gas Sensor (SGP30)</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/K1100-Temp-Humi-Sensor-Grove-LoRa-E5/" target="_blank">Grove Temp&Humi Sensor (SHT40)</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/K1100-Vision-AI-Module-Grove-LoRa-E5/" target="_blank">Grove Vision AI Module</a></li>
        </ul>
    </div>
    <h3>
        <span class="headline" id="Helium">Helium</span>
    </h3>
    <div class="item">
        <ul>
            <li><a href="https://wiki.seeedstudio.com/Helium-Introduction/" target="_blank">Helium Introduction</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/Connecting-to-Helium/" target="_blank">Connect to Helium</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/Integrate_into_Google_Sheets_via_Helium/" target="_blank">Integrate into Google Sheets via Helium</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/Connect_AWS_via_helium/" target="_blank">Connect AWS IOT Core via Helium (Helium)</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/Integrate-into-Azure-IoT-Hub/" target="_blank">Integrate into Azure IoT Hub</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/Configuring-Web-APP-Visualization/" target="_blank">Configure Web APP Visualization</a></li>
        </ul>
    </div>
    <h3>
         <span class="headline" id="The Things Network Section">The Things Network Section</span>
    </h3>
    <div class="item">
        <ul>
            <li><a href="https://wiki.seeedstudio.com/TTN-Introduction/" target="_blank">TTN Introduction</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/Connecting-to-TTN/" target="_blank">Connect to TTN</a></li>
        </ul>
    </div>
	<h3>
         <span class="headline" id="WiFi">WiFi</span>
    </h3>
    <div class="item">
        <ul>
            <li><a href="https://wiki.seeedstudio.com/Getting_started_with_Ubidots/" target="_blank">Quickly Start with Ubidots</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/Quick-Start-to-using-Blynk/" target="_blank">Quick Start to Using Blynk</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/Wio-Terminal-Advanced-WiFi/" target="_blank">Advanced Wi-Fi Usage</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/Connect-Wio-Terminal-to-Google-Cloud/" target="_blank">Connect Wio Terminal to Google Cloud IoT Core</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/Connect-Wio-Terminal-to-Azure-IoT-Central/" target="_blank">Connect Wio Terminal to Microsoft Azure IoT Central</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/Wio-Terminal-Azure-IOT/" target="_blank">Connect the Wio Terminal to Microsoft Azure IoT Hub</a></li>
        </ul>
    </div>
	<h3>
         <span class="headline" id="Cloud">Cloud</span>
    </h3>
    <div class="item">
        <ul>
            <li><a href="https://wiki.seeedstudio.com/Connect_AWS_via_helium/" target="_blank">Connect AWS IOT Core via Helium (Helium)</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/Integrate-into-Azure-IoT-Hub/" target="_blank">Integrate into Azure IoT Hub (Helium)</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/Connect-Wio-Terminal-to-Google-Cloud/" target="_blank">Connect Wio Terminal to Google Cloud IoT Core (WiFi)</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/Connect-Wio-Terminal-to-Azure-IoT-Central/" target="_blank">Connect Wio Terminal to Microsoft Azure IoT Central (WiFi)</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/Wio-Terminal-Azure-IOT/" target="_blank">Connect the Wio Terminal to Microsoft Azure IoT Hub (WiFi)</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/Getting_started_with_Ubidots/" target="_blank">Quickly Start with Ubidots (WiFi)</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/Quick-Start-to-using-Blynk/" target="_blank">Quick Start to Using Blynk (WiFi)</a></li>
        </ul>
    </div>
	<h3>
         <span class="headline" id="TinyML">TinyML</span>
    </h3>
    <div class="item">
        <ul>
            <li><a href="https://wiki.seeedstudio.com/Wio-Terminal-TinyML-EI-1/" target="_blank">Wio Terminal Edge Impulse Getting Started (built-in sensor)</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/K1111-Edge-Impulse/" target="_blank">Develop Edge Impulse application to cloud via Helium (built-in sensor)</a></li>
        </ul>
        <ul>
            <li><a href="https://wiki.seeedstudio.com/Train-Deploy-AI-Model/" target="_blank">Train and Deploy Your Own AI Model Into Grove - Vision AI</a></li>
        </ul>
    </div>
	<h3>
         <span class="headline" id="Project">Project</span>
    </h3>
    <div class="item">
        <ul>
            <li><a href="https://wiki.seeedstudio.com/K1111-Quick-Start-Guide/" target="_blank">Soil moisture monitoring alarm system</a></li>
        </ul>
        <ul>
            <li><a href="https://www.hackster.io/hendra/environment-data-lora-node-c91415" target="_blank">Grove Wio E5 peer-to-peer communication</a></li>
        </ul>
    </div>
</div>



## <span id="jump1">Introduction</span>

### Quick Start With SenseCAP - K1100

<html>
    <table style="margin-left: auto; margin-right: auto;">
        <tr>
            <td>
                <!--左侧内容-->
	    		<td align="left">In this tutorial, we would like you to start by going through our <strong>Quick Start Guide</strong> section. Learn about the extraordinary data presentation of our kit and the ease of IoT functionality. In this chapter, you can experience the fun of data collection and data processing with our open source project in the shortest time. And you can transfer the data you collected to different platforms through WiFi or LoRa®.</br></br><a href="https://wiki.seeedstudio.com/K1100-quickstart/">Jump to chapter ></a></br></br><div align=center><img width = 700 src="https://files.seeedstudio.com/wiki/K1100-quick-start/step.png"></div></td>
            </td>
            <td>
                <!--右侧内容-->
				<br></br><br>
                <div align=center><img width = 600 src="https://files.seeedstudio.com/wiki/K1100_overview/1.png"></div>
            </td>
        </tr>
    </table>
</html>

### SenseCAP Cloud Applications

<html>
    <table style="margin-left: auto; margin-right: auto;">
        <tr>
            <!--左侧内容-->
	    	<td colspan="2">To make it easier for our users to connect data from the SenseCAP platform to various other Paas platforms for more in-depth data processing. We will do a series of tutorials for SenseCAP & Node-RED.</br></br><a href="https://wiki.seeedstudio.com/K1100-quickstart/">Jump to chapter ></a></td>
        </tr>
        <tr>
            <td><strong>SenseCAP Console with Node-RED Quick Start</strong></td>
            <td><strong>Connect SenseCAP to Azure IoT Central via Node-RED</strong></td>
        </tr>
        <tr>
            <td>This tutorial, the first in a series, will walk you through installing and using Node-red and calling the SenseCAP backend API to connect to Node-RED.</td>
            <td>The content of this chapter will continue to use the Node-RED introduced earlier and facilitate the management of the K1100 kit in Microsoft Azure IoT Central through the use of Node-RED.</td>
        </tr>
        <tr>
            <td><a href="https://wiki.seeedstudio.com/K1100_sensecap_node-red/">Jump to chapter ></a></td>
            <td><a href="https://wiki.seeedstudio.com/K1100_SenseCAP_to_Azure_IoT_Central/">Jump to chapter ></a></td>
        </tr>
        <tr>
            <td><strong>Connecting SenseCAP to InfluxDB via Node-RED</strong></td>
            <td><strong></strong></td>
        </tr>
        <tr>
            <td>The content of this chapter will continue to use the Node-RED introduced earlier and facilitate the management of the K1100 kit in InfluxDB through the use of Node-RED.</td>
            <td></td>
        </tr>
        <tr>
            <td><a href="https://wiki.seeedstudio.com/K1100_SenseCAP_to_influxdb/">Jump to chapter ></a></td>
            <td></td>
        </tr>
    </table>
</html>

### Microsoft Azure IoT

<html>
    <table style="margin-left: auto; margin-right: auto;">
		<tr>
            <td colspan="2">Microsoft Azure IoT is a collection of Microsoft-managed cloud services that connect, monitor, and control billions of IoT assets. It includes security and operating systems for devices and equipment, along with data and analytics that help businesses to build, deploy and manage IoT applications.
			</td>
			<td>
                <!--右侧内容-->
                <div align=center><img width = 600 src="https://files.seeedstudio.com/wiki/K1100/microsoft-azure.png"></div>
            </td>
		</tr>
        <tr>
            <td><strong>Connect SenseCAP to Azure IoT Central via Node-RED</strong></td>
            <td><strong>Connect Wio Terminal to Microsoft Azure IoT Central (WiFi)</strong></td>
        </tr>
        <tr>
            <td>In this chapter, we will describe how to forward data stored in SenseCAP to Azure IoT Central by using Node RED.</td>
            <td>In this chapter, we will describe how to send data from the Wio Terminal to Azure IoT Central via the WiFi function that comes with the Wio Terminal.</td>
        </tr>
        <tr>
            <td><a href="https://wiki.seeedstudio.com/K1100_SenseCAP_to_Azure_IoT_Central/">Jump to chapter ></a></td>
            <td><a href="https://wiki.seeedstudio.com/Connect-Wio-Terminal-to-Azure-IoT-Central/">Jump to chapter ></a></td>
            <td rowspan="2"><div align=center><img width = 600 src="https://files.seeedstudio.com/wiki/K1100/central.png"></div></td>
        </tr>
        <tr>
            <td><strong>Develop in Microsoft Azure IoT Central</strong></td>
        </tr>
        <tr>
            <td align="left">In this chapter we will describe how to process data and further develop it in Azure IoT Central.</td>
        </tr>
        <tr>
            <td><a href="https://wiki.seeedstudio.com/Develop-in-Microsoft-Azure-IoT-Central/">Jump to chapter ></a></td>
        </tr>
        <tr>
            <td><strong>Connect the Wio Terminal to Microsoft Azure IoT Hub (WiFi)</strong></td>
            <td><strong>Integrate into Azure IoT Hub (Helium)</strong></td>
        </tr>
        <tr>
	        <td align="left">In this chapter, we will describe how to send data from the Wio Terminal to Azure IoT Hub via the WiFi function that comes with the Wio Terminal.</td>
            <td align="left">This chapter describes the use of Azure IoT Hub in the Helium console.</td>
        </tr>
        <tr>
            <td><a href="https://wiki.seeedstudio.com/Integrate-into-Azure-IoT-Hub">Jump to chapter ></a></td>
            <td><a href="https://wiki.seeedstudio.com/Integrate-into-Azure-IoT-Hub/">Jump to chapter ></a></td>
            <td rowspan="2"><div align=center><img width = 200 src="https://files.seeedstudio.com/wiki/K1100/hub.png"></div></td>
        </tr>
        <tr>
            <td><strong>Microsoft IoT Hub for Beginners Course</strong></td>
        </tr>
        <tr>
            <td align="left">The course cover the journey of food from farm to table. This includes farming, logistics, manufacturing, retail and consumer - all popular industry areas for IoT devices.</td>
        </tr>
        <tr>
            <td><a href="https://github.com/microsoft/IoT-For-Beginners">Jump to chapter ></a></td>
        </tr>
        <tr>
            <td><strong>Microsoft Azure IoT Data visualisation with Power BI</strong></td>
            <td><strong>Configure Web APP Visualization</strong></td>
        </tr>
        <tr>
            <td align="left">In this tutorial, you configure your IoT Central application, then use Power BI store, manager to visualize the insights derived from the telemetry.</td>
            <td align="left">This section describes how to visualize the data information received by Microsoft Azure using the free Web APP integration.</td>
        </tr>
        <tr>
            <td><a href="https://wiki.seeedstudio.com/K1100_Azure_to_PowerBI/">Jump to chapter ></a></td>
            <td><a href="https://wiki.seeedstudio.com/Configuring-Web-APP-Visualization/">Jump to chapter ></a></td>
        </tr>
    </table>
</html>



### Arduino Programming Sensors Section
<html>
    <table style="margin-left: auto; margin-right: auto;">
		<tr>
            <td colspan="3">If you are an engineer interested in hardware/software or have been in the industry for years, you can start your journey of discovery with our <strong>Arduino</strong> section. You don't have to worry about the content in the cloud. The easy-to-understand tutorials will enable you to complete various IoT projects with minimal time cost.
			</td>
			<td>
                <!--右侧内容-->
                <div><img width = 1500 src="https://files.seeedstudio.com/wiki/K1100_overview/2.png"></div>
            </td>
		</tr>
        <tr>
            <td bgcolor="#93dfef"><div align=center><img width = 150 src="https://files.seeedstudio.com/wiki/K1100_overview/light.png"></div>
            </td>
            <td bgcolor="#93dfef"><div align=center><img width = 150 src="https://files.seeedstudio.com/wiki/K1100_overview/imu.png"></div>
            </td>
            <td bgcolor="#93dfef"><div align=center><img width = 150 src="https://files.seeedstudio.com/wiki/K1100_overview/soil.png"></div>
            </td>
        </tr>
        <tr>
            <td><strong>Wio Terminal Light Sensor</strong></td>
            <td><strong>Wio Terminal IMU Sensor</strong></td>
            <td><strong>Soil Moisture Sensor</strong></td>
        </tr>
        <tr>
            <td>This section describes how to use the Wio Terminal Light Sensor to acquire data and connect to the LoRa® network via the Grove - Wio-E5 in preparation for connecting to the Helium/TTN.</td>
            <td>This section describes how to use the Wio Terminal IMU Sensor to acquire data and connect to the LoRa® network via the Grove - Wio-E5 in preparation for connecting to the Helium/TTN.</td>
            <td>This section describes how to use the Grove Soil Moisture Sensor to acquire data and connect to the LoRa® network via the Grove - Wio-E5 in preparation for connecting to the Helium/TTN.</td>
        </tr>
        <tr>
            <td><a href="https://wiki.seeedstudio.com/K1100-Light-Sensor-Grove-LoRa-E5/">Jump to chapter ></a></td>
            <td><a href="https://wiki.seeedstudio.com/K1100-IMU-Sensor-Grove-LoRa-E5/">Jump to chapter ></a></td>
            <td><a href="https://wiki.seeedstudio.com/K1100-Soil-Moisture-Sensor-Grove-LoRa-E5/">Jump to chapter ></a></td>
        </tr>
        <tr>
            <td bgcolor="#93dfef"><div align=center><img width = 150 src="https://files.seeedstudio.com/wiki/K1100_overview/sgp30.png"></div>
            </td>
            <td bgcolor="#93dfef"><div align=center><img width = 150 src="https://files.seeedstudio.com/wiki/K1100_overview/sht40.png"></div>
            </td>
            <td bgcolor="#93dfef"><div align=center><img width = 150 src="https://files.seeedstudio.com/wiki/K1100_overview/ai.png"></div>
            </td>
        </tr>
        <tr>
            <td><strong>Grove VOC and eCO2 Gas Sensor (SGP30)</strong></td>
            <td><strong>Grove Temp&Humi Sensor (SHT40)</strong></td>
            <td><strong>Grove Vision AI Module</strong></td>
        </tr>
        <tr>
            <td>This section describes how to use the Grove VOC and eCO2 Gas Sensor to acquire data and connect to the LoRa® network via the Grove - Wio-E5 in preparation for connecting to the Helium/TTN.</td>
            <td>This section describes how to use the Wio Terminal Light Sensor to acquire data and connect to the LoRa® network via the Grove - Wio-E5 in preparation for connecting to the Helium/TTN.</td>
            <td >This section describes how to use the Wio Terminal Light Sensor to acquire data and connect to the LoRa® network via the Grove - Wio-E5 in preparation for connecting to the Helium/TTN.</td>
        </tr>
        <tr>
            <td><a href="https://wiki.seeedstudio.com/K1100-VOC-and-eCO2-Gas-Sensor-Grove-LoRa-E5/">Jump to chapter ></a></td>
            <td><a href="https://wiki.seeedstudio.com/K1100-Temp-Humi-Sensor-Grove-LoRa-E5/">Jump to chapter ></a></td>
            <td><a href="https://wiki.seeedstudio.com/K1100-Vision-AI-Module-Grove-LoRa-E5/">Jump to chapter ></a></td>
        </tr>
    </table>
</html>

### Helium Section

<html>
    <table style="margin-left: auto; margin-right: auto;">
		<tr>
            <td colspan="2">To send the sensor values to the cloud platform, the choice of network is a necessary part. We offer two platforms with the widest coverage in LoRa® for you to choose from today. Here is Helium Section.
			</td>
			<td>
                <!--右侧内容-->
                <div align=center><img width = 550 src="https://files.seeedstudio.com/wiki/K1100_overview/3.png"></div>
            </td>
		</tr>
        <tr>
            <td><strong>Helium Introduction</strong></td>
            <td><strong>Connecting to Helium</strong></td>
        </tr>
        <tr>
            <td>In this chapter, we will introduce the Helium console controls that we use to get a first impression of the Helium console.</td>
            <td>This section describes how to configure Helium so that sensor data can be successfully uploaded to and displayed in Helium.</td>
        </tr>
        <tr>
            <td><a href="https://wiki.seeedstudio.com/Helium-Introduction">Jump to chapter ></a></td>
            <td><a href="https://wiki.seeedstudio.com/Connecting-to-Helium">Jump to chapter ></a></td>
        </tr>
        <tr>
            <td><strong>Helium integrate into Azure IoT Hub</strong></td>
            <td><strong>Helium integrate into Google Sheets</strong></td>
        </tr>
        <tr>
	        <td align="left">This section describes how to configure Microsoft Azure and integrate with Helium to upload data to the Microsoft Azure platform.</td>
            <td align="left">This section introduces the use of Google Forms in Helium integration. and using Google Forms to store sensor data.</td>
        </tr>
        <tr>
            <td><a href="https://wiki.seeedstudio.com/Integrate-into-Azure-IoT-Hub">Jump to chapter ></a></td>
            <td><a href="https://wiki.seeedstudio.com/Integrate_into_Google_Sheets_via_Helium/">Jump to chapter ></a></td>
        </tr>
        <tr>
            <td><strong>Helium integrate into AWS IOT Core</strong></td>
        </tr>
        <tr>
            <td align="left">This section describes how to configure and use the AWS integration in Helium.</td>
        </tr>
        <tr>
            <td><a href="https://wiki.seeedstudio.com/Connect_AWS_via_helium/">Jump to chapter ></a></td>
        </tr>
        <tr>
            <td colspan="3"><div align=center><img width = 600 src="https://files.seeedstudio.com/wiki/Wio-Terminal-Developer-for-helium/113.png"></div></td>
        </tr>   
    </table>
</html>


### The Things Network Section

<html>
    <table style="margin-left: auto; margin-right: auto;">
		<tr>
            <td colspan="2">To send the sensor values to the cloud platform, the choice of network is a necessary part. We offer two platforms with the widest coverage in LoRa® for you to choose from today. Here is The Things Network Section. (Hereinafter referred to as TTN)
			</td>
			<td>
                <!--右侧内容-->
                <div align=center><img width = 450 src="https://files.seeedstudio.com/wiki/Wio-Terminal-Developer-for-TTN/TTN.png"></div>
            </td>
		</tr>
        <tr>
            <td><strong>TTN Introduction</strong></td>
            <td><strong>Connecting to TTN</strong></td>
        </tr>
        <tr>
            <td>In this chapter, we will introduce the TTN console controls that we use to get a first impression of the TTN console.</td>
            <td>This section describes how to configure TTN so that sensor data can be successfully uploaded to and displayed in TTN.</td>
        </tr>
        <tr>
            <td><a href="https://wiki.seeedstudio.com/TTN-Introduction">Jump to chapter ></a></td>
            <td><a href="https://wiki.seeedstudio.com/Connecting-to-TTN">Jump to chapter ></a></td>
        </tr>
        <tr>
            <td colspan="3"><div align=center><img width = 600 src="https://files.seeedstudio.com/wiki/K1100/overview.png"></div></td>
        </tr>
    </table>
</html>


### Wio Terminal WiFi Section

<html>
    <table style="margin-left: auto; margin-right: auto;">
		<tr>
            <td colspan="3">If you are in an environment where LoRaWAN® is not available, you can also choose to use the WiFi function of Wio Terminal to complete the point-to-point communication.
			</td>
			<td>
                <!--右侧内容-->
                <div align=center><img width = 60 src="https://files.seeedstudio.com/wiki/K1100_overview/wifi.png"></div>
            </td>
		</tr>
        <tr>
            <td><strong>Advanced WiFi Usage</strong></td>
            <td><strong>Connect Wio Terminal to Microsoft Azure IoT Central</strong></td>
            <td colspan="2"><strong>Quick Start with Ubidots</strong></td>
        </tr>
        <tr>
            <td align="left">This section introduces some of the advanced library usage of the Wi-Fi such as HTTPClient, DNSServer and WebServer Libraries. With these libraries implemented, you are able to develop your IoT projects with simple APIs.</td>
            <td align="left">In this section, we will walk you through the process of connecting the Wio Terminal to Microsoft Azure IoT Central and send telemetry data from the onboard sensors/ hardware on the Wio Terminal such as the 3-axis accelerometer, light sensor, 3 buttons to Microsoft Azure IoT Central.</td>
            <td align="left" colspan="2">In this chapter, we will describe how to experience the process of uploading data to Ubidots and displaying it graphically through your own WiFi network.</td>
        </tr>
        <tr>
            <td><a href="https://wiki.seeedstudio.com/Wio-Terminal-Advanced-WiFi/">Jump to chapter ></a></td>
            <td><a href="https://wiki.seeedstudio.com/Connect-Wio-Terminal-to-Azure-IoT-Central/">Jump to chapter ></a></td>
            <td colspan="2"><a href="https://wiki.seeedstudio.com/Getting_started_with_Ubidots/">Jump to chapter ></a></td>
        </tr>
        <tr>
            <td><strong>Quick Start to using Blynk</strong></td>
            <td><strong>Connect Wio Terminal to Google Cloud IoT Core</strong></td>
        </tr>
        <tr>
            <td align="left">In this chapter, we will describe how to experience the process of uploading data to Blynk and displaying it graphically through your own WiFi network.</td>
            <td align="left">In this section, we will walk you through the process of connecting the Wio Terminal to Google Cloud IoT Core and send telemetry data from the Wio Terminal to the Google Cloud IoT core.</td>
        </tr>
        <tr>
            <td><a href="https://wiki.seeedstudio.com/Quick-Start-to-using-Blynk/">Jump to chapter ></a></td>
            <td><a href="https://wiki.seeedstudio.com/Connect-Wio-Terminal-to-Google-Cloud/">Jump to chapter ></a></td>
            <td colspan="2"></td>
        </tr>
    </table>
</html>

<table align="center">
	
</table>


### TinyML Section

<html>
    <table style="margin-left: auto; margin-right: auto;">
		<tr>
            <td colspan="2">Of course, if you are interested in TinyML and want to implement some machine learning projects and content, you can refer to the TinyML section.
			</td>
			<td>
                <!--右侧内容-->
                <div align=center><img width = 900 src="https://files.seeedstudio.com/wiki/K1100_overview/tinyml.png"></div>
            </td>
        </tr>
        <tr>
            <td><strong>Develop Edge Impulse application to cloud via Helium</strong></td>
            <td><strong>Train and Deploy Your Own AI Model Into Grove - Vision AI</strong></td>
        </tr>
        <tr>
            <td align="left">Combine Wio Terminal's built-in sensors with Edge Impulse, a powerful and easy-to-use machine learning platform, to train simple sensor recognition projects.</td>
            <td align="left">In this wiki, we will teach you how to train your own AI model for your specific application and then deploy it easily to the SenseCAP A1101 - LoRaWAN Vision AI Sensor and Grove - Vision AI Module.</td>
		</tr>
        <tr>
            <td><a href="https://wiki.seeedstudio.com/K1111-Edge-Impulse/">Jump to chapter ></a></td>
            <td><a href="https://wiki.seeedstudio.com/Train-Deploy-AI-Model/">Jump to chapter ></a></td>
            <td colspan="2"></td>
        </tr>
    </table>
</html>


### Project Section

<html>
    <table style="margin-left: auto; margin-right: auto;">
		<tr>
            <td>In addition to the above basic tutorials for each platform, we have prepared a variety of rich examples on the use of this kit, so you can see here that the value of the kit is unlimited!
			</td>
			<td>
                <!--右侧内容-->
                <div align=center><img width = 350 src="https://files.seeedstudio.com/wiki/K1100_overview/project.png"></div>
            </td>
		</tr>
    </table>
</html>

<table align="center">
	<tr>
	    <td align="center"><p style="text-align:center;"><img src="https://files.seeedstudio.com/wiki/LoraWANdevKit/lorawandevkit33.jpg" alt="pir" width="1100" height="auto"></p></td>
	    <td align="left"><strong>Soil moisture monitoring alarm system</strong></br></br>In this section, we will show you how to unitize the Wio Terminal to link Grove - Wio-E5 and Grove - Soil Moisture Sensor, connecting them with Helium and Azure IoT Central, to build a real-time backyard potting monitor and watering-reminder.</br></br><a href="https://wiki.seeedstudio.com/soil-alarm-system/">Jump to chapter ></a></td>
	</tr>
    <tr>
	    <td colspan="2" align="center"><b>The project is still being updated !!!</b></td>
	</tr>
</table>


We hope you enjoy your learning journey in IoT!

## Course Code Library

All libraries and code for this course can be found at the following address Before the course starts, you can clone the repo to your own computer for reference and learning.

<p style=":center"><a href="https://github.com/limengdu/Seeed-Studio-LoRaWAN-Dev-Kit" target="_blank"><div align=center><img width = 300 src="https://files.seeedstudio.com/wiki/seeed_logo/github.png" /></div></a></p>

## Helium LoRaWAN®

For more details, please read [Helium LoRaWAN®: Public Infrastructure with Private Network Servers](https://blog.helium.com/helium-lorawan-public-infrastructure-with-ultimate-privacy-9132560e4ae6).

## The Things Network LoRaWAN®

For more details, please read [TTN offcial website](https://www.thethingsnetwork.org/docs/).


## Tech Support

Please do not hesitate to submit the issue into our [forum](https://forum.seeedstudio.com/).


> **Upgradable to Industrial Sensors**
SenseCAP K1100 serves as a gateway to the higher-end SenseCAP LoRaWAN® series of industrial outdoor sensors. Users can prototype multiple AIoT applications with this kit, after which they can scale them up to the robust, long-life, and industrial SenseCAP sensors.

<div align=center><img width = 800 src="https://media-cdn.seeedstudio.com/media/wysiwyg/__16582252472624.png"/></div>

## Statement

- The LoRa® Mark is a trademark of Semtech Corporation or its subsidiaries.
- LoRaWAN® is a mark used under license from the LoRa Alliance®.


<!--## About Grove - Wio-E5

Grove - Wio-E5 embedded with LoRa®-E5 STM32WLE5JC, powered by ARM Cortex M4 ultra-low-power MCU core and LoRa® SX126x, is an easy-to-use wireless radio module, supporting LoRaWAN® protocol on the EU868 & US915 frequency and (G)FSK, BPSK, (G)MSK, LoRa® modulations.

Grove - Wio-E5 can endow your development boards' strong features of ultra-long transmitting range, great performance, and high efficiency by easiliy plug and play with Grove connector on board. By connecting Grove - Wio-E5 to your development boards, your devices is able to communicate with and control Wio-E5 conveniently by AT command through UART connection. 

<div align=center><img width= 800 src="https://files.seeedstudio.com/wiki/K1100/wio-e5.png"/></div>

Please check out this article [LoRapedia](https://www.seeedstudio.com/blog/2020/08/03/lorapedia-an-introduction-of-lora-and-lorawan-technology/), which introduces LoRa and LoRaWAN to learn about LoRaWAN and LoRa technology in detail.

Wio-E5 series has a built-in AT command firmware, which supports LoRaWAN® Class A/B/C protocol and a wide frequency plan: EU868/US915/AU915/AS923/KR920/IN865. With this AT command firmware, users can easily and quickly build their prototype or application.

The AT command firmware contains a bootloader for DFU and the AT application. The "PB13/SPI_SCK/BOOT" pin is used to control Wio-E5 to stay in the bootloader or jump to the AT application. When PB13 is HIGH, the module will jump to AT application after reset, with a default baud rate of 9600. When PB13 is LOW (press the "Boot" button on Wio-E5 Dev Board or Wio-E5 mini), the module will stay in the bootloader, and keep transmitting "C" character every 1S at baud rate 115200.

- The specification of the Grove - Wio-E5 AT command can be found [here](https://files.seeedstudio.com/products/317990687/res/LoRa-E5%20AT%20Command%20Specification_V1.0%20.pdf).

- More information on the use of Grove - Wio-E5 can be found in this [wiki](https://wiki.seeedstudio.com/Grove_LoRa_E5_New_Version/).
-->

