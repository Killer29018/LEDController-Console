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
    uint8_t m_FPS;
    cHSV m_PrimaryColour;
    LEDMatrix* m_Matrix;
public:
    Effect(EffectEnum effectEnum, LEDMatrix* matrix)
        : effectEnum(effectEnum), m_FPS(40), m_PrimaryColour(0, 255, 255),
          m_Matrix(matrix) {}

    virtual ~Effect() {}

    virtual void update() {}
    virtual void render(const char* panelName) {}

    uint8_t& getFPS() { return m_FPS; }
    void setFPS(uint8_t fps) { m_FPS = fps; }

    cHSV getPrimaryColour() { return m_PrimaryColour; }
    void setPrimaryColour(cHSV colour) { m_PrimaryColour = colour; }
};

#endif
