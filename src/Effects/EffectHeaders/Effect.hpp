#ifndef EFFECT_EFFECT_HPP
#define EFFECT_EFFECT_HPP

#include "../../LED/LEDMatrix.hpp"
#include "../EffectsExtra.hpp"

#include "../../ImGui/ImGuiPanel.hpp" // Get Imgui Functions

class Effect
{
public:
    EffectEnum effectEnum;
protected:
    float m_DeltaTotal;
    uint8_t m_FPS;
    cRGB m_PrimaryColour;
public:
    Effect(EffectEnum effectEnum)
        : effectEnum(effectEnum), m_FPS(40), m_DeltaTotal(0.0f),
        m_PrimaryColour(cHSV(0, 0xFF, 0xFF)) {}

    virtual ~Effect() {}

    virtual void update(LEDMatrix* matrix, float dt) {}
    virtual void render(const char* panelName) {}

    uint8_t& getFPS() { return m_FPS; }
    void setFPS(uint8_t fps) { m_FPS = fps; }

    cRGB getPrimaryColour() { return m_PrimaryColour; }
    void setPrimaryColour(cRGB colour) { m_PrimaryColour = colour; }
};

#endif
