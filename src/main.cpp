
#include "ButtonThread.h"
#include "FreeRTOS.h"
#include "LoRaThread.h"
#include "SamplerThread.h"
#include "Seeed_Arduino_ooFreeRTOS.h"
#include "SysConfig.h"
#include "ui.h"
#include <Arduino.h>
#include <SPI.h>
#ifdef CM_DEBUG
#include "cm_backtrace.h"
#endif
#define HARDWARE_VERSION "seeed_k1100_dev_kit"
#define SOFTWARE_VERSION "V1.0.0"

#define ANSI_R "\x1b[31m"
#define ANSI_G "\x1b[32m"
#define ANSI_Y "\x1b[33m"
#define ANSI_B "\x1b[34m"
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_CYAN    "\x1b[36m"
#define ANSI_RESET   "\x1b[0m"

using namespace cpp_freertos;

TFT_eSPI    tft;
// TFT_eSprite spr = TFT_eSprite(&tft);

void display_init() // Display initialization, black background rotation
{
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setFreeFont(FSSB9);
    tft.drawString(DEVICE, 90, 110, GFXFF);
    tft.setFreeFont(FSS9);
    tft.drawString(VERSION, 150, 150, FONT2);
}

using namespace std;
void LogMemoryUsage(const char *s);
void LogHeapChange(const char *s);
void LogTaskTrace(void);

void setup() {
    LOGSS.begin(115200);
    display_init();
    SysConfig *cfg = new SysConfig();
    cfg->init();

#ifdef CM_DEBUG
    cm_backtrace_init("Seeed K1100 dev kit", HARDWARE_VERSION, SOFTWARE_VERSION);
#endif
    // put your setup code here, to run once:
    uint32_t start = millis();  
    while (!LOGSS && (millis() - start) < 1500)
        ; // Open the Serial Monitor to get started or wait for 1.5"


    Message *btnMail = new Message(256);
    // Message *sensorMail = new Message(256);

    ButtonThread  *btn     = new ButtonThread(*btnMail);
    UI            *u       = new UI(tft, *cfg, *btnMail);
    SamplerThread *sampler = new SamplerThread(*cfg, *u);
}

// Get the size of memory left in the system in freertos.
void LogMemoryUsage(const char *s) {
    int CurFree, MinFree, Percentage;
    int Total = configTOTAL_HEAP_SIZE;
    CurFree = xPortGetFreeHeapSize();
    MinFree = xPortGetMinimumEverFreeHeapSize();
    Percentage = (Total-CurFree)*100 / Total;
    string UsageGraph = "["ANSI_G;
    for(int i=0; i<25; i++)
    {
        if(i == Percentage/4)
            UsageGraph += "#"ANSI_RESET;
        else
            UsageGraph += "#";
    }
    UsageGraph += "]";
    LOGSS.println("====== RTOS HEAP USAGE =======");
    LOGSS.printf("| Log in  : "ANSI_Y"%s \r\n"ANSI_RESET, s);
    LOGSS.printf("| Total   : "ANSI_B"%d "ANSI_RESET"bytes\r\n", Total);
    LOGSS.printf("| Maximum : "ANSI_R"%d "ANSI_RESET"bytes\r\n", Total-MinFree);
    LOGSS.printf("| Current : "ANSI_G"%d "ANSI_RESET"bytes\r\n", (Total-CurFree), UsageGraph.c_str());
    LOGSS.printf("| %s \r\n", UsageGraph.c_str());
}

void LogTaskTrace() {
    char TraceBuf[512];
    vTaskList(TraceBuf);
    LOGSS.println("========= RTOS task stack usage ==========");
    LOGSS.println("TaskName    State  Priority  FreeStack");
    LOGSS.printf("%s", TraceBuf);
    LOGSS.println("================== end ===================");
}

void LogHeapChange(const char *s) {
    static int pre_free_heap = 0;
    int heap_change = configTOTAL_HEAP_SIZE - xPortGetFreeHeapSize() - pre_free_heap;
    if(heap_change > 0) {
        LOGSS.printf(ANSI_R" => %s +%d\r\n"ANSI_RESET, s, heap_change);
    }
    else if(heap_change < 0) {
        LOGSS.printf(ANSI_G" => %s %d\r\n"ANSI_RESET, s, heap_change);
    }
    pre_free_heap += heap_change;
}

void loop() {
    // LOGSS.printf("Main Stacks Free Bytes Remaining %d\r\n", uxTaskGetStackHighWaterMark(NULL));
    vTaskDelete(NULL);
}
