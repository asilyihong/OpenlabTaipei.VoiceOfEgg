#include "Config.h"

static unsigned long currTime = 0;
static unsigned long prevTime = 0;
int childStatus[CHILDREN_COUNT] = {0};

void setup()
{
    currTime = millis();
    prevTime = currTime;
}

void loop()
{
    currTime = millis();

    if (currTime - prevTime > FRAME_US)
    {
        // TODO: read from child or send to child
    }
    else
    {
        delay(0);
    }
}
