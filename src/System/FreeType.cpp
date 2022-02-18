#include "FreeType.hpp"

#include "../Panels/Logger.hpp"

FT_Library FreeType::m_Library;
FT_Face FreeType::m_Face;
uint32_t FreeType::m_FontSize;
std::unordered_map<char, Character*> FreeType::m_Characters;

FreeType::FreeType() { }

FreeType::~FreeType() { }

void FreeType::init()
{
    FT_Error error = FT_Init_FreeType(&m_Library);

    if (error)
    {
        Logger::log(LoggerType::LOG_ERROR, "FreeType failed to initialize: %d", error);
        exit(error);
    }
}

void FreeType::loadFont(const char* filePath, uint32_t fontSize)
{
    FT_Error error = FT_New_Face(m_Library, filePath, 0, &m_Face);

    if (error)
    {
        Logger::log(LoggerType::LOG_ERROR, "Failed to initialize m_Face");
        exit(error);
    }

    error = FT_Set_Pixel_Sizes(m_Face, 0, fontSize);

    if (error)
    {
        Logger::log(LoggerType::LOG_ERROR, "Failed to set pixel size");
        exit(error);
    }
    m_FontSize = fontSize;

    loadCharacters();
}

void FreeType::setFontSize(uint32_t fontSize)
{
    FT_Error error = FT_Set_Pixel_Sizes(m_Face, 0, fontSize);

    if (error)
    {
        Logger::log(LoggerType::LOG_ERROR, "Failed to set pixel size");
        exit(error);
    }
    m_FontSize = fontSize;

    loadCharacters();
}

void FreeType::loadCharacters()
{
    FT_Error error;
    Character* character = nullptr;
    for (uint8_t i = 0; i < 128; i++)
    {
        error = FT_Load_Char(m_Face, i, FT_LOAD_RENDER);

        if (error)
        {
            Logger::log(LoggerType::LOG_ERROR, "Failed to load character: %d", i);
            exit(error);
        }

        character = new Character(m_Face->glyph->bitmap.width, m_Face->glyph->bitmap.rows,
                            m_Face->glyph->bitmap_left,  m_Face->glyph->bitmap_top, 
                            m_Face->glyph->advance.x,
                            m_Face->glyph->bitmap);
    
        // m_Characters[i] = character;
        m_Characters.insert(std::pair<uint8_t, Character*>(i, character));
    }
}
