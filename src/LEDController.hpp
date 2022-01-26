#ifndef LED_CONTROLLER_HPP
#define LED_CONTROLLER_HPP

#include <vector>
#include <cstdint>
#include "Socket.hpp"
#include "Colour.hpp"

#define MAX_BYTES 1400

class LEDController
{
public:
private:
    uint8_t m_DataBuffer[MAX_BYTES];
    unsigned int m_TotalPackets;
protected:
    std::vector<cRGB> m_LEDs;
    uint8_t m_Brightness = 255;
public:
    LEDController() = default;

    virtual void setup(unsigned int LEDCount);

    void upload(Socket& socket);

    void setBrightness(uint8_t brightness) { m_Brightness = brightness; }
    uint8_t& getBrightness() { return m_Brightness; }

    void setLED(int index, const cRGB& led);
    cRGB& getLED(int index);

    void fillSolid(cRGB led);
    void fillRainbow(cHSV initial, uint8_t deltaHue = 5);
private:
    void setDataBounds();
    void setDataSize(int size);
};

#endif
