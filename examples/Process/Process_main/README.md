# Process part

The page is implemented as follows according to the modao documentation requirements. Functions are decoupled and can be called independently of each other. As a test ino, burn under example/Process_main and click A to check the network status display in the lower left corner. Click B to view the interfaces in Process. In the Ai vision Log part, due to the limited ability of unfamiliar with ardiuno, the queue of uint8_t and int did not construct the time data currently displayed.

Process_Display();  //
Process_TinyML_ENTER(); //
Vision_AI_real_time_analysis(); //
Network_state(int s_key); // Change network status 1 display ON 0 display OFF can be imported into Process_Display or anywhere

| Code | Content | Status|
| :------------ |:---------------:| -----:|
|Vision AI log| Vision_AI_real_time_analysis() |✅ |
|Tiny ml has entered| Process_TinyML_ENTER() |✅✅ |
|Network| Network_state(int s_key)|✅✅|
|Proccsee Home| Network_state(int s_key)|✅✅|

## Process_Display()

draw top three buttons background color default bottom layer

## Process_main(int page)

!!!Note
    Add page 0 here; page 3 For the convenience of testing, this function can be called, or it can be called independently

- case 0   Vision AI log
- case 1  Proccsee Home
- case 2  Tiny ml example
- case 3  Tiny ml has entered

## Network_state(int s_key)

The network status display function Network in the lower left corner. 1 ON 0 OFF

## Vision_AI_real_time_analysis(void)

 Vision AI log 

## Process_TinyML_ENTER()

This is the interface that TinyML has entered in order to present the QR code.
https://www.arduino.cc/reference/en/libraries/qrcode/ this [library](https://www.arduino.cc/reference/en/libraries/qrcode/)

