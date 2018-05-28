#pragma once

#include "Animation.h"

#include "../View/BoardView.h"

namespace MinerSpeed
{
class CellView;
}

namespace MinerSpeed
{

class CollapseAnimation : public Animation
{
  public:
    CollapseAnimation(King::Engine &engine);
    virtual ~CollapseAnimation();

  public:
    void Start(const BoardView::VPShiftedCellViews &shiftedCellViews, const BoardView::VPCellViews &createdCellViews);

  private:
    void AddToGroup(const MinerSpeed::CellView *cellView, const size_t shift);
};
} // namespace MinerSpeed
