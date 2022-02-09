#include "../EffectHeaders/Effect_None.hpp"

Effect_None::Effect_None()
    : Effect(EffectEnum::NONE) {}

Effect_None::~Effect_None() {}

void Effect_None::update(LEDMatrix* matrix)
{
    matrix->fillSolid({ 0, 0, 0 });
}

void Effect_None::render(const char* panelName)
{
    if (ImGui::Begin(panelName))
    {
    }
    ImGui::End();
}
