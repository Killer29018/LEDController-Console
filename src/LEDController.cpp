#include "LEDController.hpp"

#include <cmath>
#include <algorithm>

#include <sstream>

cHSV RGB2HSV(const cRGB& rgb)
{
     cHSV hsv;

     float Rp = rgb.R / 255.0;
     float Gp = rgb.G / 255.0;
     float Bp = rgb.B / 255.0;

     float max = std::max(Rp, Gp);
     max = std::max(max, Bp);

     float min = std::min(Rp, Gp);
     min = std::min(min, Bp);

     float delta = max - min;
     uint16_t h;

     hsv.V = 255 * max;
     if (max < 0.0001)
         hsv.S = 0;
     else
         hsv.S = 255 * (delta/max);

     if (delta < 0.0001)
         h = 0;
     else if (max == Rp)
         h = 60 * std::fmod(((Gp-Bp) / delta), 6);
     else if (max == Gp)
         h = 60 * (((Bp-Rp) / delta) + 2);
     else if (max == Bp)
         h = 60 * (((Rp-Gp) / delta) + 4);

     hsv.H = (h / 360.0) * 255;

     return hsv;
}

cRGB HSV2RGB(const cHSV& hsv)
{
    // Rewritten from FastLED hsv2rgb.cpp hsv2rgb_rainbow

    cRGB rgb;

    // Scale yellow up
    const uint8_t Y1 = 1;
    // Scale yellow up more
    const uint8_t Y2 = 0;

    // Scale green down
    const uint8_t G2 = 0;
    // Scale green down by what
    const uint8_t Gscale = 0;

    uint8_t hue = hsv.H;
    uint8_t sat = hsv.S;
    uint8_t val = hsv.V;

    uint8_t offset = hue & 0x1F; // 0..31

    uint8_t offset8 = offset << 3;
    
#define K255 255
#define K171 171
#define K170 170
#define K85  85
#define scale8(i, sc) ((uint16_t)(i) * (uint16_t)(sc)) >> 8 
#define scale8_video(i, sc) (((int)i * (int)sc) >> 8) + ((i && sc) ? 1 : 0) 
    uint8_t third = scale8(offset8, (256 / 3));

    if (!(hue & 0x80))
    { // 0xx
        if (!(hue & 0x40))
        { // 00x
            if (!(hue & 0x20))
            { // 000
                rgb.R = K255 - third;
                rgb.G = third;
                rgb.B = 0;
            }
            else
            { // 001
                if (Y1)
                {
                    rgb.R = K171;
                    rgb.G = K85 + third;
                    rgb.B = 0;
                }
                if (Y2)
                {
                    rgb.R = K170 + third;
                    uint8_t twoThirds = scale8(offset8, ((256*2)/3));
                    rgb.G = K85 + twoThirds;
                    rgb.B = 0;
                }
            }
        }
        else
        { // 01x
            if (!(hue & 0x20))
            { //010
                if (Y1)
                {
                    uint8_t twoThirds = scale8(offset8, ((256*2) / 3));

                    rgb.R = K171 - twoThirds;
                    rgb.G = K170 + third;
                    rgb.B = 0;
                }
                if (Y2)
                {
                    rgb.R = K255 - offset8;
                    rgb.G = K170 + third;
                    rgb.B = 0;
                }
            }
            else
            { // 011
                rgb.R = 0;
                rgb.G = K255 - third;
                rgb.B = third;
            }
        }
    }
    else
    { //1xx
        if(!(hue & 0x40))
        { // 10X
            if(!(hue & 0x20))
            { // 100
                rgb.R = 0;
                uint8_t twothirds = scale8( offset8, ((256 * 2) / 3)); // max=170
                rgb.G = K171 - twothirds; 
                rgb.B = K85  + twothirds;
            }
            else
            { // 101
                rgb.R = third;
                rgb.G = 0;
                rgb.B = K255 - third;
            }
        }
        else
        {
            if(!(hue & 0x20))
            { // 110
                rgb.R = K85 + third;
                rgb.G = 0;
                rgb.B = K171 - third;
            }
            else
            { // 111
                rgb.R = K170 + third;
                rgb.G = 0;
                rgb.B = K85 - third;
            }
        }
    }

    if (G2) rgb.G = rgb.G >> 1;
    if( Gscale ) rgb.G = scale8_video( rgb.G, Gscale);
    
    if (sat != 255)
    {
        if (sat == 0)
        {
            rgb.R = 255;
            rgb.G = 255;
            rgb.B = 255;
        }
        else
        {
            uint8_t desat = 255 - sat;
            desat = scale8_video(desat, desat);
            uint8_t satscale = 255 - desat;

            if (rgb.R) rgb.R = scale8(rgb.R, satscale) + 1;
            if (rgb.G) rgb.G = scale8(rgb.G, satscale) + 1;
            if (rgb.B) rgb.B = scale8(rgb.B, satscale) + 1;

            uint8_t brightnessFloor = desat;
            rgb.R += brightnessFloor;
            rgb.G += brightnessFloor;
            rgb.B += brightnessFloor;
        }
    }

    if (val != 255)
    {
        val = scale8_video(val, val);
        if (val == 0)
        {
            rgb.R = 0;
            rgb.G = 0;
            rgb.B = 0;
        }
        else
        {
            if (rgb.R) rgb.R = scale8(rgb.R, val) + 1;
            if (rgb.G) rgb.G = scale8(rgb.G, val) + 1;
            if (rgb.B) rgb.B = scale8(rgb.B, val) + 1;
        }
    }

#undef scale8_video
#undef scale8
#undef K85
#undef K170
#undef K171
#undef K255
    return rgb;
}

