#ifndef EFFECTS_EFFECT_MANAGER_HPP
#define EFFECTS_EFFECT_MANAGER_HPP

#include "../ImguiWindow.hpp"
#include "../LEDMatrix.hpp"
#include "Effects.hpp"

#include "EffectList/AllEffects.hpp"

class EffectManager : public ImguiWindow
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

    void renderImgui();
private:
    void updateEffect();
};

#endif
