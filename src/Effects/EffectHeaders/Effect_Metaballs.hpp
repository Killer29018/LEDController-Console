#ifndef EFFECT_EFFECT_METABALLS_HPP
#define EFFECT_EFFECT_METABALLS_HPP

#include "Effect.hpp"

struct Blob
{
    float x;
    float y;
    float velX;
    float velY;
    float accX;
    float accY;
};

class Effect_Metaballs : public Effect
{
private:
    static const uint16_t MAX_BLOBS = 2;
    Blob m_Blobs[MAX_BLOBS];
    uint8_t m_Threshold;
    uint16_t m_CurrentBlobs;
    float m_MaxSpeed;
    uint8_t m_Size;
public:
    Effect_Metaballs(LEDMatrix* matrix);
    ~Effect_Metaballs();

    void update() override;
    // void render(const char* panelName) override;
private:
    void createBlobs();
    void updateBlobs();
    void randomiseAcc(Blob& b);
    void updateVel(Blob& b);
    uint8_t getValue(int x, int y);
};

#endif
