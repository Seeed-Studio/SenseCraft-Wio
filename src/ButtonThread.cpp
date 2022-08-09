#include "ButtonThread.h"

ButtonThread::ButtonThread(Message &m)
    : Thread("ButtonThread", 256, 1),
      Mail(m)
{
    Start();
};

void ButtonThread::Run()
{
    LOGSS.print("Starting ButtonThread ");
    for (uint8_t i = 0; i < 8; i++)
    {
        pinMode(btn[i], INPUT_PULLUP);
    }
    uint8_t Message[8];
    while (true)
    {
        Delay(Ticks::MsToTicks(10));

        for (uint8_t i = 0; i < 8; i++)
        {
            if (digitalRead(btn[i]) == LOW)
            {
                Delay(Ticks::MsToTicks(100));
                if (digitalRead(btn[i]) == LOW)
                {
                    //avoid send 0 to Message               
                    Message[0] = i + 1;
                    Mail.Send(Message, strlen((const char *)Message));
                }
            }
        }
    }
}