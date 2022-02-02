#ifndef EFFECTS_EFFECT_HPP
#define EFFECTS_EFFECT_HPP

#include "../../LEDMatrix.hpp"

#include "../../ImguiWindow.hpp"

#include "../Effects.hpp"

class Effect
{
public:
    EffectEnum effectEnum;
protected:
    float m_DeltaTotal;
    int m_FPS;
public:
    Effect(EffectEnum effectEnum) : effectEnum(effectEnum), m_FPS(40.0f), m_DeltaTotal(0.0f) {}
    virtual ~Effect() {}

    virtual void updateEffect(LEDMatrix* matrix, float dt) {}
    virtual void render(const char* panelName) {}

    int getFPS() { return m_FPS; }
    void setFPS(int fps) { m_FPS = fps; }
};

#endif
