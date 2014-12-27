#ifdef SERVO_ROTATE

#include <Servo.h>

static unsigned long prevServoTime = 0;
int servoUpPrd = 500;
int servoDownPrd = 1000;
int movePeriod = 0;
int currPosition = 1500;

Servo servo;

void servoSetup(unsigned long currT, int upInterval, int downInterval)
{
    servo.attach(SERVER_PIN);
    servo.writeMicroseconds(currPosition);
    servoUpPrd = (SERVO_HIGH_MS - SERVO_LOW_MS) * FRAME_MS / upInterval;
    servoDownPrd = (SERVO_HIGH_MS - SERVO_LOW_MS) * FRAME_MS / downInterval;
}

void servoLoop(unsigned long currT)
{
    if (eggStatus == OPEN_START)
    {
        if (SERVO_HIGH_MS - currPosition > servoUpPrd)
        {
            currPosition += servoUpPrd;
        }
        else
        {
            currPosition = SERVO_HIGH_MS;
            eggStatus = OPEN_END;
        }
        servo.writeMicroseconds(currPosition);
    }
    else if (eggStatus == CLOSE_START)
    {
        if (currPosition - SERVO_LOW_MS > servoDownPrd)
        {
            currPosition -= servoDownPrd;
        }
        else
        {
            currPosition = SERVO_LOW_MS;
            eggStatus = CLOSE_END;
        }
        servo.writeMicroseconds(currPosition);
    }
}

/**
 * servoStat: 0 for down, 1 for raise
 */
void servoSetStatus(unsigned long currT, int servoStat)
{
    if (servoStat == 0 && (eggStatus == VOICE_START || eggStatus == VOICE_END))
    {
        eggStatus = CLOSE_START;    
        // TODO: emit close event.
    }
    else if (servoSetup == 1 && (eggStatus == CLOSE_START || eggStatus == CLOSE_END))
    {
        eggStatus = OPEN_START;    
        // TODO: emit open event.
    }
}

#endif // SERVO_ROTATE
