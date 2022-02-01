#ifndef EFFECTS_EFFECT_MANAGER_HPP
#define EFFECTS_EFFECT_MANAGER_HPP

#include "../ImguiWindow.hpp"
#include "../LEDMatrix.hpp"
#include "EffectList/Effect.hpp"

#include "EffectList.hpp"

#include <vector>


class EffectManager : public ImguiWindow
{
public:
private:
    LEDMatrix* m_Matrix;

    Effect* m_CurrentEffect = NULL;

    const char* m_EffectSettings = "Effect Settings";
public:
    EffectManager() = default;
    ~EffectManager();

    void init(LEDMatrix& matrix);
    void setEffect(EffectList effect);

    void renderImgui();
private:
    void updateEffect();
};

#endif
