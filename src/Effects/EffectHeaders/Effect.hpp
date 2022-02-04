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
    int m_FPS;
    cRGB m_PrimaryColour;
public:
    Effect(EffectEnum effectEnum)
        : effectEnum(effectEnum), m_FPS(40), m_DeltaTotal(0.0f),
        m_PrimaryColour(0, 255, 255) {}

    virtual ~Effect() {}

    virtual void update(LEDMatrix* matrix, float dt) {}
    virtual void render(const char* panelName) {}

    int getFPS() { return m_FPS; }
    void setFPS(int fps) { m_FPS = fps; }

    cRGB getPrimaryColour() { return m_PrimaryColour; }
    void setPrimaryColour(cRGB colour) { m_PrimaryColour = colour; }
};

#endif
