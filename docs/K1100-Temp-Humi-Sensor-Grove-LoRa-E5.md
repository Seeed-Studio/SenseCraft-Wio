# Grove Temp&Humi Sensor (SHT40)

In this section, we will detail how the sensors work, how to get sensor data using Wio Terminal and how to send the data out using Wio Terminal & Grove - Wio-E5.

## Working Principle of Sensors

The SHT40 temperature is measured using a thermocouple method. A thermocouple consists of two types of metallic wires of different materials. One end of the two wires is welded together to form the working end, which is placed at the temperature to be measured. The other end is called the free end and is connected to the main control to form a closed loop. When the temperature of the working end and the free end is different, a thermoelectric potential will appear in the loop, and the change in this voltage will be sent to the SCM through conversion of the circuit and transformed into a signal that can be recognized by the machine.

The SHT40 humidity is measured using a polyamine or acetate polymer film (a highly fractionated compound) deposited on two conductive electrodes. When the film absorbs or loses water, it changes the dielectric constant between the two electrodes, which in turn causes a change in capacitor capacity. The change in capacitance can be captured and converted using external measurement circuitry, and eventually displayed as an easily identifiable signal on the output.

<td align="center"><div align=center><img width = 800 src="https://files.seeedstudio.com/wiki/Wio-Terminal-Developer-for-helium/165.png"/></div></td>

