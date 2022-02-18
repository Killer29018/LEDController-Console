#include "FreeType.hpp"

#include "../Panels/Logger.hpp"

std::unordered_map<char, Character*> FreeType::m_Characters;

void FreeType::loadFont(const char* filePath, uint32_t pixelSize)
{
    FT_Library library;
    FT_Face face;
    FT_Error error = FT_Init_FreeType(&library);

    if (error)
    {
        Logger::log(LoggerType::LOG_ERROR, "FreeType failed to initialize: %d", error);
        exit(error);
    }

    error = FT_New_Face(library, filePath, 0, &face);

    if (error)
    {
        Logger::log(LoggerType::LOG_ERROR, "Failed to initialize face");
        exit(error);
    }

    error = FT_Set_Pixel_Sizes(face, 0, pixelSize);

    if (error)
    {
        Logger::log(LoggerType::LOG_ERROR, "Failed to set pixel size");
        exit(error);
    }

    Character* character = nullptr;
    for (uint8_t i = 0; i < 128; i++)
    {
        error = FT_Load_Char(face, i, FT_LOAD_RENDER);

        if (error)
        {
            Logger::log(LoggerType::LOG_ERROR, "Failed to load character: %d", i);
            exit(error);
        }

        character = new Character(face->glyph->bitmap.width, face->glyph->bitmap.rows,
                            face->glyph->bitmap_left,  face->glyph->bitmap_top, 
                            face->glyph->advance.x,
                            face->glyph->bitmap);
    
        // m_Characters[i] = character;
        m_Characters.insert(std::pair<uint8_t, Character*>(i, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(library);
}

    // error = FT_New_Face(library, "../../res/fonts/arial.ttf", 0, &face);

    // if (error == FT_Err_Unknown_File_Format)
    // {
    //     Logger::log(LoggerType::LOG_ERROR, "FONT: Incorrect file type: %d", error);
    // }
    // else if (error)
    // {
    //     Logger::log(LoggerType::LOG_ERROR, "FONT: Failed to open: %d", error);
    // }

    // error = FT_Set_Pixel_Sizes(face, 0, 16); // 16 Pixels Wide and Tall

    // for (int i = 0; i < 256; i++)
    // {
    //     error = FT_Load_Char(face, i, FT_LOAD_RENDER);
    // }

    // error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
// }

// void FreeType::loadFont(const char* filePath)
// {

// }
