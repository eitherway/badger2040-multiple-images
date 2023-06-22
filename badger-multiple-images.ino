#include <GxEPD2_BW.h>
#include "bitmaps.h"

/*
enum pin {
      A           = 12,
      B           = 13,
      C           = 14,
      D           = 15,
      E           = 11,
      UP          = 15, // alias for D
      DOWN        = 11, // alias for E
      USER        = 23,
      CS          = 17,
      CLK         = 18,
      MOSI        = 19,
      DC          = 20,
      RESET       = 21,
      BUSY        = 26,
      VBUS_DETECT = 24,
      LED         = 25,
      BATTERY     = 29,
      ENABLE_3V3  = 10
    };
    */

const int buttonUp = 15;
const int buttonDown = 11;

int downButtonState = 0;
int lastDownButtonState = 0;
int upButtonState = 0;
int lastUpButtonState = 0;

int displayedBitmap = 0;

// Docs about Pins can be found here: https://github.com/pimoroni/pimoroni-pico/blob/main/libraries/badger2040/badger2040.hpp
GxEPD2_BW <GxEPD2_290_T5D, GxEPD2_290_T5D::HEIGHT> display(
        GxEPD2_290_T5D(/*CS*/ 17, /*DC*/ 20, /*RST*/ 21, /*BUSY*/ 26)); // GDEW029T5D 128x296, UC8151D


void setup() {
    Serial.begin(9600);

    // initialize the button pins as an input:
    pinMode(buttonUp, INPUT_PULLDOWN);
    pinMode(buttonDown, INPUT_PULLDOWN);

    display.init(115200, true, 2, false); // USE THIS for Waveshare boards with "clever" reset circuit, 2ms reset pulse
    displayBitmap();
}

void displayBitmap() {
    display.setFullWindow();
    display.fillScreen(GxEPD_WHITE);
    display.drawInvertedBitmap(0, 0, epd_bitmap_allArray[displayedBitmap], 128, 296, GxEPD_BLACK);
    display.display();
    display.powerOff();
}

void loop() {
    downButtonState = digitalRead(buttonDown);
    upButtonState = digitalRead(buttonUp);

    // compare the upButtonState to its previous state
    if (upButtonState != lastUpButtonState) {
        if (upButtonState == HIGH) {
            Serial.println("up-on");
            displayedBitmap = (displayedBitmap + 1);

            if (displayedBitmap >= epd_bitmap_allArray_LEN) {
                // reset display if the next bitmap doesn't exist
                displayedBitmap = 0;
            }

            Serial.println("Display Bitmap: " + displayedBitmap);
            displayBitmap();
        } else {
            Serial.println("up-off");
        }
    }

    lastUpButtonState = upButtonState;

    // compare the downButtonState to its previous state
    if (downButtonState != lastDownButtonState) {
        if (downButtonState == HIGH) {
            Serial.println("down-on");
            displayedBitmap = displayedBitmap - 1;

            if (displayedBitmap < 0) {
                // reset to last bitmap
                displayedBitmap = epd_bitmap_allArray_LEN - 1;
            }

            Serial.println("Display Bitmap: " + displayedBitmap);
            displayBitmap();
        } else {
            Serial.println("down-off");
        }
    }

    lastDownButtonState = downButtonState;

    // Delay a little to avoid bouncing
    delay(50);
};
