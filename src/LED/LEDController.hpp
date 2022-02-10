#ifndef LED_CONTROLLER_HPP
#define LED_CONTROLLER_HPP

#include <vector>
#include <cstdint>
#include "../System/Socket.hpp"
#include "../Colour.hpp"

#define MAX_BYTES 1400

class LEDController
{
public:
private:
    uint8_t m_DataBuffer[MAX_BYTES];
    uint32_t m_TotalPackets;
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
    cRGB getLEDWBrightness(int index);

    void fillSolid(cRGB colour);
    void fillSolid(cRGB colour, int start, int length);
    void fillRainbow(cHSV initial, uint8_t deltaHue = 5);

    uint32_t getLEDCount() { return m_LEDs.size(); }

    void changeSize(uint32_t size);
private:
    void setDataBounds();
    void setDataSize(int size);
};

#endif
