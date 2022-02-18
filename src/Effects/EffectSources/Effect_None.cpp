#include "../EffectHeaders/Effect_None.hpp"

#include "../../System/FreeType.hpp" // TEMPORARY

Effect_None::Effect_None(LEDMatrix* matrix)
    : Effect(EffectEnum::NONE, matrix) {}

Effect_None::~Effect_None() {}

void Effect_None::update()
{
    m_Matrix->fillSolid({ 0, 0, 0 });

    // TEMPORARY

    cHSV colour(0, 255, 0);
    Character* character;

    uint32_t originX = 0;
    uint32_t originY = 0;

    std::string text = "Abp";
    for (char ch : text)
    {
        character = FreeType::getCharacter(ch);

        for (int32_t r = 0; r < character->sizeY; r++)
        {
            for (int32_t c = 0; c < character->sizeX; c++)
            {
                int32_t x = originX + character->bearingX + c;
                int32_t y = originY + (character->sizeY - character->bearingY) + r;

                int32_t index = (r * character->sizeX) + c;
                uint8_t value = character->data[index];

                if (value > 1) value = 255;
                colour.v = value;

                if (x >= 0 && x < m_Matrix->getColumns() && y >= 0 && y < m_Matrix->getRows())
                    m_Matrix->setLED(x, y, colour);

            }
        }
        originX += (character->advance >> 6);
    }

    originX = 0;
    originY = 10;
    text = "ABP";
    for (char ch : text)
    {
        character = FreeType::getCharacter(ch);

        for (int32_t r = 0; r < character->sizeY; r++)
        {
            for (int32_t c = 0; c < character->sizeX; c++)
            {
                int32_t x = originX + character->bearingX + c;
                int32_t y = originY + (character->sizeY - character->bearingY) + r;

                int32_t index = (r * character->sizeX) + c;
                uint8_t value = character->data[index];

                if (value > 1) value = 255;
                colour.v = value;

                if (x >= 0 && x < m_Matrix->getColumns() && y >= 0 && y < m_Matrix->getRows())
                    m_Matrix->setLED(x, y, colour);

            }
        }
        originX += (character->advance >> 6);
    }
}

void Effect_None::render(const char* panelName)
{
    if (ImGui::Begin(panelName))
    {
    }
    ImGui::End();
}
