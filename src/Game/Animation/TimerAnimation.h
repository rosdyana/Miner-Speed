#pragma once

#include "Animation.h"

namespace MinerSpeed
{

    class TimerAnimation : public Animation
    {
    public:
        TimerAnimation(King::Engine &engine);
        virtual ~TimerAnimation();

    };
} // namespace MinerSpeed