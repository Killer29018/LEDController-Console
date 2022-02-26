#include "FreeType.hpp"

#include "../Panels/Logger.hpp"

#include <filesystem>

FT_Library FreeType::m_Library;
FT_Face FreeType::m_Face;
uint32_t FreeType::m_FontSize;
int32_t FreeType::m_MaxBelow;
int32_t FreeType::m_MaxAbove;
std::unordered_map<char, Character*> FreeType::m_Characters;
char* FreeType::m_CurrentFont;

FreeType::FreeType() { }

FreeType::~FreeType() { }

void FreeType::init()
{
    m_CurrentFont = (char*)malloc(sizeof(char));

    FT_Error error = FT_Init_FreeType(&m_Library);

    if (error)
    {
        Logger::log(LoggerType::LOG_ERROR, "FreeType failed to initialize: %d", error);
        exit(error);
    }
}

bool FreeType::loadFont(const char* filePath, uint32_t fontSize)
{
    std::filesystem::path path = std::filesystem::path(filePath);
    if (path.extension().generic_string() != ".ttf")
    {
        Logger::log(LoggerType::LOG_WARN, "Only .ttf are currently supported");
        return false;
    }
    if (!std::filesystem::exists(path))
    {
        Logger::log(LoggerType::LOG_WARN, "File does not exist");
        return false;
    }

    m_CurrentFont = (char*)realloc(m_CurrentFont, strlen(filePath) * sizeof(char));
    strcpy(m_CurrentFont, filePath);

    FT_Error error = FT_New_Face(m_Library, filePath, 0, &m_Face);

    if (error)
    {
        Logger::log(LoggerType::LOG_ERROR, "Failed to initialize m_Face");
        return false;
    }

    error = FT_Set_Pixel_Sizes(m_Face, 0, fontSize);

    if (error)
    {
        Logger::log(LoggerType::LOG_ERROR, "Failed to set pixel size");
        return false;
    }
    m_FontSize = fontSize;

    return loadCharacters();
}

bool FreeType::setFontSize(uint32_t fontSize)
{
    FT_Error error = FT_Set_Pixel_Sizes(m_Face, 0, fontSize);

    if (error)
    {
        Logger::log(LoggerType::LOG_ERROR, "Failed to set pixel size");
        return false;
        exit(error);
    }
    m_FontSize = fontSize;

    return loadCharacters();
}

bool FreeType::loadCharacters()
{
    FT_Error error;
    Character* character = nullptr;
    for (uint8_t i = 0; i < 128; i++)
    {
        error = FT_Load_Char(m_Face, i, FT_LOAD_RENDER);

        if (error)
        {
            Logger::log(LoggerType::LOG_ERROR, "Failed to load character: %d", i);
            return false;
        }

        m_MaxAbove = std::max(m_MaxAbove, (int32_t)m_Face->glyph->bitmap_top);
        m_MaxBelow = std::max(m_MaxBelow, (int32_t)(m_Face->glyph->bitmap.rows - m_Face->glyph->bitmap_top));

        character = new Character(m_Face->glyph->bitmap.width, m_Face->glyph->bitmap.rows,
                            m_Face->glyph->bitmap_left,  m_Face->glyph->bitmap_top, 
                            m_Face->glyph->advance.x,
                            m_Face->glyph->bitmap);
    
        m_Characters[i] = character;
        // m_Characters.insert(std::pair<uint8_t, Character*>(i, character));
    }

    return true;
}
