#include "../EffectHeaders/Effect_SolidColour.hpp"

Effect_SolidColour::Effect_SolidColour() 
    : Effect(EffectEnum::SOLID_COLOUR) {}

Effect_SolidColour::~Effect_SolidColour() {}

void Effect_SolidColour::update(LEDMatrix* matrix)
{
    matrix->fillSolid(m_PrimaryColour);
}

void Effect_SolidColour::render(const char* panelName)
{
    if (ImGui::Begin(panelName))
    {
    }
    ImGui::End();
}

