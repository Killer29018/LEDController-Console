#include "../EffectHeaders/Effect_Text.hpp"

#include "../../System/FreeType.hpp"

Effect_Text::Effect_Text(LEDMatrix* matrix)
    : Effect(EffectEnum::TEXT, matrix)
{
    m_OffsetX = 0;
    m_OffsetY = m_Matrix->getRows() / 2;

    m_LengthX = 0;

    m_Scroll = true;
    if (m_Scroll)
        m_CurrentScroll = m_Matrix->getColumns() + 1;
    else
        m_CurrentScroll = 0;
}

Effect_Text::~Effect_Text()
{

}

void Effect_Text::update()
{
    m_Matrix->fillSolid({ 0, 0, 0 });
    cHSV colour = m_PrimaryColour;

    static Character* character;

    static int32_t currentX;
    static int32_t currentY;
    currentX = m_OffsetX;
    currentY = m_OffsetY;

    if (m_Scroll) currentX += m_CurrentScroll;

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
                if (x < 0 || x > m_Matrix->getColumns() - 1) continue;

                y = currentY - character->bearingY+ r;
                if (y < 0 || y > m_Matrix->getRows() - 1) continue;

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

    if (!render && currentX < 0)
    {
        m_CurrentScroll = m_Matrix->getColumns() + 1 - m_OffsetX;
    }

    if (m_Scroll) m_CurrentScroll -= 1;
}

void Effect_Text::render(const char* panelName)
{
    if (ImGui::Begin(panelName))
    {
        ImGui::PushItemWidth(-1);

        ImGui::Text("Text: ");
        static char text[500] = "Hello, World!";
        if (ImGui::InputText("##Text", text, 500) || strcmp(text, m_Text.c_str()))
        {
            m_Text = std::string(text);
        }

        int min, max;
        
        if (!m_Scroll)
        {
            ImGui::Text("Offset X");
            min = -m_LengthX; max = m_Matrix->getColumns();
            ImGui::SliderScalar("##OffsetX", ImGuiDataType_S32, &m_OffsetX, &min, &max, "%d");
        }

        ImGui::Text("Offset Y");
        min = 0; max = m_Matrix->getRows() + FreeType::getMaxAbove();
        ImGui::SliderScalar("##OffsetY", ImGuiDataType_S32, &m_OffsetY, &min, &max, "%d");

        ImGui::Text("Scroll");
        if (ImGui::Checkbox("##Scroll", &m_Scroll))
        {
            if (m_Scroll)
                m_CurrentScroll = 0;
            else
                m_OffsetX += m_CurrentScroll;
        }

        ImGui::Text("Font Size");
        uint32_t fontSize = FreeType::getFontSize();
        min = 5;
        max = 40;
        if (ImGui::SliderScalar("##FontSize", ImGuiDataType_U32, &fontSize, &min, &max, "%u"))
            FreeType::setFontSize(fontSize);

        ImGui::PopItemWidth();
    }
    ImGui::End();
}
