#ifndef APA102cDataStreamer_H
#define APA102cDataStreamer_H
#include "resouce/fakePinToPort.h"
#include "APA102cColorDataBuffer.h"
template<std::size_t NUM_USE_CH>
class APA102cDataStreamer{
public:
    APA102cDataStreamer()
    : colorDataBuffer()
    , bitTransformBuffer()
    {
        initRate(1000000).initGlobalBrightness(31); //default
    }

    APA102cDataStreamer<NUM_USE_CH>& initPin(const uint8_t dataPin, const uint8_t clockPin){
        dataPinPort = fakePinToPort(dataPin);
        changeModeDigitalWrite(dataPin);
        clockPinPort = fakePinToPort(clockPin);
        changeModeDigitalWrite(clockPin);
        return *this;
    }

    APA102cDataStreamer<NUM_USE_CH>& initRate(const uint32_t Hz){
        waitClockTimeH = teensy32_Rate_Hz / Hz;
        return *this;
    }

    APA102cDataStreamer<NUM_USE_CH>& initGlobalBrightness(uint8_t brightness = 31){
        if(brightness > 31) brightness = 31;
        for(findex = 0 ; findex < NUM_USE_CH ; ++findex){
            colorDataBuffer.rawData[findex].global = brightness;
        }
        return *this;
    }

    void show(){
        sendStartData();
        sendallAllData();
        sendEndData();
    }

    void setFillColor(const uint8_t r, const uint8_t g, const uint8_t b){
        for(findex = 0 ; findex < NUM_USE_CH ; ++findex){
            colorDataBuffer[findex * 3 + 0] = r;
            colorDataBuffer[findex * 3 + 1] = g;
            colorDataBuffer[findex * 3 + 2] = b;
        }
    }

    APA102cColorDataBuffer<NUM_USE_CH> colorDataBuffer;
private:

    inline void sendallAllData(){
        for(findex = 0 ; findex < NUM_USE_CH ; ++findex){
            writeBitSifter<3>(colorDataBuffer.rawData[findex].head);
            writeBitSifter<5>(colorDataBuffer.rawData[findex].global);
            writeBitSifter<8>(colorDataBuffer.rawData[findex].blue);
            writeBitSifter<8>(colorDataBuffer.rawData[findex].green);
            writeBitSifter<8>(colorDataBuffer.rawData[findex].red);
        }
    }

    inline void sendStartData(){
       *dataPinPort = 0xFE;
       for(bitWriteCounter = 0 ; bitWriteCounter < 32 ; ++bitWriteCounter){
           clockHigh();
           clockLow();
       }
   }

   inline void sendEndData(){
       *dataPinPort = 0x01;
       for(bitWriteCounter = 0 ; bitWriteCounter < 32 ; ++bitWriteCounter){
           clockHigh();
           clockLow();
       }
   }

    template<std::size_t bitNum>
    inline void writeBitSifter(const uint8_t byte){
        bitTransformBuffer = byte << (32 - bitNum);
        for(bitWriteCounter = 0 ; bitWriteCounter < bitNum ; ++bitWriteCounter){
            clockLow();
            asm volatile(
                "lsls %[byte], %[byte], #1"     "\n\t"
                "bcs L_%=_HIGH"                      "\n\t"
                "mov %[dataOut], #0xFE"         "\n\t"
                "b L_%=_END"                         "\n\t"

                "L_%=_HIGH: mov %[dataOut], #0x01"   "\n\t"
                "b L_%=_END"                         "\n\t"

                "L_%=_END:"
                :[dataOut] "=g" (*dataPinPort)
                ,[byte] "+r" (bitTransformBuffer)
                : :"memory"
            );
            clockHigh();
        }
    }

    inline void clockHigh(){
        waitClock();
        asm volatile(
            "ldr  %[clockOut], =0x01" "\n\t"
            :[clockOut]"=r" (*clockPinPort) :  :"memory"
        );
    }

    inline void clockLow(){
        waitClock();
        asm volatile(
            "ldr  %[clockOut], =0xFE" "\n\t"
            :[clockOut] "=r" (*clockPinPort) :  :"memory"
        );
    }

    inline void waitClock(){
        waitClockNum = waitClockTimeH >> 2;
        asm volatile(
            "L_%=_waitClock:subs %0, #1"   "\n\t"
            "bne L_%=_waitClock"           "\n\t"
            : "+r" (waitClockNum) :
      );
    }

    uint32_t bitTransformBuffer;
    std::size_t findex;
    uint8_t bitWriteCounter;
    uint8_t* dataPinPort;
    uint8_t* clockPinPort;

    uint32_t waitClockNum;
    uint32_t waitClockTimeH;

    const uint32_t teensy32_Rate_Hz = 720000000;
};
 #endif /* end of include guard: APA102cDcataStreamer_H */
