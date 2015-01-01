#ifdef SERVO_ROTATE

#include <Servo.h>

static unsigned long prevServoTime = 0;
int servoUpPrd = 50;
int servoDownPrd = 12;
int currPosition = 1500;

Servo servo;

void servoSetup(unsigned long currT, int upInterval, int downInterval)
{
    servo.attach(SERVO_PIN);
    servo.writeMicroseconds(currPosition);
    servoUpPrd = (SERVO_HIGH_MS - SERVO_LOW_MS) * FRAME_MS / upInterval;
    servoDownPrd = (SERVO_HIGH_MS - SERVO_LOW_MS) * FRAME_MS / downInterval;
}

void servoLoop(unsigned long currT)
{
    if (intEggStatus == OPEN_START)
    {
        if (SERVO_HIGH_MS - currPosition > servoUpPrd)
        {
            currPosition += servoUpPrd;
        }
        else
        {
            currPosition = SERVO_HIGH_MS;
            intEggStatus = OPEN_END;
        }
        servo.writeMicroseconds(currPosition);
    }
    else if (intEggStatus == CLOSE_START)
    {
        if (currPosition - SERVO_LOW_MS > servoDownPrd)
        {
            currPosition -= servoDownPrd;
        }
        else
        {
            currPosition = SERVO_LOW_MS;
            intEggStatus = CLOSE_END;
        }
        servo.writeMicroseconds(currPosition);
    }
}

/**
 * servoStat: 0 for down, 1 for raise
 */
void servoSetStatus(unsigned long currT, int servoStat)
{
    if (servoStat == 0 && (intEggStatus == VOICE_START || intEggStatus == VOICE_END))
    {
        intEggStatus = CLOSE_START;    
	extEggStatus |= STATUS;
        // TODO: emit close event.
    }
    else if (servoStat == 1 && (intEggStatus == CLOSE_START || intEggStatus == CLOSE_END))
    {
        intEggStatus = OPEN_START;    
	extEggStatus &= ~STATUS;
        // TODO: emit open event.
    }
}

#endif // SERVO_ROTATE
