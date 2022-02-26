#include "../EffectHeaders/Effect_Text.hpp"

#include "../../System/FreeType.hpp"

Effect_Text::Effect_Text(LEDMatrix* matrix)
    : Effect(EffectEnum::TEXT, matrix)
{
    m_OffsetX = 0;
    m_OffsetY = m_Matrix->getRows() / 2;

    m_LengthX = 0;

    m_CurrentDirection = TextDirection::RIGHT_LEFT;

    m_Scroll = true;
    if (m_Scroll)
        m_CurrentScroll = m_Matrix->getColumns() + 1;
    else
        m_CurrentScroll = 0;

    m_AnimateHue = false;
    m_DeltaHue = 1;
    m_HueOffset = 0;
    m_MaxCount = 10;
}

Effect_Text::~Effect_Text()
{

}

void Effect_Text::update()
{
    m_Matrix->fillSolid({ 0, 0, 0 });
    cHSV colour = m_PrimaryColour;

    if (m_AnimateHue) colour.h += m_HueOffset;

    static Character* character;

    static int32_t currentX;
    static int32_t currentY;
    currentX = m_OffsetX;
    currentY = m_OffsetY;

    if (m_Scroll) 
    {
        switch (m_CurrentDirection)
        {
            case TextDirection::RIGHT_LEFT:
            case TextDirection::LEFT_RIGHT:
                currentX += m_CurrentScroll; break;

            case TextDirection::BOTTOM_TOP:
            case TextDirection::TOP_BOTTOM:
                currentY += m_CurrentScroll; break;
        }
    }

    int32_t x, y, index;
    uint8_t value;
    
    bool solidColour = true;
    bool render = false;
    if (m_Text.size() <= 0) return;

    for (char ch : m_Text)
    {
        character = FreeType::getCharacter(ch);

        for (int32_t r = 0; r < character->sizeY; r++)
        {
            for (int32_t c = 0; c < character->sizeX; c++)
            {
                x = currentX - character->bearingX + c;
                if (x < 0 || x > (int32_t)m_Matrix->getColumns() - 1) continue;

                y = currentY - character->bearingY+ r;
                if (y < 0 || y > (int32_t)m_Matrix->getRows() - 1) continue;

                index = (r * character->sizeX) + c;
                value = character->data[index];

                if (value > 1 && solidColour) value = 255;
                colour.v = value;
                render = true;
                m_Matrix->setLED(x, y, colour);
            }
        }
        currentX += (character->advance >> 6) + 1;
    }

    m_LengthX = std::max(currentX, m_LengthX);

    if (!render)
    {
        switch (m_CurrentDirection)
        {
            case TextDirection::RIGHT_LEFT:
                if (currentX < 0)
                    m_CurrentScroll = m_Matrix->getColumns() + 1 - m_OffsetX;
                break;
            case TextDirection::LEFT_RIGHT:
                if (currentX > (int32_t)m_Matrix->getColumns())
                    m_CurrentScroll = -m_LengthX - m_OffsetX;
                break;
            case TextDirection::TOP_BOTTOM:
                if (currentY > 0)
                    m_CurrentScroll = -(FreeType::getMaxBelow()) - m_OffsetY;
                break;
            case TextDirection::BOTTOM_TOP:
                if (currentY < 0)
                    m_CurrentScroll = (int32_t)m_Matrix->getRows() + FreeType::getMaxAbove() - m_OffsetY;
                // m_CurrentScroll = m_Matrix->getColumns() + 1 - m_OffsetX;
                break;
         }
    }
    else if (m_AnimateHue)
    {
        m_CurrentCount++;
        if (m_CurrentCount >= m_MaxCount)
        {
            m_HueOffset += m_DeltaHue;
            m_CurrentCount = 0;
        }
    }

    if (m_Scroll) 
    {
        switch  (m_CurrentDirection)
        {
            case TextDirection::RIGHT_LEFT:
            case TextDirection::BOTTOM_TOP:
                m_CurrentScroll -= 1;
                break;
            case TextDirection::LEFT_RIGHT:
            case TextDirection::TOP_BOTTOM:
                m_CurrentScroll += 1;
                break;
        }
    }
}

void Effect_Text::render(const char* panelName)
{
    if (ImGui::Begin(panelName))
    {
        int min, max;
        ImGui::PushItemWidth(-1);

        ImGui::Text("Text: ");
        static char text[500] = "Hello, World!";
        if (ImGui::InputText("##Text", text, 500) || strcmp(text, m_Text.c_str()))
        {
            m_Text = std::string(text);
        }

        static char fontpath[500];
        static bool initial = true;
        if (initial)
        {
            strcpy(fontpath, FreeType::getCurrentFilePath());
            initial = false;
        }

        if (ImGui::InputText("##Font", fontpath, 500, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            strcpy(fontpath, FreeType::getCurrentFilePath());
        }

        ImGui::Text("Font Size");
        uint32_t fontSize = FreeType::getFontSize();
        min = 5;
        max = 40;
        if (ImGui::SliderScalar("##FontSize", ImGuiDataType_U32, &fontSize, &min, &max, "%u"))
            FreeType::setFontSize(fontSize);

        ImGui::Text("Offset X");
        min = -m_LengthX; max = m_Matrix->getColumns();
        ImGui::SliderScalar("##OffsetX", ImGuiDataType_S32, &m_OffsetX, &min, &max, "%d");

        ImGui::Text("Offset Y");
        min = -FreeType::getMaxBelow(); max = m_Matrix->getRows() + FreeType::getMaxAbove();
        ImGui::SliderScalar("##OffsetY", ImGuiDataType_S32, &m_OffsetY, &min, &max, "%d");

        ImGui::Text("Scroll");
        if (ImGui::Checkbox("##Scroll", &m_Scroll))
        {
            if (m_Scroll)
                m_CurrentScroll = 0;
            else
                m_OffsetX += m_CurrentScroll;
        }
        
        if (m_Scroll)
        {
            uint32_t currentDirection = static_cast<uint32_t>(m_CurrentDirection);
            const char* currentDirectionString = TextDirectionString.at(currentDirection);

            ImGui::Text("Current Direction");
            if (ImGui::BeginCombo("##Current Direction", currentDirectionString, ImGuiComboFlags_HeightLarge))
            {
                for (uint32_t n = 0; n < TextDirectionString.size(); n++)
                {
                    const bool isSelected = (currentDirection == n);
                    if (ImGui::Selectable(TextDirectionString.at(n), isSelected))
                    {
                        m_CurrentDirection = static_cast<TextDirection>(n);
                        Logger::log(LoggerType::LOG_INFO, "Changed Text direction %s", TextDirectionString.at(n));
                    }

                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
        }

        ImGui::Text("Animate Hue");
        ImGui::Checkbox("##AnimateHue", &m_AnimateHue);

        if (m_AnimateHue)
        {
            ImGui::Text("Delta Hue");
            min = 0;
            max = 255;
            ImGui::SliderScalar("##DeltaHue", ImGuiDataType_U8, &m_DeltaHue, &min, &max, "%u");

            ImGui::Text("Hue Update Speed");
            uint8_t value = max - m_MaxCount;
            ImGui::SliderScalar("##HueUpdate", ImGuiDataType_U8, &value, &min, &max, "%u");
            m_MaxCount = max - value;
        }

        ImGui::PopItemWidth();
    }
    ImGui::End();
}
