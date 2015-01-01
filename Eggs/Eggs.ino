#include "Config.h"


static unsigned long currTime = 0;
static unsigned long prevTime = 0;
static unsigned long prevDetectTime = 0;
int intEggStatus = CLOSE_START;
int extEggStatus = 0;

void setup()
{
    currTime = millis();
    // init SD card and read info from that.
    pinMode(6, INPUT);
    Serial.begin(9600);
#ifdef LED_BLINK
    ledSetup(currTime, 1000, 1000);
#endif // LED_BLINK
#ifdef SERVO_ROTATE
    servoSetup(currTime, 2000, 1000);
#endif // SERVO_ROTATE
}

void loop()
{
    currTime = millis();
    int btnStatus;
    if (currTime - prevTime > FRAME_MS)
    {
        // read from light sensor
        btnStatus = digitalRead(6);
        // read from control board
        if (((extEggStatus & MODE) == 0) && (currTime - prevDetectTime > IDLE_INTERVAL))
        {
            extEggStatus |= MODE;
        }
        if (btnStatus)
        {
            if (intEggStatus == CLOSE_END)
            {
                intEggStatus = OPEN_START;
            }
        }
        else
        {
            if (intEggStatus == OPEN_END)
            {
                intEggStatus = CLOSE_START;
            }
        }

#ifdef LED_BLINK
            ledLoop(currTime);
#endif // LED_BLINK
#ifdef SERVO_ROTATE
            servoLoop(currTime);
#endif // SERVO_ROTATE
        prevTime = currTime;
    }
    else
    {
        delay(0);
    }
}
