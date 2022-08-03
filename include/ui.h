#include <stdint.h>
#include <TFT_eSPI.h>


#define SCREEN_WIDTH 320                        // Wio Terminal Maximum Width
#define SCREEN_HIGH 240                         // Wio Terminal Maximum Height

/*build a ui class with pages*/
class UI
{
public:
    UI();
    ~UI();
    void init();

private:
    TFT_eSPI tft;
    TFT_eSprite spr = TFT_eSprite(&tft);
};
