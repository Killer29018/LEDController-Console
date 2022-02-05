#include "EffectManager.hpp"

#include "KRE/KRE.hpp"

EffectManager::~EffectManager()
{
    delete m_CurrentEffect;
}

void EffectManager::init(LEDMatrix& matrix)
{
    m_Matrix = &matrix;

    m_CurrentEffect = new Effect_None();
}

void EffectManager::setEffect(EffectEnum effect)
{
    m_CurrentEnum = effect;

    delete m_CurrentEffect;

    switch (effect)
    {
    case EffectEnum::NONE:          m_CurrentEffect = new Effect_None(); break;
    case EffectEnum::SOLID_COLOUR:  m_CurrentEffect = new Effect_SolidColour(); break;
    case EffectEnum::RAINBOW:       m_CurrentEffect = new Effect_Rainbow(); break;
    case EffectEnum::GLITTER:       m_CurrentEffect = new Effect_Glitter(); break;
    }
}

void EffectManager::renderImGui()
{
    if (ImGui::Begin("Effects"))
    {
        ImGui::PushItemWidth(-1);

        { // Selectable
            int intEnum = static_cast<int>(m_CurrentEnum);
            const char* currentItem = EffectName[intEnum];
            if (ImGui::BeginCombo("##EffectCombo", currentItem, 0))
            {
                for (int n = 0; n < EffectName.size(); n++)
                {
                    const bool isSelected = (intEnum == n);
                    if (ImGui::Selectable(EffectName[n], isSelected))
                    {
                        m_CurrentEnum = static_cast<EffectEnum>(n);
                        setEffect(m_CurrentEnum);
                    }

                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
        }

        ImGui::Text("Speed");
        uint8_t fps = m_CurrentEffect->getFPS();
        int min = 0, max = 100;
        ImGui::SliderScalar("##EFFECT_FPS", ImGuiDataType_U8, &fps, &min, &max, "%u");
        m_CurrentEffect->setFPS(fps);

        ImGui::Text("Primary Colour");
        { // Colour
            cRGB colour = m_CurrentEffect->getPrimaryColour();
            ImVec4 imColour = ImVec4(colour.r / 255.0f, colour.g / 255.0f, colour.b / 255.0f, 0.0f);

            static ImGuiColorEditFlags colourFlags = ImGuiColorEditFlags_NoAlpha | 
                ImGuiColorEditFlags_PickerHueBar;

            ImGui::ColorPicker4("##PrimaryColourPicker", (float*)&imColour, colourFlags, NULL);
            colour = cRGB(imColour.x * 255.0f, imColour.y * 255.0f, imColour.z * 255.0f);
            m_CurrentEffect->setPrimaryColour(colour);
        }

        ImGui::PopItemWidth();

        ImGui::End();
    }

    if (ImGui::Begin(m_EffectSettings))
    {
        ImGui::End();
    }

    updateEffect();
}

void EffectManager::updateEffect()
{
    if (!m_CurrentEffect) return;

    m_CurrentEffect->update(m_Matrix, KRE::Clock::deltaTime);

    m_CurrentEffect->render("Effect Settings");
}
