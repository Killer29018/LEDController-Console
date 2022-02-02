#ifndef EFFECT_SOLID_COLOUR_HPP
#define EFFECT_SOLID_COLOUR_HPP

#include "Effect.hpp"

class Effect_SolidColour : public Effect
{
private:
    cRGB m_Colour;
public:
    Effect_SolidColour() 
        : Effect(EffectEnum::SOLID_COLOUR) 
    {
        m_Colour = cRGB(255, 0, 0);
    }
    ~Effect_SolidColour() {}

    void updateEffect(LEDMatrix* matrix, float dt) override
    {
        m_DeltaTotal += dt;
        if (m_DeltaTotal >= (1.0 / (float)m_FPS))
        {
            matrix->fillSolid(m_Colour);
            m_DeltaTotal = 0;
        }
    }

    void render(const char* panelName) override
    {
        if (ImGui::Begin(panelName))
        {
            ImVec4 colour = ImVec4(m_Colour.r / 255.0f, m_Colour.g / 255.0f, m_Colour.b / 255.0f, 0.0f);
            static ImGuiColorEditFlags flags = ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_PickerHueBar;

            ImGui::PushItemWidth(-1);
            ImGui::ColorPicker4("##ColourPicker", (float*)&colour, flags, NULL);
            m_Colour = cRGB(colour.x * 255.0f, colour.y * 255.0f, colour.z * 255.0f);
            ImGui::PopItemWidth();
            ImGui::End();
        }
    }
};

#endif
