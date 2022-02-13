#ifndef EFFECT_EFFECT_FIRE_HPP
#define EFFECT_EFFECT_FIRE_HPP

#include "Effect.hpp"

#include <vector>

class Effect_Fire : public Effect
{
private:
    float m_OffsetX;
    float m_OffsetY;

    float m_Speed;

    float m_CoolingFactor;

    uint32_t m_BufferSizeX, m_BufferSizeY;
    std::vector<cHSV> m_Buffer1;
    std::vector<cHSV> m_Buffer2;
public:
    Effect_Fire(LEDMatrix* matrix);
    ~Effect_Fire();

    void update() override;
    void render(const char* panelName) override;
private:
    uint8_t getValue(uint32_t x, uint32_t y);
    uint32_t getIndex(uint32_t x, uint32_t y);

    void checkBufferSize();

    uint8_t getAverage(uint32_t x, uint32_t y);
    uint8_t average_EvenBelow(uint32_t x, uint32_t y);
    uint8_t average_DirectBelow(uint32_t x, uint32_t y);

    uint8_t getConvertedHue(uint8_t value);
};

#endif
