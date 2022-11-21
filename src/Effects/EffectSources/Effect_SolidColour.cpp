#include "../EffectHeaders/Effect_SolidColour.hpp"

Effect_SolidColour::Effect_SolidColour(LEDMatrix* matrix) 
    : Effect(EffectEnum::SOLID_COLOUR, matrix) {}

Effect_SolidColour::~Effect_SolidColour() {}

void Effect_SolidColour::update()
{
    m_Matrix->fillSolid(m_PrimaryColour);
}

// void Effect_SolidColour::render(const char* panelName)
// {
//     if (ImGui::Begin(panelName))
//     {
//     }
//     ImGui::End();
// }

