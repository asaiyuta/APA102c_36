# APA102c
APA102c_for_teensy

Improving now...

```cpp:example  
    APA102cDataStreamer<18> apa102c;
    apa102c.initPin(6,7).initRate(1500000).initGlobalBrightness(1);  // initPint(data,clock) initRate(Hz) 
    apa102c.setFillColor(255,0,0); //all Leds
    apa102c.colorDataBuffer[0] = 200; // pixel 0.r
    apa102c.colorDataBuffer[1] = 200; // pixel 0.g
    apa102c.colorDataBuffer[2] = 200; // pixel 0.b

    apa102c.show();
```
