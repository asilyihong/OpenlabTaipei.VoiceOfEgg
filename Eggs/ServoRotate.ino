#ifdef SERVO_ROTATE

#include <ServoTimer2.h>

static unsigned long prevServoTime = 0;
int servoUpPrd = 50;
int servoDownPrd = 12;
int currPosition = 1500;

ServoTimer2 servo;

void servoSetup(unsigned long currT, int upInterval, int downInterval)
{
    servo.attach(SERVO_PIN);
    servo.write(currPosition);
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
            openEndTime = currT;
        }
        servo.write(currPosition);
    }
    else if (intEggStatus == CLOSE_START)
    {
        if (currPosition - SERVO_LOW_MS > servoDownPrd)
        {
            currPosition -= servoDownPrd;
        }
        else
        {
            closeEndTime = currT;
            currPosition = SERVO_LOW_MS;
            intEggStatus = CLOSE_END;
        }
        servo.write(currPosition);
    }
}

#endif // SERVO_ROTATE
