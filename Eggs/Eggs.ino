#include <Ultrasonic.h>
#include <SD.h>

#include "Config.h"

/* create ultrasonic instance */
Ultrasonic ultrasonic(ULTRASONIC_TRIGGER_PIN, ULTRASONIC_ECHO_PIN);

static unsigned long currTime = 0;
static unsigned long prevTime = 0;
static unsigned long prevDetectTime = 0;
static unsigned long openEndTime = 0;
static unsigned long eggRaisePeriod = 10 * 1000;
static unsigned long voiceWaitPeriod = 1 * 1000;
int intEggStatus = CLOSE_END;
int extEggStatus = 0;
int closeDist = 30;
float cmMesc = 0, inMesc = 0;
long microSec = 0;
bool detectOpenEnd = false;
File voice;

void setup()
{
    String key = "", value = "", voiceFileName;
    bool getEqual = false, endOfLine;
    char ch;
    int ledDarkInterval = 1000;
    int ledLightInterval = 1000;
    int servoRaiseInterval = 1000;
    int servoDownInterval = 1000;

    currTime = millis();
    // init SD card and read info from that.
    Serial.begin(9600);
    /* init SD card */
    pinMode(SD_CS_PIN, OUTPUT);
    if (!SD.begin(SD_SDCS_PIN)) {
        Serial.println("initialization failed!");
        return;
    }
    voice = SD.open("/config.ini");
    /* file exists */
    if (voice)
    {
        while (voice.available())
        {
            ch = voice.read();
            if (ch == '\n')
            {
                getEqual = false;
                endOfLine = true;
            }
            else if (ch == '=')
            {
                getEqual = true;
                continue;
            }

            if (endOfLine) 
            {
                if (key.equals("eggRaisePeriod"))
                {
                    eggRaisePeriod = value.toInt();
                }
                else if (key.equals("voiceWaitPeriod"))
                {
                    voiceWaitPeriod = value.toInt();
                }
                else if (key.equals("ledDarkInterval"))
                {
                    ledDarkInterval = value.toInt();
                }
                else if (key.equals("ledLightInterval"))
                {
                    ledLightInterval = value.toInt();
                }
                else if (key.equals("servoRaiseInterval"))
                {
                    servoRaiseInterval = value.toInt();
                }
                else if (key.equals("servoDownInterval"))
                {
                    servoDownInterval = value.toInt();
                }
                else if (key.equals("closeDist"))
                {
                    closeDist = value.toInt();
                }
                else if (key.equals("voiceFileName"))
                {
                    voiceFileName = String(value);
                }
                endOfLine = false;
                key = "";
                value = "";
                continue;
            }

            if (getEqual)
            {
                value = String(value + ch);
            }
            else
            {
                key = String(key + ch);
            }
        }
        voice.close();
    }

#ifdef LED_BLINK
    ledSetup(currTime, ledLightInterval, ledDarkInterval);
#endif // LED_BLINK
#ifdef SERVO_ROTATE
    servoSetup(currTime, servoRaiseInterval, servoDownInterval);
#endif // SERVO_ROTATE
    // TODO: open voice file.
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
            if (cmMesc < closeDist)
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
        setEggStatus(currTime, 0);

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
    else if (raiseOrDown == 0 && (intEggStatus == VOICE_START || intEggStatus == VOICE_END) 
            && (currTime - prevDetectTime > eggRaisePeriod))
    {
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

