#include "APA102c_36.h"
#define NUM_PIXEL 18
APA102cDataStreamer<NUM_PIXEL> test;
void setup() {
    test.initPin(6,7).setRate(10000000).setGlobalBrightness(255);
}

void loop() {
    test.setFillColor(255,0,0);
    test.show();
    test.show();
    delay(650);
    test.setFillColor(0,255,0);
    test.show();
    test.show();
    delay(650);
    test.setFillColor(0,0,255);
    test.show();
    test.show();
    delay(650);
    test.setFillColor(0,0,0);
    test.show();
    test.show();

    for(int ch = 0 ; ch < NUM_PIXEL ; ++ch){
      for(uint8_t b = 0 ; b < 255 ; ++b){
        test.colorDataBuffer[ch * 3 + 0] = b; //R
        test.colorDataBuffer[ch * 3 + 1] = b; //G
        test.colorDataBuffer[ch * 3 + 2] = b; //B
        test.show();
        delay(1);     
      }
    test.show();
    }
}
