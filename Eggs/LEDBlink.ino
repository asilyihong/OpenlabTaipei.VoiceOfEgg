#ifdef LED_BLINK

static unsigned long prevLedTime;
int ledStatus = LOW;
int darkInterval = 500;
int lightInterval = 1000;

void ledSetup(unsigned long currT, int light, int dark)
{
    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);
    ledSetStatus(currT, LOW);
    prevLedTime = currT;
    lightInterval = light;
    darkInterval = dark;
}

void ledLoop(unsigned long currT)
{
    if (intEggStatus != CLOSE_END)
    {
	if (ledStatus == LOW && currT - prevLedTime > darkInterval)
	{
	    ledSetStatus(currT, HIGH);
	}
	else if (ledStatus == HIGH && currT - prevLedTime > lightInterval)
	{
	    ledSetStatus(currT, LOW);
	}
    }
    else
    {
        if (ledStatus == HIGH)
        {
            ledSetStatus(currT, LOW);
        }
	prevLedTime = currT;
    }
}

void ledSetStatus(unsigned long currT, int ledStat)
{
    prevLedTime = currT;
    ledStatus = ledStat;
    digitalWrite(LED1_PIN, ledStatus);
    digitalWrite(LED2_PIN, ledStatus);
}

#endif
