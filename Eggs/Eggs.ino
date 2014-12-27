#include "Config.h"
#include <Servo.h>

typedef enum {
    OPEN_START = 0,
    OPEN_END = OPEN_START + 1,
    VOICE_START = OPEN_START + 2,
    VOICE_END = OPEN_START + 3,
    CLOSE_START = OPEN_START + 4,
    CLOSE_END = OPEN_START + 5
} E_internalEggStatus;

static unsigned long currTime = 0;
static unsigned long prevTime = 0;
static unsigned long prevDetectTime = 0;
E_internalEggStatus eggStatus = CLOSE_START;

void setup()
{
    currTime = millis();
    // init SD card and read info from that.
#ifdef LED_BLINK
    ledSetup(currTime, 1000, 1000);
#endif // LED_BLINK
#ifdef SERVO_ROTATE
    servoLoop(currTime, 1000，1000);
#endif // SERVO_ROTATE
}

void loop()
{
    currTime = millis();
    if (currTime - prevTime > FRAME_MS)
    {
        // read from light sensor
        // read from control board
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
