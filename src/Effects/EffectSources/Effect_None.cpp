#include "../EffectHeaders/Effect_None.hpp"

Effect_None::Effect_None(LEDMatrix* matrix)
    : Effect(EffectEnum::NONE, matrix) {}

Effect_None::~Effect_None() {}

void Effect_None::update()
{
    m_Matrix->fillSolid({ 0, 0, 0 });
}

// void Effect_None::render(const char* panelName)
// {
//     if (ImGui::Begin(panelName))
//     {
//     }
//     ImGui::End();
// }
