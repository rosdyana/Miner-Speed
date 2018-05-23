#pragma once

#include "Animation.h"

#include "../View/BoardView.h"

namespace MinerSpeed
{
    class CellView;
}

namespace MinerSpeed
{

    class StartAnimation : public Animation
    {
    public:
        StartAnimation(King::Engine &engine);
        virtual	~StartAnimation();

    public:
        void	Start(const BoardView::VPCellViews &cellViews, const size_t rowCount);

    private:
        void	AddToGroup(const MinerSpeed::CellView *cellView, const size_t shift);

    };
}
