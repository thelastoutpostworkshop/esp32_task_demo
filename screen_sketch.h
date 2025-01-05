// Screen Sketch
//
#include <bb_spi_lcd.h>  // Install this library with the Arduino IDE Library Manager
                         // Tested on version 2.7.1
#include <AnimatedGIF.h> // Install this library with the Arduino IDE Library Manager
                         // Tested on version 2.1.1

#include "gif/alien_eye.h"
#include "gif/supernova.h"

// Main object for the display driver
BB_SPI_LCD tft;

// Pins definition for the display
#define TFT_MISO -1
#define TFT_MOSI 9 // SDA
#define TFT_SCLK 3 // SDA
#define TFT_CS 8   // Chip select control pin
#define TFT_DC 18  // Data Command control pin
#define TFT_RST 17 // Reset pin (could connect to RST pin)

// GIFs to display
#define GIF_COUNT 2 // Number of GIFs to cycle through, if you have enough space on flash memory
int currentGifPlayed = 0;
const uint8_t *gifData[GIF_COUNT] = {alien_eye,supernova};                // Add more GIFs here if you have enough space on flash memory
const size_t gifSizes[GIF_COUNT] = {sizeof(alien_eye), sizeof(supernova)}; // Add corresponding sizes here
AnimatedGIF *gifTopPlay[GIF_COUNT];
volatile bool gifChangeRequested = false;

void GIFDraw(GIFDRAW *pDraw);
AnimatedGIF *openGif(uint8_t *gifdata, size_t gifsize);
void screenSketch(void *parameter);
void *GIFAlloc(uint32_t u32Size);
void printGifErrorMessage(int errorCode);

void screenSketch(void *parameter)
{
    tft.begin(LCD_GC9A01, FLAGS_SWAP_RB | FLAGS_FLIPX, 40000000, TFT_CS, TFT_DC, TFT_RST, -1, TFT_MISO, TFT_MOSI, TFT_SCLK);
    tft.setRotation(LCD_ORIENTATION_0);
    tft.fillScreen(TFT_BLACK);

    // Opening all the GIFs
    for (int i = 0; i < GIF_COUNT; i++)
    {
        gifTopPlay[i] = openGif((uint8_t *)gifData[i], gifSizes[i]);
        if (gifTopPlay[i] == NULL)
        {
            Serial.println("Cannot open GIF");
            while (true)
            {
            }
        }
    }

    for (;;)
    {
        // Play the current frame of the current GIF
        bool framePlayed = gifTopPlay[currentGifPlayed]->playFrame(true, NULL);

        // Check if a new GIF has been requested
        if (framePlayed && gifChangeRequested)
        {
            gifChangeRequested = false; // Reset the flag
            tft.fillScreen(TFT_BLACK);  // Clear the screen
            gifTopPlay[currentGifPlayed]->reset();
            Serial.printf("Switching to GIF %d\n", currentGifPlayed);
        }
    }
}

// Open Gif and allocate memory
AnimatedGIF *openGif(uint8_t *gifdata, size_t gifsize)
{
    AnimatedGIF *gif;
    gif = (AnimatedGIF *)malloc(sizeof(AnimatedGIF));
    if (gif == NULL)
    {
        Serial.println("Not RAM Enough memory for GIF structure");
        return NULL;
    }

    gif->begin(GIF_PALETTE_RGB565_BE); // Set the cooked output type we want (compatible with SPI LCDs)

    if (gif->open(gifdata, gifsize, GIFDraw))
    {
        Serial.printf("Successfully opened GIF; Canvas size = %d x %d\n", gif->getCanvasWidth(), gif->getCanvasHeight());
        Serial.printf("GIF memory size is %ld (%2.2f MB)\n", gifsize, (float)gifsize / (1024 * 1024));
        gif->setDrawType(GIF_DRAW_COOKED); // We want the Animated GIF library to generate ready-made pixels
        if (gif->allocFrameBuf(GIFAlloc) != GIF_SUCCESS)
        {
            Serial.println("Not Enough RAM memory for frame buffer");
            return NULL;
        }
        return gif;
    }
    else
    {
        printGifErrorMessage(gif->getLastError());
        return NULL;
    }
}

// Draw callback from the AnimatedGIF decoder
void GIFDraw(GIFDRAW *pDraw)
{
    if (pDraw->y == 0)
    { // set the memory window (once per frame) when the first line is rendered
        tft.setAddrWindow(pDraw->iX, pDraw->iY, pDraw->iWidth, pDraw->iHeight);
    }
    // For all other lines, just push the pixels to the display. We requested 'COOKED'big-endian RGB565 and
    tft.pushPixels((uint16_t *)pDraw->pPixels, pDraw->iWidth);
}

void *GIFAlloc(uint32_t u32Size)
{
    return malloc(u32Size);
}

// Get human-readable error related to GIF
void printGifErrorMessage(int errorCode)
{
    switch (errorCode)
    {
    case GIF_DECODE_ERROR:
        Serial.println("GIF Decoding Error");
        break;
    case GIF_TOO_WIDE:
        Serial.println("GIF Too Wide");
        break;
    case GIF_INVALID_PARAMETER:
        Serial.println("Invalid Parameter for gif open");
        break;
    case GIF_UNSUPPORTED_FEATURE:
        Serial.println("Unsupported feature in GIF");
        break;
    case GIF_FILE_NOT_OPEN:
        Serial.println("GIF File not open");
        break;
    case GIF_EARLY_EOF:
        Serial.println("GIF early end of file");
        break;
    case GIF_EMPTY_FRAME:
        Serial.println("GIF with empty frame");
        break;
    case GIF_BAD_FILE:
        Serial.println("GIF bad file");
        break;
    case GIF_ERROR_MEMORY:
        Serial.println("GIF memory Error");
        break;
    default:
        Serial.println("Unknown Error");
        break;
    }
}