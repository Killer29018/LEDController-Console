#ifndef LED_CONTROLLER_HPP
#define LED_CONTROLLER_HPP

#include <vector>
#include <cstdint>
#include "Socket.hpp"

#define MAX_BYTES 1400

struct cRGB
{
    uint8_t R, G, B;

    cRGB(uint8_t R, uint8_t G, uint8_t B)
        : R(R), G(G), B(B) {}

    cRGB() : R(0x0), G(0x0), B(0x0) {}
};

struct cHSV
{
    uint16_t H;
    uint8_t S, V;
    cHSV(uint16_t H, uint8_t S, uint8_t V)
        : H(H), S(S), V(V) {}

    cHSV() : H(0x0), S(0x0), V(0x0) {}
};

cHSV rgb2hsv(const cRGB& rgb);
cRGB hsv2rgb(const cHSV& hsv);

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

    void solidColour(const cRGB& led);
private:
    void setDataBounds();
    void setDataSize(int size);
};

#endif
