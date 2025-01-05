// Neopixels Sketch

#include <Adafruit_NeoPixel.h> // Install this library with the Arduino IDE Library Manager
                               // Tested on version 1.12.3

// Neopixels
// Define macros for color indices
#define COLOR_RED 0
#define COLOR_GREEN 1
#define COLOR_BLUE 2
#define COLOR_CYAN 3
#define COLOR_MAGENTA 4
#define COLOR_YELLOW 5
#define COLOR_WHITE 6
#define COLOR_ORANGE 7
#define COLOR_PURPLE 8
#define COLOR_PINK 9

#define PIXELS_PIN 4
#define PIXELS_COUNT 6
Adafruit_NeoPixel neopixels(PIXELS_COUNT, PIXELS_PIN, NEO_GRB + NEO_KHZ800);
volatile int currentPixelColor = COLOR_CYAN;

void neopixelsSketch(void *parameter);
void showRandomPixels();
uint32_t randomColor();

void neopixelsSketch(void *parameter)
{
    // Setup start here
    neopixels.begin();
    neopixels.setBrightness(30);
    randomSeed(analogRead(0)); // Seed the random generator

    // End of your setup

    // Loop function, run repeatedly
    for (;;)
    {
        showRandomPixels();
        delay(2000); // 2 seconds before choosing random pixels to lit
    }
}

void showRandomPixels()
{
    // Turn off all pixels first
    neopixels.clear();

    // Light up a random number of pixels (1 to PIXEL_COUNT)
    int numPixelsToLight = random(1, PIXELS_COUNT + 1);
    uint32_t color = randomColor(); // Generate a random color
    for (int i = 0; i < numPixelsToLight; i++)
    {
        int randomPixel = random(0, PIXELS_COUNT); // Random pixel index
        neopixels.setPixelColor(randomPixel, color);
    }

    neopixels.show(); // Update the pixels to reflect changes
}

uint32_t randomColor()
{
    // Generate a random color
    return neopixels.Color(random(0, 256), random(0, 256), random(0, 256));
}