For more information on the use of Grove Temp&Humi Sensor its reference [here](https://wiki.seeedstudio.com/Grove-SHT4x/).

## Materials Required

<table align="center">
	<tr>
	    <td align="center"><div align=center><img width = 210 src="https://files.seeedstudio.com/wiki/Wio-Terminal/img/WT-GROVE.jpeg"/></div></td>
	    <td align="center"><div align=center><img width = 210 src="https://files.seeedstudio.com/products/113020091/%E5%AE%98%E7%BD%91/11302009_Preview-34.png"/></div></td>
        <td align="center"><div align=center><img width = 160 src="https://files.seeedstudio.com/wiki/Wio-Terminal-Developer-for-helium/SHT40.jpg"/></div></td>
	</tr>
	<tr>
	    <td align="center">Wio Terminal</td>
	    <td align="center">Grove - Wio-E5</td>
        <td align="center">Grove Temp&Humi Sensor (SHT40)</td>
	</tr>
</table>

## Preliminary Preparation

### Connection

In this routine, we need to connect to a nearby LoRa® gateway with the help of Grove - Wio-E5. We need to configure the Grove port on the right side of the Wio Terminal as a soft serial port to receive AT commands. Grove Temp&Humi Sensor (on the left side) according to the diagram below.

<td align="center"><div align=center><img width = 600 src="https://files.seeedstudio.com/wiki/K1100/SHT40.jpg"/></div></td>


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

**Step 5.** Download Grove - Wio-E5 Library

Visit the [Disk91_LoRaE5](https://github.com/limengdu/Disk91_LoRaE5) repositories and download the entire repo to your local drive.

<p style=":center"><a href="https://github.com/limengdu/Disk91_LoRaE5" target="_blank"><div align=center><img width = 300 src="https://files.seeedstudio.com/wiki/seeed_logo/github.png" /></div></a></p>

**Step 6.** Adding libraries to the Arduino IDE

Now, the 3-Axis Digital Accelerometer library can be installed to the Arduino IDE. Open the Arduino IDE, and click `sketch -> Include Library -> Add .ZIP Library`, and choose the `Disk91_LoRaE5` file that you've have just downloaded.

<div align=center><img width = 600 src="https://files.seeedstudio.com/wiki/Wio-Terminal/img/Xnip2019-11-21_15-50-13.jpg"/></div>

## Get the value of the Grove Temp&Humi Sensor (SHT40)

**Step 1.** Download Grove Temp&Humi Sensor Code Library

Visit the [arduino-i2c-sht4x](https://github.com/Sensirion/arduino-i2c-sht4x) repositories and download the entire repo to your local drive.

<p style=":center"><a href="https://github.com/Sensirion/arduino-i2c-sht4x" target="_blank"><div align=center><img width = 300 src="https://files.seeedstudio.com/wiki/seeed_logo/github.png" /></div></a></p>

Visit the [Sensirion Arduino Core Library](https://github.com/Sensirion/arduino-core) repositories and download the entire repo to your local drive.

<p style=":center"><a href="https://github.com/Sensirion/arduino-core" target="_blank"><div align=center><img width = 300 src="https://files.seeedstudio.com/wiki/seeed_logo/github.png" /></div></a></p>

**Step 2.** Adding libraries to the Arduino IDE

Now, the Grove Temp&Humi Sensor library can be installed to the Arduino IDE. Open the Arduino IDE, and click `sketch -> Include Library -> Add .ZIP Library`, and choose the `arduino-i2c-sht4x` file that you've have just downloaded.

<div align=center><img width = 600 src="https://files.seeedstudio.com/wiki/Wio-Terminal/img/Xnip2019-11-21_15-50-13.jpg"/></div>

**Step 3.** Obtain temperature and humidity data from SHT40

<p style=":center"><a href="https://github.com/limengdu/Seeed-Studio-LoRaWAN-Dev-Kit/tree/main/sensor/Get-SHT40-value" target="_blank"><div align=center><img width = 300 src="https://files.seeedstudio.com/wiki/seeed_logo/github.png" /></div></a></p>

This repo shows us the temperature and humidity data obtained. In the code, we use the `measureHighPrecision()` function to get the temperature and humidity information, which is positive and float.

```c++
#include <Arduino.h>
#include <SensirionI2CSht4x.h>
#include <Wire.h>

SensirionI2CSht4x sht4x;

void setup() {

    Serial.begin(115200);
    while (!Serial) {
        delay(100);
    }

    Wire.begin();

    uint16_t error;
    char errorMessage[256];

    sht4x.begin(Wire);

    uint32_t serialNumber;
    error = sht4x.serialNumber(serialNumber);
    if (error) {
        Serial.print("Error trying to execute serialNumber(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    } else {
        Serial.print("Serial Number: ");
        Serial.println(serialNumber);
    }
}

void loop() {
    uint16_t error;
    char errorMessage[256];

    delay(1000);

    float temperature;
    float humidity;
    error = sht4x.measureHighPrecision(temperature, humidity);
    if (error) {
        Serial.print("Error trying to execute measureHighPrecision(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    } else {
        Serial.print("Temperature:");
        Serial.print(temperature);
        Serial.print("\t");
        Serial.print("Humidity:");
        Serial.println(humidity);
    }
}
```

Open the serial monitor of Arduino IDE and select the baud rate as 115200 and observe the result.

<td align="center"><div align=center><img width = 700 src="https://files.seeedstudio.com/wiki/Wio-Terminal-Developer-for-helium/166.png"/></div></td>

## Send data via Grove - Wio-E5

We combine the previous code of Grove - Wio-E5 to connect to the LoRa® network. Using the AT command it is possible to send the value of the Grove Temp&Humi Sensor to the LoRa® network.

In the code in the previous section, we know that the temperature and humidity values are generally a combination of two integers and two decimals, and all are positive, float.

Due to the limitations of sending data, we need to solve the problem of converting float numbers to integers on the sending side to ensure that the data sent is an integer, so we multiply all temperature and humidity values by 100.

In this way, we determine the content, size and format of the data to be sent via the AT command. We might as well set up a large enough array, store the strings we need to send into the array, and finally use the **send_sync()** function to send the array out.

The pseudo-code for the above idea is roughly as follows.

```c
  ......
  error = sht4x.measureHighPrecision(temperature, humidity);

  int_temp = temperature*100;
  int_humi = humidity*100;
  
  static uint8_t data[4] = { 0x00 };  //Use the data[] to store the values of the sensors

  data_decord(int_temp, int_humi, data);
  
  if ( lorae5.send_sync(              //Sending the sensor values out
        8,                            // LoRaWan Port
        data,                         // data array
        sizeof(data),                 // size of the data
        false,                        // we are not expecting a ack
        7,                            // Spread Factor
        14                            // Tx Power in dBm
       ) 
  )
  ......
```

The rest of what we need to do is to use the `begin()` function to initialize Grove - Wio-E5 and the `setup()` function to configure the triplet information of Grove - Wio-E5. When we send a data message using the `send_sync()` function, we will try to join the LoRaWAN® at the same time, and once it succeeds, the data will be sent and information such as signal strength and address will be returned.

The full code example can be found [here](https://github.com/limengdu/Seeed-Studio-LoRaWAN-Dev-Kit/tree/main/sensor/SHT40-send-data).

<p style=":center"><a href="https://github.com/limengdu/Seeed-Studio-LoRaWAN-Dev-Kit/tree/main/sensor/SHT40-send-data" target="_blank"><div align=center><img width = 300 src="https://files.seeedstudio.com/wiki/seeed_logo/github.png" /></div></a></p>

!!!Tip
    We do not recommend that you upload the code now to see the results, because at this point you have not yet configured Helium/TTN and will get a "Join failed" result. We recommend that you upload this code after you have completed the [Connecting to Helium](https://wiki.seeedstudio.com/Connecting-to-Helium/) or [Connecting to TTN](https://wiki.seeedstudio.com/Connecting-to-TTN/) chapter to complete the complete data sending process.

Once you have experienced and understood how the Grove Temp&Humi Sensor works and the data format, please continue with the next step of the tutorial join LoRaWAN®.

<table align="center">
	<tr>
		<td colspan="2" bgcolor="8FBC8F"><b>Helium Section</b></td>
	</tr>
	<tr>
	    <td align="center"><p style="text-align:center;"><img src="https://files.seeedstudio.com/wiki/Wio-Terminal-Developer-for-helium/178.jpeg" alt="pir" width="500" height="auto"></p></td>
	    <td align="left"><strong>Helium Introduction</strong></br></br>In this chapter, we will introduce the Helium console controls that we use to get a first impression of the Helium console.</br></br><a href="https://wiki.seeedstudio.com/Helium-Introduction">Jump to chapter ></a></td>
	</tr>
	<tr>
	    <td align="center"><p style="text-align:center;"><img src="https://files.seeedstudio.com/wiki/Wio-Terminal-Developer-for-helium/helium.png" alt="pir" width="500" height="auto"></p></td>
	    <td align="left"><strong>Connecting to Helium</strong></br></br>This section describes how to configure Helium so that sensor data can be successfully uploaded to and displayed in Helium.</br></br><a href="https://wiki.seeedstudio.com/Connecting-to-Helium">Jump to chapter ></a></td>
	</tr>
	<tr>
		<td colspan="2" bgcolor="8FBC8F"><b>TTN Section</b></td>
	</tr>
	<tr>
	    <td align="center"><p style="text-align:center;"><img src="https://files.seeedstudio.com/wiki/Wio-Terminal-Developer-for-TTN/TTN.png" alt="pir" width="500" height="auto"></p></td>
	    <td align="left"><strong>TTN Introduction</strong></br></br>In this chapter, we will introduce the TTN console controls that we use to get a first impression of the TTN console.
		</br></br><a href="https://wiki.seeedstudio.com/TTN-Introduction">Jump to chapter ></a></td>
	</tr>
	<tr>
	    <td align="center"><p style="text-align:center;"><img src="https://files.seeedstudio.com/wiki/K1100/ttncon.jpg" alt="pir" width="500" height="auto"></p></td>
	    <td align="left"><strong>Connecting to TTN</strong></br></br>This section describes how to configure TTN so that sensor data can be successfully uploaded to and displayed in TTN.
		</br></br><a href="https://wiki.seeedstudio.com/Connecting-to-TTN">Jump to chapter ></a></td>
	</tr>
</table>

## Tech Support

Please do not hesitate to submit the issue into our [forum](https://forum.seeedstudio.com/).


> **Upgradable to Industrial Sensors**
SenseCAP K1100 serves as a gateway to the higher-end SenseCAP LoRaWAN® series of industrial outdoor sensors. Users can prototype multiple AIoT applications with this kit, after which they can scale them up to the robust, long-life, and industrial SenseCAP sensors.

<div align=center><img width = 800 src="https://media-cdn.seeedstudio.com/media/wysiwyg/__16582252472624.png"/></div>

## Statement

- The LoRa® Mark is a trademark of Semtech Corporation or its subsidiaries.
- LoRaWAN® is a mark used under license from the LoRa Alliance®.

