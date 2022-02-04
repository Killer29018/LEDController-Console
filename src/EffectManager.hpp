#ifndef EFFECTS_EFFECT_MANAGER_HPP
#define EFFECTS_EFFECT_MANAGER_HPP

#include "ImGui/ImGuiPanel.hpp"
#include "LED/LEDMatrix.hpp"
#include "Effects/Effects.hpp"
#include "Effects/EffectsExtra.hpp"

class EffectManager : public ImGuiPanel
{
public:
private:
    LEDMatrix* m_Matrix;

    Effect* m_CurrentEffect = NULL;
    EffectEnum m_CurrentEnum;

    const char* m_EffectSettings = "Effect Settings";
public:
    EffectManager() = default;
    ~EffectManager();

    void init(LEDMatrix& matrix);
    void setEffect(EffectEnum effect);

    void renderImGui();
private:
    void updateEffect();
};

#endif
