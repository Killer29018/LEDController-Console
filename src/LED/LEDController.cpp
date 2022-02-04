#include "LEDController.hpp"

#include <cmath>
#include <algorithm>

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

cRGB LEDController::getLEDWBrightness(int index)
{
    return m_LEDs[index] / (255.0 / m_Brightness);
}

void LEDController::fillSolid(cRGB led)
{
    std::fill(m_LEDs.begin(), m_LEDs.end(), led);
}

void LEDController::fillRainbow(cHSV hsv, uint8_t deltaHue)
{
    for (int i = 0; i < m_LEDs.size(); i++)
    {
        uint8_t hue = hsv.H + (deltaHue * i);
        m_LEDs[i] = cHSV(hue, hsv.S, hsv.V);
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
