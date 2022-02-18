#include "../EffectHeaders/Effect_Text.hpp"

#include "../../System/FreeType.hpp"

Effect_Text::Effect_Text(LEDMatrix* matrix)
    : Effect(EffectEnum::TEXT, matrix)
{
    m_OffsetX = 0;
    m_OffsetY = 10;
}

Effect_Text::~Effect_Text()
{

}

void Effect_Text::update()
{
    m_Matrix->fillSolid({ 0, 0, 0 });
    cHSV colour = m_PrimaryColour;

    static Character* character;

    static uint32_t currentX;
    static uint32_t currentY;
    currentX = m_OffsetX;
    currentY = m_OffsetY;

    int32_t x, y, index;
    uint8_t value;
    
    bool solidColour = true;

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
                m_Matrix->setLED(x, y, colour);
            }
        }
        currentX += (character->advance >> 6) + 1;
    }
}

void Effect_Text::render(const char* panelName)
{
    if (ImGui::Begin(panelName))
    {
        static char text[500];
        if (ImGui::InputText("##Text", text, 500))
        {
            m_Text = std::string(text);
        }
            
        ImGui::End();
    }
}
