#include <DFPlayer_Mini_Mp3.h>
#include <SoftwareSerial.h>
//#include <Ultrasonic.h>

#include "Config.h"

/* create ultrasonic instance */
//Ultrasonic ultrasonic(ULTRASONIC_TRIGGER_PIN, ULTRASONIC_ECHO_PIN);
SoftwareSerial cumSerial(CUM_RX, CUM_TX);

static unsigned long currTime = 0;
static unsigned long prevTime = 0;
static unsigned long prevDetectTime = 0;
static unsigned long openEndTime = 0;
static unsigned long closeEndTime = 0;
unsigned long autoRaiseTime = conf[EGG_ID - 1].idleInterval;
int intEggStatus = CLOSE_END;
int extEggStatus = 0;
int cmMesc = 0;
long microSec = 0;

void setup()
{
    cumSerial.begin(9600);
    Serial.begin(9600);

#ifdef LED_BLINK
    ledSetup(currTime, conf[EGG_ID - 1].ledLightInterval, conf[EGG_ID - 1].ledDarkInterval);
#endif // LED_BLINK
#ifdef SERVO_ROTATE
    servoSetup(currTime, conf[EGG_ID - 1].servoRaiseInterval, conf[EGG_ID - 1].servoDownInterval);
#endif // SERVO_ROTATE
    randomSeed(millis());

    // init DFPlayer
    mp3_set_serial(cumSerial);
    mp3_single_loop(false);
    mp3_set_reply(false);
    mp3_set_volume(conf[EGG_ID - 1].voiceVolume);
    currTime = millis();
    closeEndTime = currTime;
    openEndTime = currTime;
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
            cmMesc = analogRead(LIGHT_SENSOR_PIN);
//            microSec = ultrasonic.timing();
//            cmMesc = ultrasonic.convert(microSec, Ultrasonic::CM);
            Serial.print("cmMesc: ");
            Serial.println(cmMesc);
//            Serial.print("closeEndTime: ");
//            Serial.println(closeEndTime);
//            Serial.print("currTime: ");
//            Serial.println(currTime);
//            Serial.print("extEggStatus: ");
//            Serial.println(extEggStatus);
//            Serial.print("autoRaiseTime: ");
//            Serial.println(autoRaiseTime);
            if (cmMesc < conf[EGG_ID - 1].closeDist || 
                    (((extEggStatus & MODE) != 0) && (closeEndTime >= openEndTime) && (currTime - closeEndTime > autoRaiseTime)))
            {
                if (cmMesc < conf[EGG_ID - 1].closeDist)
                {
                    extEggStatus = extEggStatus & (~MODE);
                }
                prevDetectTime = currTime;
                switch (intEggStatus)
                {
                    case OPEN_START:
                    case OPEN_END:
                    case VOICE_START:
                    case VOICE_END:
                    case CLOSE_START:
                        break;
                    case CLOSE_END:
                    default:
                        setEggStatus(currTime, 1);
                        break;
                }
            }
        }
        if (((extEggStatus & MODE) != 0) && (closeEndTime >= openEndTime) && (currTime - closeEndTime > conf[EGG_ID - 1].idleInterval))
        {
            autoRaiseTime = conf[EGG_ID - 1].idleInterval + random(conf[EGG_ID - 1].randomDelayInterval);
        }
        /* Change to manual mode */
        if ((extEggStatus & MODE) == 0 && (currTime - closeEndTime > conf[EGG_ID - 1].idleInterval))
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
    if (intEggStatus == VOICE_START)
    {
        intEggStatus = VOICE_END;
    }
    else if (intEggStatus == OPEN_END && currT - openEndTime > conf[EGG_ID - 1].voiceWaitPeriod)
    {
        intEggStatus = VOICE_START;
        // TODO: play sound
        mp3_play(EGG_ID);
    }
    else if (raiseOrDown == 0 && (intEggStatus == VOICE_START || intEggStatus == VOICE_END) 
            && (currTime - prevDetectTime > conf[EGG_ID - 1].eggRaisePeriod))
    {
        intEggStatus = CLOSE_START;    
	extEggStatus &= ~STATUS;
        autoRaiseTime = currT;
        closeEndTime = currT;
        mp3_stop();
        // TODO: emit close event.
    }
    else if (raiseOrDown == 1 && intEggStatus == CLOSE_END)
    {
        intEggStatus = OPEN_START;    
	extEggStatus |= STATUS;
        closeEndTime = currT;
        // TODO: emit open event.
    }
}

