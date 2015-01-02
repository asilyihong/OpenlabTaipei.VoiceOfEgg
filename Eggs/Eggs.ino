#include <Ultrasonic.h>
#include "Config.h"

Ultrasonic ultrasonic(ULTRASONIC_TRIGGER_PIN, ULTRASONIC_ECHO_PIN);

static unsigned long currTime = 0;
static unsigned long prevTime = 0;
static unsigned long prevDetectTime = 0;
static unsigned long openEndTime = 0;
static unsigned long eggRaisePeriod = 10 * 1000;
static unsigned long voiceWaitPeriod = 1 * 1000;
int intEggStatus = CLOSE_END;
int extEggStatus = 0;
float cmMesc = 0, inMesc = 0;
long microSec = 0;
bool detectOpenEnd = false;

void setup()
{
    currTime = millis();
    // init SD card and read info from that.
    Serial.begin(9600);
#ifdef LED_BLINK
    ledSetup(currTime, 1000, 1000);
#endif // LED_BLINK
#ifdef SERVO_ROTATE
    servoSetup(currTime, 1000, 3000);
#endif // SERVO_ROTATE
}

void loop()
{
    currTime = millis();
    if (currTime - prevTime > FRAME_MS)
    {
        // read from control board
        if (((extEggStatus & MODE) != 0) || intEggStatus == CLOSE_END)
        {
            // read from ultra sonic
            microSec = ultrasonic.timing();
            cmMesc = ultrasonic.convert(microSec, Ultrasonic::CM);
            if (cmMesc < CLOSE_DIST)
            {
                prevDetectTime = currTime;
                switch (intEggStatus)
                {
                    case OPEN_START:
                    case OPEN_END:
                    case VOICE_START:
                        break;
                    case VOICE_END:
                    case CLOSE_START:
                    case CLOSE_END:
                    default:
                        setEggStatus(currTime, 1);
                        break;
                }
            }
        }

        /* Change to manual mode */
        if (((extEggStatus & MODE) == 0) && (currTime - prevDetectTime > IDLE_INTERVAL))
        {
            extEggStatus |= MODE;
        }
        if (((extEggStatus & MODE) == 0) && (currTime - prevDetectTime > eggRaisePeriod)) /* Auto close in detect mode */
        {
            setEggStatus(currTime, 0);
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

/**
 * raiseOrDown: 0 for down, 1 for raise
 */
void setEggStatus(unsigned long currT, int raiseOrDown)
{
    if (intEggStatus == OPEN_END && currT - openEndTime > voiceWaitPeriod)
    {
        intEggStatus = VOICE_START;    
        // TODO: play sound
    }
    else if (raiseOrDown == 0 && (intEggStatus == VOICE_START || intEggStatus == VOICE_END))
    {
        Serial.println("SET EGG DOWN");
        intEggStatus = CLOSE_START;    
	extEggStatus &= ~STATUS;
        // TODO: emit close event.
    }
    else if (raiseOrDown == 1 && (intEggStatus == CLOSE_START || intEggStatus == CLOSE_END))
    {
        intEggStatus = OPEN_START;    
	extEggStatus |= STATUS;
        // TODO: emit open event.
    }
}

