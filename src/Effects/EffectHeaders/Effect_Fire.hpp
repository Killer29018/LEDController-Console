#ifndef EFFECT_EFFECT_FIRE_HPP
#define EFFECT_EFFECT_FIRE_HPP

#include "Effect.hpp"

class Effect_Fire : public Effect
{
private:
    float m_OffsetX;
    float m_OffsetY;

    uint32_t m_BufferSizeX, m_BufferSizeY;
    cHSV* buffer1;
    cHSV* buffer2;
public:
    Effect_Fire(LEDMatrix* matrix);
    ~Effect_Fire();

    void update() override;
    void render(const char* panelName) override;
private:
    uint8_t getValue(uint32_t x, uint32_t y);
    uint32_t getIndex(uint32_t x, uint32_t y);

    void checkBufferSize();
};

#endif
