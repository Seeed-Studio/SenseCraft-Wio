// #include <SPI.h>
// #include <Seeed_FS.h>
// #include "SD/Seeed_SD.h"
// #include"seeed_line_chart.h" //include the library

// File myFile;
// TFT_eSPI tft;
// TFT_eSprite spr = TFT_eSprite(&tft);  // Sprite 

// #define max_size 30 //maximum size of data
// doubles data[3]; //Initilising a doubles type to store data


// int vibration;
// int dataCount;
// int keyPressed = 0;
// int keyUpdate = 0;
// int sampleNumber = 0;
// int typeIndex = 0;
// String sampleType[] = {"Idle","Noise","Unknown","Person1","Person2"};
// String fileName;
// int fileHeader = 0;


// // define logging interval and sample size
// #define FREQUENCY_HZ        500
// #define INTERVAL_MS         (1000 / (FREQUENCY_HZ + 1))
// #define SAMPLE_SZ           2500   // 500 * 5 seconds 
// int sampleData[SAMPLE_SZ];
// unsigned long timeData[SAMPLE_SZ];

// void setup() {
//     Serial.begin(115200);
//     if (!SD.begin(SDCARD_SS_PIN, SDCARD_SPI)) {
//         Serial.println("initialization failed!");
//         while(1);
//     }
//     pinMode(A0, INPUT);
//     pinMode(WIO_KEY_A, INPUT_PULLUP);  // rightmost button
//     pinMode(WIO_KEY_B, INPUT_PULLUP);  // center button
//     pinMode(WIO_KEY_C, INPUT_PULLUP);  // leftmost button
//     pinMode(WIO_5S_PRESS, INPUT_PULLUP);  // 5 way button
    
//     tft.begin();
//     tft.setRotation(3);
//     spr.createSprite(TFT_HEIGHT,TFT_WIDTH);
//     tft.setTextSize(2);
    
//     dataCount = 0;


// }

// void loop() {
//     static unsigned long last_interval_ms = 0;


// //    if (millis() > last_interval_ms + INTERVAL_MS) {
// //      last_interval_ms = millis();


//       spr.fillSprite(TFT_WHITE);
//       vibration = analogRead(A0);
//       Serial.print("600,");
//       Serial.print(vibration);
//       Serial.println(",400");
   
//       if (data[0].size() == max_size) {
//         for (uint8_t i = 0; i<3; i++){
//           data[i].pop(); //this is used to remove the first read variable
//         }
//         //data[0].pop();//this is used to remove the first read variable
//       }
//       data[0].push(vibration); //read variables and store in data
//       data[1].push(530);  //top
//       data[2].push(480);  //bottom


// //      if (dataCount < SAMPLE_SZ){
// //        saveData();
// //        dataCount++;
// //      }
   
//       //Settings for the line graph title
//       auto header =  text(0, 0)
//                   .value(" Vibration Readings")
//                   .align(center)
//                   .valign(vcenter)
//                   .width(tft.width())
//                   .thickness(2);
   
//       header.height(header.font_height() * 2);
//       header.draw(); //Header height is the twice the height of the font
   
//     //Settings for the line graph
//       auto content = line_chart(20, header.height()); //(x,y) where the line graph begins
//            content
//                   .height(tft.height() - header.height() * 1.5) //actual height of the line chart
//                   .width(tft.width() - content.x() * 2) //actual width of the line chart
//                   .based_on(450.0) //Starting point of y-axis, must be a float
//                   .show_circle(false) //drawing a cirle at each point, default is on.
//                   .value({data[1],data[0],data[2]}) //passing through the data to line graph
//                   .color(TFT_WHITE,TFT_RED,TFT_WHITE) //Setting the color for the line
//                   .draw();
   
//       spr.pushSprite(0, 0);


//       checkpress();  // check for button press
      
// //    }
// }

// void saveData(String fName,unsigned long tData, int sData){
// //    fileName = sampleType[typeIndex];
// //    fileName.concat(sampleNumber);
// //    fileName.concat(".csv");  
// //    tft.drawString(fileName,50,130);
//     myFile = SD.open(fName,FILE_APPEND);
// //    vibration = analogRead(A0);
// //    Serial.println(vibration);
//     if (fileHeader == 0) {
//       myFile.println("timestamp,VSBV203");  // print file header if first pass
//       fileHeader = 1;
//     }
//     myFile.print(tData);
//     myFile.print(",");
//     myFile.println(sData);
//     myFile.close();
// }


// void captureData(unsigned long timeStamp, int count){
//   vibration = analogRead(A0);
//   timeData[count] = timeStamp;
//   sampleData[count] = vibration;
// }


// // Check state of buttons
// void checkpress(){
//   char buffer[20];
//   static unsigned long last_interval_ms = 0;


//   if (digitalRead(WIO_KEY_A) == LOW) {
//     if (keyPressed == 3) {
//       keyUpdate = 0;
//     } else {
//       keyUpdate = 1;
//       keyPressed = 3;
//     }
//   }
//   else if (digitalRead(WIO_KEY_B) == LOW) {
//     if (keyPressed == 2) {
//       keyUpdate = 0;
//     } else {
//       keyUpdate = 1;
//       keyPressed = 2;
//     }
//   }
//   else if (digitalRead(WIO_KEY_C) == LOW) {
//     if (keyPressed == 1) {
//       keyUpdate = 0;
//     } else {
//       keyUpdate = 1;
//       keyPressed = 1;
//     }
//   }
//   else if (digitalRead(WIO_5S_PRESS) == LOW) {
//       keyPressed = 0;
//   }

//   delay(200);


//   if (keyUpdate) {
   
//     switch (keyPressed) {
//       case 1:
//         tft.fillScreen(TFT_BLACK);
//         tft.drawString("Start Sampling",25,90);
// //        tft.drawString(fileName,50,130);
//         while (dataCount < SAMPLE_SZ){
//           if (millis() > last_interval_ms + INTERVAL_MS) {
//             last_interval_ms = millis();  
//             captureData(last_interval_ms,dataCount);
//             dataCount++;
//           }
// //          if (dataCount >= SAMPLE_SZ) break;
//         }   
//         fileName = sampleType[typeIndex];
//         fileName.concat("_");
//         fileName.concat(sampleNumber);
//         fileName.concat(".csv");  
//         tft.drawString(fileName,50,130);
//         for (int i = 0; i < SAMPLE_SZ; i++){
//           saveData(fileName,timeData[i],sampleData[i]);
//         }
//         dataCount = 0;     
//         break;
//       case 2:
//         sampleNumber++;
//         Serial.println(sampleNumber);
//         tft.fillScreen(TFT_BLACK);
//         tft.drawString("Sample Number: ",25,110);
//         tft.drawString(String(sampleNumber),200,110);
//         break;
//       case 3:
//         if (typeIndex++ > 3) typeIndex = 0;
//         Serial.println(sampleType[typeIndex]);
//         tft.fillScreen(TFT_BLACK);
//         tft.drawString("Sample Type: ",25,110);
//         tft.drawString(sampleType[typeIndex],200,110);
//         break;      
//     }
    
//     keyUpdate = 0;

//   delay(2000);    
  
//   }
    
// }