#ifndef EFFECT_NONE_HPP
#define EFFECT_NONE_HPP

#include "Effect.hpp"

class Effect_None : public Effect
{
public:
    Effect_None(LEDMatrix* matrix);
    ~Effect_None();

    void update() override;
    void render(const char* panelName) override;
};

#endif
