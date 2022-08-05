#if 0
#include "Seeed_Arduino_ooFreeRTOS.h"
#include <Arduino.h>
#include <SPI.h>
#include "ui.h"
#include "ButtonThread.h"
using namespace cpp_freertos;

TFT_eSPI tft;
TFT_eSprite spr = TFT_eSprite(&tft); 


void display_init()            // Display initialization, black background rotation
{
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);
}



void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);
    uint32_t start = millis();
    while (!Serial && (millis() - start) < 1500)
        ; // Open the Serial Monitor to get started or wait for 1.5"

    display_init();

    Message *Mail;
    Mail = new Message(256);
    ButtonThread *btn = new ButtonThread(*Mail);
    UI *u = new UI(tft,spr, *Mail);
    Thread::StartScheduler();
}

void loop()
{
}
#endif
#if 1
#include <Arduino.h>
#include <SPI.h>
#define RADIUS 40      // Radius of completed symbol = 40

#define WAIT 0         // Loop delay

// 1bpp Sprites are economical on memory but slower to render
#define COLOR_DEPTH 1  // Colour depth (1, 8 or 16 bits per pixel)

// Rotation angle increment and start angle
#define ANGLE_INC 3
int angle = 0;

#include <TFT_eSPI.h>                 // Hardware-specific library

TFT_eSPI    tft = TFT_eSPI();         // Invoke library

TFT_eSprite img = TFT_eSprite(&tft);  // Sprite class

// =========================================================================
// Get coordinates of end of a vector, pivot at x,y, length r, angle a
// =========================================================================
// Coordinates are returned to caller via the xp and yp pointers
#define RAD2DEG 0.0174532925
void getCoord(int x, int y, int* xp, int* yp, int r, int a) {
    float sx1 = cos((a - 90) * RAD2DEG);
    float sy1 = sin((a - 90) * RAD2DEG);
    *xp =  sx1 * r + x;
    *yp =  sy1 * r + y;
}
#endif 

// -------------------------------------------------------------------------

// =========================================================================
// Draw circles for Yin and Yang - rotate positions to create symbol
// =========================================================================
// x,y == coordinate center within Sprite
// start_angle = 0 - 359
// r = radius

void yinyang(int x, int y, int start_angle, int r) {
    int x1 = 0; // getCoord() will update these
    int y1 = 0;

    getCoord(x, y, &x1, &y1, r / 2, start_angle); // Get x1 ,y1
    img.fillCircle(x1,  y1, r / 2, TFT_WHITE);
    img.fillCircle(x1,  y1, r / 8, TFT_BLACK);

    getCoord(x, y, &x1, &y1, r / 2, start_angle + 180);
    img.fillCircle(x1,  y1, r / 2, TFT_BLACK);
    img.fillCircle(x1,  y1, r / 8, TFT_WHITE);

    img.drawCircle(x, y, r, TFT_WHITE);
}





// -------------------------------------------------------------------------
void setup(void) {
    tft.begin();
    tft.setRotation(0);
    tft.fillScreen(TFT_BLUE);

    img.setColorDepth(COLOR_DEPTH);
    img.createSprite(RADIUS * 2 + 1, RADIUS * 2 + 1);
    img.fillSprite(TFT_BLACK);
}
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
void loop() {
    // Draw Yin and Yang symbol circles into Sprite
    yinyang(RADIUS, RADIUS, angle, RADIUS);

    // Set the 2 pixel palette colours that 1 and 0 represent on the display screen
    img.setBitmapColor(TFT_WHITE, TFT_BLACK);

    // Push Sprite image to the TFT screen at x, y
    img.pushSprite(tft.width() / 2 - RADIUS, 0); // Plot sprite

    angle += 3;                 //Increment angle to rotate circle positions
    if (angle > 359) {
        angle = 0;    // Limit angle range
    }

    // Slow things down
    delay(WAIT);
}
