#include "../EffectHeaders/Effect_SolidColour.hpp"

Effect_SolidColour::Effect_SolidColour() 
    : Effect(EffectEnum::SOLID_COLOUR) {}

Effect_SolidColour::~Effect_SolidColour() {}

void Effect_SolidColour::update(LEDMatrix* matrix, float dt)
{
    m_DeltaTotal += dt;
    if (m_DeltaTotal >= (1.0 / (float)m_FPS))
    {
        matrix->fillSolid(m_PrimaryColour);
        m_DeltaTotal = 0;
    }
}

void Effect_SolidColour::render(const char* panelName)
{
    if (ImGui::Begin(panelName))
    {
        ImGui::End();
    }
}

