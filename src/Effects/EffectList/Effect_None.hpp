#ifndef EFFECT_NONE_HPP
#define EFFECT_NONE_HPP

#include "Effect.hpp"

class Effect_None : public Effect
{
public:
    Effect_None() : Effect(EffectList::NONE) {}
    ~Effect_None() {}

    void updateEffect(LEDMatrix* matrix, float dt) override
    {
        m_DeltaTotal += dt;
        if (m_DeltaTotal >= (1.0 / (float)m_FPS)) { m_DeltaTotal = 0; }
    }

    void render(const char* panelName) override
    {
        if (ImGui::Begin(panelName))
        {
        }
        ImGui::End();
    }
};

#endif
