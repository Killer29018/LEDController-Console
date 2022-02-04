#ifndef EFFECT_NONE_HPP
#define EFFECT_NONE_HPP

#include "Effect.hpp"

class Effect_None : public Effect
{
public:
    Effect_None();
    ~Effect_None();

    void update(LEDMatrix* matrix, float dt) override;
    void render(const char* panelName) override;
};

#endif
