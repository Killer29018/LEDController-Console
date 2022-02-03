#ifndef EFFECT_ALLEFFECTS_HPP
#define EFFECT_ALLEFFECTS_HPP

#include "../../LEDMatrix.hpp"
#include "../Effects.hpp"
#include "../../ImguiWindow.hpp" // Get Imgui Functions

class Effect
{
public:
    EffectEnum effectEnum;
protected:
    float m_DeltaTotal;
    int m_FPS;
public:
    Effect(EffectEnum effectEnum)
        : effectEnum(effectEnum), m_FPS(40), m_DeltaTotal(0.0f) {}
    virtual ~Effect() {}

    virtual void updateEffect(LEDMatrix* matrix, float dt) {}
    virtual void render(const char* panelName) {}

    int getFPS() { return m_FPS; }
    void setFPS(int fps) { m_FPS = fps; }
};

class Effect_None : public Effect
{
public:
    Effect_None();
    ~Effect_None();

    void updateEffect(LEDMatrix* matrix, float dt) override;
    void render(const char* panelName) override;
};

class Effect_SolidColour : public Effect
{
private:
    cRGB m_Colour;
public:
    Effect_SolidColour();
    ~Effect_SolidColour();

    void updateEffect(LEDMatrix* matrix, float dt) override;
    void render(const char* panelName) override;
};

class Effect_Rainbow : public Effect
{
private:
    uint8_t deltaHue;
public:
    Effect_Rainbow();
    ~Effect_Rainbow();

    void updateEffect(LEDMatrix* matrix, float dt) override;
    void render(const char* panelName) override;
};

#endif
