#include <SPI.h>

#include <Wire.h>

// Lepton-FLiR-Arduino Image Capture Example
// In this example, we will copy out thermal image frames to individual BMP files located
// on a MicroSD card using the SD library. Note that you will need a MicroSD card reader
// module for this example to work. Both the FLiR module and MicroSD card reader module
// will be on the same SPI lines, just using different chip enable pins/wires.

#include "LeptonFLiR.h"
LeptonFLiR flirController(Wire, 10); // Library using Wire and chip select pin D22



void setup() {
    Serial.begin(115200);

    Wire.begin();                       // Wire must be started first
    Wire.setClock(400000);              // Supported baud rates are 100kHz, 400kHz, and 1000kHz
    SPI.begin();                        // SPI must be started first as well

//    SD.begin(cardCSPin);                // SD library using chip select pin D24

    // Using memory allocation mode 80x60 8bpp and fahrenheit temperature mode
    flirController.init(LeptonFLiR_ImageStorageMode_80x60_8bpp, LeptonFLiR_TemperatureMode_Fahrenheit);

    // Setting use of AGC for histogram equalization (since we only have 8-bit per pixel data anyways)
    flirController.agc_setAGCEnabled(ENABLED);
//      flirController.readNextFrame();
//  byte *imageData = flirController.getImageData();
//  int height = flirController.getImageWidth();
//  int width = flirController.getImageHeight();
//  int pitch = flirController.getImagePitch();
//  uint32_t padSize  = (4-(width*3)%4)%4;
//
//  byte pad[3] = {0,0,0};
//    imageData += (height - 1) * pitch;
//
//    for (int y = 0; y < flirController.getImageHeight(); ++y) {
//            for (int x = 0; x < flirController.getImageWidth(); ++x) {
//                int val = flirController.getImageDataRowCol(y, x);
//                Serial.println(val);
//            }
//            Serial.println(y);
//        }

//    for (int y = height - 1; y >= 0; --y) {
//        for (int x = 0; x < width; ++x) {
//            byte pixel[3]; // blue green red
//            pixel[0] = pixel[1] = pixel[2] = imageData[x];
//            Serial.println("");
//        Serial.print("pixel");
//            Serial.print("\t");
//            Serial.print((long)pixel[0]);
//            Serial.print("\t");
//            Serial.print((long)pixel[1]);
//            Serial.print("\t");
//            Serial.print((long)pixel[2]);
//        }
//        Serial.println("");
//        Serial.print("pad");
//            Serial.print("\t");
//            Serial.print((long)pad[0]);
//            Serial.print("\t");
//            Serial.print((long)pad[1]);
//            Serial.print("\t");
//            Serial.print((long)pad[2]);
//        imageData -= pitch;
//    }

}


void loop() {
    if (flirController.readNextFrame()) { // Read next frame and store result into internal imageData
        // Find the hottest spot on the frame
        int hotVal = 0; 
        int hotX=0;
        int hotY=0;

        for (int y = 0; y < flirController.getImageHeight(); ++y) {
            for (int x = 0; x < flirController.getImageWidth(); ++x) {
                int val = flirController.getImageDataRowCol(y, x);

                if (val > hotVal) {
                    hotVal = val;
                    hotX = x; hotY = y;
                }
            }
        }

        Serial.print("Hottest point: [");
        Serial.print(hotX);
        Serial.print(",");
        Serial.print(hotY);
        Serial.println("]");
    }
}
