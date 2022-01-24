#include "LEDController.hpp"

#include <cmath>
#include <algorithm>

cHSV rgb2hsv(const cRGB& rgb)
{
    cHSV hsv;
    uint8_t min, max, delta;
    
    min = std::min(rgb.R, rgb.G);
    min = std::min(min, rgb.B);

    max = std::max(rgb.R, rgb.G);
    max = std::max(max, rgb.B);

    hsv.V = max;
    delta = max - min;
    if (max != 0)
    {
        hsv.S = (delta*255 / max);
    }
    else
    {
        hsv.S = 0.0;
        hsv.H = 0.0;
        return hsv;
    }

    if (rgb.R == max)
        hsv.H = (rgb.G - rgb.B) * (60/delta);
    else if (rgb.G == max)
        hsv.H = 120 + (rgb.B - rgb.R) * (60/delta);
    else
        hsv.H = 240 + (rgb.R - rgb.G) * (60/delta);
    return hsv;
}

cRGB hsv2rgb(const cHSV& hsv)
{

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

cRGB& LEDController::getLED(int index) 
{ 
    return m_LEDs[index]; 
}

void LEDController::solidColour(const cRGB& led)
{
    std::fill(m_LEDs.begin(), m_LEDs.end(), led);
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
