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
    }
}

void EffectManager::renderImgui()
{
    if (ImGui::Begin("Effects"))
    {
        ImGui::PushItemWidth(-1);
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

        ImGui::Text("Speed");
        int fps = m_CurrentEffect->getFPS();
        ImGui::SliderInt("##EFFECT_FPS", &fps, 1, 100);
        m_CurrentEffect->setFPS(fps);

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

    m_CurrentEffect->updateEffect(m_Matrix, KRE::Clock::deltaTime);

    m_CurrentEffect->render("Effect Settings");
}
