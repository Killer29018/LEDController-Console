#ifndef EFFECT_EFFECT_TEXT_HPP
#define EFFECT_EFFECT_TEXT_HPP

#include "Effect.hpp"

class Effect_Text : public Effect
{
private:
    int16_t m_OffsetX;
    int16_t m_OffsetY;

    std::string m_Text;
public:
    Effect_Text(LEDMatrix* matrix);
    ~Effect_Text() override;

    void update() override;
    void render(const char* panelName) override;
};

#endif
