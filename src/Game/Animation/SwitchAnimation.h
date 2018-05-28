#pragma once

#include "Animation.h"

#include "../View/BoardView.h"

namespace MinerSpeed
{

class SwitchAnimation : public Animation
{
  public:
    SwitchAnimation(King::Engine &engine);
    virtual ~SwitchAnimation();

  public:
    void Start(const MinerSpeed::BoardView::VPCellViews &cellViews);
};
} // namespace MinerSpeed
