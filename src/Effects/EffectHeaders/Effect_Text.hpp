#ifndef EFFECT_EFFECT_TEXT_HPP
#define EFFECT_EFFECT_TEXT_HPP

#include "Effect.hpp"

enum class TextDirection
{
    RIGHT_LEFT,
    LEFT_RIGHT,
    TOP_BOTTOM,
    BOTTOM_TOP,
};

const std::vector<const char*> TextDirectionString = {
    "Right -> Left",
    "Left -> Right",
    "Top -> Bottom",
    "Bottom -> Top",
};

class Effect_Text : public Effect
{
private:
    int32_t m_OffsetX;
    int32_t m_OffsetY;

    int32_t m_CurrentScroll;

    bool m_Scroll;

    int32_t m_LengthX;

    std::string m_Text;

    TextDirection m_CurrentDirection;
public:
    Effect_Text(LEDMatrix* matrix);
    ~Effect_Text() override;

    void update() override;
    void render(const char* panelName) override;
};

#endif
