#include "EffectManager.hpp"

#include "EffectList/AllEffects.hpp"

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

void EffectManager::setEffect(EffectList effect)
{
    delete m_CurrentEffect;

    switch (effect)
    {
    case EffectList::NONE:      m_CurrentEffect = new Effect_None(); break;
    case EffectList::RAINBOW:   m_CurrentEffect = new Effect_Rainbow(); break;
    }
}

void EffectManager::renderImgui()
{
    if (ImGui::Begin("Effects"))
    {
    }
    ImGui::End();

    if (ImGui::Begin(m_EffectSettings))
    {
        ImGui::Text("Speed");
        int fps = m_CurrentEffect->getFPS();
        ImGui::SliderInt("##EFFECT_FPS", &fps, 1, 100);
        m_CurrentEffect->setFPS(fps);
    }
    ImGui::End();

    updateEffect();
}

void EffectManager::updateEffect()
{
    if (!m_CurrentEffect) return;

    m_CurrentEffect->updateEffect(m_Matrix, KRE::Clock::deltaTime);


    m_CurrentEffect->render("Effect Settings");
}
