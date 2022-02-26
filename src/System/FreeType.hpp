#ifndef SYSTEM_FREE_TYPE_HPP
#define SYSTEM_FREE_TYPE_HPP

#include <ft2build.h>
#include FT_FREETYPE_H

#include <unordered_map>

#include <cstdint>

struct Character
{
    int32_t sizeX;
    int32_t sizeY;

    int32_t bearingX;
    int32_t bearingY;
    uint32_t advance;

    uint8_t* data;

    Character() : sizeX(0), sizeY(0), bearingX(0), bearingY(0), advance(0), data(nullptr) {}

    Character(uint32_t sizeX, uint32_t sizeY, uint32_t bearingX, uint32_t bearingY, uint32_t advance, FT_Bitmap& bitmap)
        : sizeX(sizeX), sizeY(sizeY), bearingX(bearingX), bearingY(bearingY), advance(advance)
    {
        uint32_t size = bitmap.rows * bitmap.width;
        data = (uint8_t*)malloc(size * sizeof(uint8_t));
        memcpy(data, bitmap.buffer, size * sizeof(uint8_t));
    }

    ~Character() { if (data) delete[] data; }
};

class FreeType
{
public:
private:
    static FT_Library m_Library;
    static FT_Face m_Face;
    static std::unordered_map<char, Character*> m_Characters;
    static uint32_t m_FontSize;
    static int32_t m_MaxAbove;
    static int32_t m_MaxBelow;
    static char* m_CurrentFont;
public:
    static void init();
    static bool loadFont(const char* filePath, uint32_t pixelSize = 8);

    static bool setFontSize(uint32_t fontSize);
    static uint32_t getFontSize() { return m_FontSize; }

    static int32_t getMaxAbove() { return m_MaxAbove; }
    static int32_t getMaxBelow() { return m_MaxBelow; }

    static Character* getCharacter(char c) { return m_Characters[c]; }

    static const char* getCurrentFilePath() { return m_CurrentFont; }

private:
    FreeType();
    ~FreeType();

    static bool loadCharacters();
};

#endif
