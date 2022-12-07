#include "EffectManager.hpp"

#include "Utils/Logger.hpp"

float EffectManager::dt = 0.0f;

EffectManager::~EffectManager()
{
    if (m_CurrentEffect)
        delete m_CurrentEffect;
}

void EffectManager::init(LEDMatrix& matrix)
{
    m_Matrix = &matrix;

    m_CurrentEffect = new Effect_None(m_Matrix);
}

void EffectManager::setEffect(EffectEnum effect)
{
    const std::lock_guard<std::mutex> lock(m_EffectMutex);

    m_CurrentEnum = effect;

    delete m_CurrentEffect;

    switch (effect)
    {
    case EffectEnum::NONE:              m_CurrentEffect = new Effect_None(m_Matrix); break;
    case EffectEnum::RAINBOW:           m_CurrentEffect = new Effect_Rainbow(m_Matrix); break;
    case EffectEnum::GLITTER:           m_CurrentEffect = new Effect_Glitter(m_Matrix); break;
    case EffectEnum::PLASMA:            m_CurrentEffect = new Effect_Plasma(m_Matrix); break;
    case EffectEnum::METABALLS:         m_CurrentEffect = new Effect_Metaballs(m_Matrix); break;
    case EffectEnum::FIREWORKS:         m_CurrentEffect = new Effect_Fireworks(m_Matrix); break;
    case EffectEnum::FALLING_RAIN:      m_CurrentEffect = new Effect_FallingRain(m_Matrix); break;
    case EffectEnum::FADING_PIXELS:     m_CurrentEffect = new Effect_FadingPixels(m_Matrix); break;
    case EffectEnum::FIRE:              m_CurrentEffect = new Effect_Fire(m_Matrix); break;
    case EffectEnum::SNAKE:             m_CurrentEffect = new Effect_Snake(m_Matrix); break;

    default:
        Logger::log(LoggerType::LOG_WARN, "Unreachable, Effect Enum added but not new Effect");
    }

    Logger::log(LoggerType::LOG_INFO, "Changed effect to %s", EffectName[static_cast<int>(effect)]);
}

void EffectManager::render()
{
//     if (ImGui::Begin("Effects"))
//     {
//         ImGui::PushItemWidth(-1);

//         { // Colour Palette Combo
//             uint32_t currentPaletteEnum = m_Matrix->getPalette();
//             const char* currentPalette = Palettes::PaletteNames.at(currentPaletteEnum);

//             ImGui::Text("Current Palette:");
//             if (ImGui::BeginCombo("##Colour Palette", currentPalette, ImGuiComboFlags_HeightLarge))
//             {
//                 for (uint32_t n = 0; n < Palettes::PaletteNames.size(); n++)
//                 {
//                     const bool isSelected = (currentPaletteEnum == n);
//                     if (ImGui::Selectable(Palettes::PaletteNames.at(n), isSelected))
//                     {
//                         m_Matrix->setPalette((Palettes::PaletteEnum)n);
//                         Logger::log(LoggerType::LOG_INFO, "Changed Palette to %s", Palettes::PaletteNames.at(n));
//                     }

//                     if (isSelected)
//                         ImGui::SetItemDefaultFocus();
//                 }
//                 ImGui::EndCombo();
//             }
//         }

//         { // Effect Combo
//             uint32_t intEnum = static_cast<int>(m_CurrentEnum);
//             const char* currentItem = EffectName.at(intEnum);

//             ImGui::Text("Current Effect:");
//             if (ImGui::BeginCombo("##EffectCombo", currentItem, ImGuiComboFlags_HeightLarge))
//             {
//                 for (uint32_t n = 0; n < EffectName.size(); n++)
//                 {
//                     const bool isSelected = (intEnum == n);
//                     if (ImGui::Selectable(EffectName.at(n), isSelected))
//                     {
//                         m_CurrentEnum = static_cast<EffectEnum>(n);
//                         setEffect(m_CurrentEnum);
//                     }

//                     if (isSelected)
//                         ImGui::SetItemDefaultFocus();
//                 }
//                 ImGui::EndCombo();
//             }
//         }


//         // Effect Speed
//         ImGui::Text("Speed");
//         uint16_t fps = m_CurrentEffect->getFPS();
//         int min = 0, max = 100;
//         ImGui::SliderScalar("##EFFECT_FPS", ImGuiDataType_U16, &fps, &min, &max, "%u");
//         m_CurrentEffect->setFPS(fps);

//         { // Colour
//             ImGui::Text("Primary Colour");
//             cHSV primaryColour = m_CurrentEffect->getPrimaryColour();
//             uint8_t paletteHue = getHueFromPalette(m_Matrix->getPalette(), primaryColour.h);
//             cRGB paletteColour = cHSV(paletteHue, primaryColour.s, primaryColour.v);

//             ImVec4 imHSVColour = ImVec4(primaryColour.h / 255.0f, primaryColour.s / 255.0f, primaryColour.v / 255.0f, 0.0f);
//             ImVec4 imPaletteColour = ImVec4(paletteColour.r / 255.0f, paletteColour.g / 255.0f, paletteColour.b / 255.0f, 0.0f);

//             ImGuiColorEditFlags flags = ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoSmallPreview;

//             float width = ImGui::GetContentRegionAvail().x;
//             width = std::min(80.0f, width);
//             ImGui::ColorButton("##ColourDisplay", imPaletteColour, ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_NoTooltip, ImVec2(width, width));

//             ImGui::ColorEdit3("##HSVPicker", (float*)&imHSVColour, flags | ImGuiColorEditFlags_InputHSV | ImGuiColorEditFlags_DisplayHSV);

//             primaryColour.h = imHSVColour.x * 255;
//             primaryColour.s = imHSVColour.y * 255;
//             primaryColour.v = imHSVColour.z * 255;

//             m_CurrentEffect->setPrimaryColour(primaryColour);
//         }

//         ImGui::PopItemWidth();
//     }
//     ImGui::End();

//     if (ImGui::Begin(m_EffectSettings))
//     {
//     }
//     ImGui::End();

    updateEffect();
}

void EffectManager::updateEffect()
{
    const std::lock_guard<std::mutex> lock(m_EffectMutex);
    if (!m_CurrentEffect) return;

    m_DeltaTotal += dt;

    if (m_DeltaTotal >= (1.0 / (float)m_CurrentEffect->getFPS()))
    {
        m_CurrentEffect->update();
        m_DeltaTotal = 0.0f;
    }

    m_CurrentEffect->render();
}
