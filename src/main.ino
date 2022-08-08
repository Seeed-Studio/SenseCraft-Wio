/*********************************************************************
 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 Copyright (c) 2019 Ha Thach for Adafruit Industries
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

#include <rpcWiFi.h>
#include <Adafruit_TinyUSB.h>
#include <Seeed_Arduino_FS.h>
#include <Seeed_Arduino_ooFreeRTOS.h>
#include <Seeed_SFUD.h>
#include <sfud.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include <system.h>

using namespace cpp_freertos;

class WiFiThread : public Thread
{
public:
  WiFiThread();

protected:
  virtual void Run();
};

WiFiThread::WiFiThread() : Thread("WiFiThread", 1024, 1)
{
  //Start();
}

void WiFiThread::Run()
{
  while (true)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.printf("WiFi Connecting...\n");
      WiFi.begin(sys.SSID().c_str(), sys.PWD().c_str());
      for (int i = 0; i < 3; i++)
      {
        if (WiFi.status() == WL_CONNECTED)
          break;
      }
      if (WiFi.status() != WL_CONNECTED)
      {
        Serial.printf("WiFi Connect failed...\n");
      }
      else
      {
        Serial.printf("WiFi Connected...\n");
      }
    }
    Delay(Ticks::MsToTicks(100));
  }
}

TFT_eSPI tft;

TFT_eSprite img = TFT_eSprite(&tft); // Sprite class

const sfud_flash *flash = NULL;

#define RADIUS 40 // Radius of completed symbol = 40

#define WAIT 0 // Loop delay

// 1bpp Sprites are economical on memory but slower to render
#define COLOR_DEPTH 1 // Colour depth (1, 8 or 16 bits per pixel)

// Rotation angle increment and start angle
#define ANGLE_INC 3
int angle = 0;

// the setup function runs once when you press reset or power the board
void setup()
{

  sys.begin();

  static WiFiThread w;

  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLUE);

  img.setColorDepth(COLOR_DEPTH);
  img.createSprite(RADIUS * 2 + 1, RADIUS * 2 + 1);
  img.fillSprite(TFT_BLACK);

  w.Start();
}

void loop()
{
  Serial.printf("ssid: %s\n", sys.SSID().c_str());
  Serial.printf("pwd: %s\n", sys.PWD().c_str());
  Serial.printf("sd: %d\n", sys.TF());
  if (0 != sys.write(sys.SSID()))
  {
    Serial.printf("write successed\n", sys.TF());
  }
  else
  {
    Serial.printf("write failed\n", sys.TF());
  }
  // Draw Yin and Yang symbol circles into Sprite
  yinyang(RADIUS, RADIUS, angle, RADIUS);

  // Set the 2 pixel palette colours that 1 and 0 represent on the display screen
  img.setBitmapColor(TFT_WHITE, TFT_BLACK);

  // Push Sprite image to the TFT screen at x, y
  img.pushSprite(tft.width() / 2 - RADIUS, 0); // Plot sprite

  angle += 3; // Increment angle to rotate circle positions
  if (angle > 359)
  {
    angle = 0; // Limit angle range
  }
  // Serial.println(sys.SSID());
  // // Slow things down
  delay(1000);
}

// -------------------------------------------------------------------------

// =========================================================================
// Draw circles for Yin and Yang - rotate positions to create symbol
// =========================================================================
// x,y == coordinate center within Sprite
// start_angle = 0 - 359
// r = radius

void yinyang(int x, int y, int start_angle, int r)
{
  int x1 = 0; // getCoord() will update these
  int y1 = 0;

  getCoord(x, y, &x1, &y1, r / 2, start_angle); // Get x1 ,y1
  img.fillCircle(x1, y1, r / 2, TFT_WHITE);
  img.fillCircle(x1, y1, r / 8, TFT_BLACK);

  getCoord(x, y, &x1, &y1, r / 2, start_angle + 180);
  img.fillCircle(x1, y1, r / 2, TFT_BLACK);
  img.fillCircle(x1, y1, r / 8, TFT_WHITE);

  img.drawCircle(x, y, r, TFT_WHITE);
}

// =========================================================================
// Get coordinates of end of a vector, pivot at x,y, length r, angle a
// =========================================================================
// Coordinates are returned to caller via the xp and yp pointers
#define RAD2DEG 0.0174532925
void getCoord(int x, int y, int *xp, int *yp, int r, int a)
{
  float sx1 = cos((a - 90) * RAD2DEG);
  float sy1 = sin((a - 90) * RAD2DEG);
  *xp = sx1 * r + x;
  *yp = sy1 * r + y;
}
