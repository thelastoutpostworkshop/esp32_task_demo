// Push Button Sketch
//
#define PUSH_BUTTION_PIN 1
unsigned long lastDebounceTime;
int lastState = -1;
int state = -1;

extern int currentGifPlayed;
extern volatile bool gifChangeRequested;

void buttonSketch(void *parameter);
bool ButtonPressed();

void buttonSketch(void *parameter)
{
    pinMode(PUSH_BUTTION_PIN, INPUT_PULLUP);
    lastDebounceTime = millis();

    for (;;)
    {
        if (ButtonPressed())
        {
            Serial.println("Button Pressed, switching to the next GIF");
            currentGifPlayed = (currentGifPlayed + 1) % GIF_COUNT; // Update the GIF index
            gifChangeRequested = true;                             // Signal screenTask
        }
        delay(5); // Small delay for debouncing
    }
}

bool ButtonPressed()
{
    int currentState = digitalRead(PUSH_BUTTION_PIN);
    if (currentState != lastState)
    {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > 1)
    {
        if (currentState != state)
        {
            state = currentState;
            lastState = currentState;
            return state == LOW; // Returns true if the button is pressed
        }
    }

    lastState = currentState;
    return false; // No change in state
}