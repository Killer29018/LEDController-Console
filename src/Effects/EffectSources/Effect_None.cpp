#include "../EffectHeaders/Effect_None.hpp"

Effect_None::Effect_None()
    : Effect(EffectEnum::NONE) {}

Effect_None::~Effect_None() {}

void Effect_None::update(LEDMatrix* matrix, float dt)
{
    m_DeltaTotal += dt;
    if (m_DeltaTotal >= (1.0 / (float)m_FPS))
    {
        matrix->fillSolid({ 0, 0, 0 });
        m_DeltaTotal = 0;
    }
}

void Effect_None::render(const char* panelName)
{
    if (ImGui::Begin(panelName))
    {
    }
    ImGui::End();
}
