//Youtube tutorial : https://youtu.be/382p1NT1Wcs

#include "screen_sketch.h"    // Screen Sketch
#include "button_sketch.h"    // Button Sketch
#include "neopixels_sketch.h" // Neopixels Sketch

void setup()
{
    Serial.begin(115200);

    xTaskCreatePinnedToCore(
        screenSketch, // Task function
        "taskName",   // Task name
        8192,         // Stack size
        NULL,         // Task input parameters
        1,            // Task priority, be carefull when changing this
        NULL,         // Task handle, add one if you want control over the task (resume or suspend the task)
        1             // Core to run the task on
    );

    xTaskCreatePinnedToCore(
        buttonSketch, // Task function
        "taskName",   // Task name
        8192,         // Stack size
        NULL,         // Task input parameters
        1,            // Task priority, be carefull when changing this
        NULL,         // Task handle, add one if you want control over the task (resume or suspend the task)
        1             // Core to run the task on
    );

    xTaskCreatePinnedToCore(
        neopixelsSketch, // Task function
        "taskName",      // Task name
        8192,            // Stack size
        NULL,            // Task input parameters
        1,               // Task priority, be carefull when changing this
        NULL,            // Task handle, add one if you want control over the task (resume or suspend the task)
        1                // Core to run the task on
    );
}

void loop()
{
}
