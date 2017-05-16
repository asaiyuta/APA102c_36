#ifndef APA102cColorDataBuffer_H
#define APA102cColorDataBuffer_H
#include <array>
struct APA102cColorData{
    APA102cColorData()
    : head(0b111)
    {}
    union{
        uint32_t raw;
        struct{
            const uint8_t head:3;
            uint8_t global:5;
            uint8_t blue;
            uint8_t green;
            uint8_t red;
        };

        struct{
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        };
    };
};

template<std::size_t SIZE>
struct APA102cColorDataBuffer{
    APA102cColorDataBuffer(){
        for(std::size_t i = 0 ; i < SIZE ; ++i){
            RGB_array[i * 3 + 0] = &(rawData[i].red);
            RGB_array[i * 3 + 1] = &(rawData[i].green);
            RGB_array[i * 3 + 2] = &(rawData[i].blue);
        }
    }

    uint8_t& operator[](const std::size_t index){
        return *(RGB_array[index]);
    }

    std::array<uint8_t*, SIZE * 3> RGB_array;
    std::array<APA102cColorData, SIZE> rawData;
};




#endif /* end of include guard: APA102cColorDataBuffer_H */