cRGB RGBFromHex(const uint32_t& hex)
{
    cRGB rgb;
    rgb.R = hex >> 16;
    rgb.G = hex >>  8;
    rgb.B = hex >>  0;
    return rgb;
}

std::string printRGB(const cRGB& rgb)
{
    std::stringstream ss;
    ss << (int)rgb.R << " : " << (int)rgb.G << " : " << (int)rgb.B;
    return ss.str();
}

std::string printHSV(const cHSV& hsv)
{
    std::stringstream ss;
    ss << (int)hsv.H << " : " << (int)(100 * (hsv.S / 255.0)) << " : " << (int)(100 * (hsv.V / 255.0));
    return ss.str();
}

void LEDController::setup(unsigned int LEDCount)
{
    m_LEDs.resize(LEDCount);
    memset(&m_LEDs[0], 0x10, LEDCount * sizeof(cRGB));

    m_TotalPackets = std::ceil((LEDCount * sizeof(cRGB)) / (MAX_BYTES - 7.0));

    setDataBounds();
}

void LEDController::upload(Socket& socket)
{
    int currentByte = 0;
    int currentIndex = 0;
    constexpr int offset = 6;
    for (int i = 0; i < m_TotalPackets; i++)
    {
        m_DataBuffer[4] = i + 1; // Current Packet
        while (currentByte <= (MAX_BYTES - 7 - 3) || currentIndex == m_LEDs.size())
        {
            float brightness = 1.0 / (255.0 / m_Brightness);
            m_DataBuffer[offset + currentByte++] = m_LEDs[currentIndex].R * brightness;
            m_DataBuffer[offset + currentByte++] = m_LEDs[currentIndex].G * brightness;
            m_DataBuffer[offset + currentByte++] = m_LEDs[currentIndex].B * brightness;
            currentIndex++;
        }
        setDataSize(currentByte);
        currentByte = 0;
        socket.sendData(m_DataBuffer, MAX_BYTES);
    }
}

void LEDController::setLED(int index, const cRGB& led) 
{ 
    m_LEDs[index] = led; 
}

void LEDController::setLED(int index, const cHSV& led)
{
    setLED(index, HSV2RGB(led));
}

cRGB& LEDController::getLED(int index) 
{ 
    return m_LEDs[index]; 
}

void LEDController::fillSolid(cRGB led)
{
    std::fill(m_LEDs.begin(), m_LEDs.end(), led);
}

void LEDController::fillSolid(cHSV led)
{
    fillSolid(HSV2RGB(led));
}

void LEDController::fillRainbow(cRGB led, uint8_t hueChange)
{
    fillRainbow(RGB2HSV(led), hueChange);
}

void LEDController::fillRainbow(cHSV hsv, uint8_t hueChange)
{
    for (int i = 0; i < m_LEDs.size(); i++)
    {
        uint8_t hue = hsv.H + (hueChange * i);
        cRGB rgb = HSV2RGB({ hue, hsv.S, hsv.V });
        m_LEDs[i] = rgb;
    }
}

void LEDController::setDataBounds()
{
    m_DataBuffer[0] = 0x9C; // Start Byte
    m_DataBuffer[1] = 0xDA; // Data
    m_DataBuffer[MAX_BYTES - 1] = 0x36; // End Byte

    m_DataBuffer[5] = m_TotalPackets; // Total Packets
}

void LEDController::setDataSize(int size)
{
    m_DataBuffer[2] = size >> 8;
    m_DataBuffer[3] = size;
}
