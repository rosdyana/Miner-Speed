#include "StartAnimation.h"
#include "../Game.h"
#include "../View/CellView.h"
#include "../Tween/GroupTween.h"
#include "../Tween/PositionTween.h"

#include <king/Engine.h>

namespace MinerSpeed
{

    //********************************************************************************************************************************
    StartAnimation::StartAnimation(King::Engine &engine)
        : Animation(engine)
    {
    }

    //********************************************************************************************************************************
    StartAnimation::~StartAnimation()
    {
    }

    //********************************************************************************************************************************
    void StartAnimation::Start(const BoardView::VPCellViews &cellViews, const size_t rowCount)
    {
        if (IsRunning()) {
            return;
        }
        mIsRunning = true;

        for (BoardView::VPCellViews::const_iterator it = cellViews.begin(); it != cellViews.end(); ++it) {
            AddToGroup((*it), ((rowCount * 2 - (*it)->GetPositionOnBoard().y)));
        }

        mGroupTween->Start();
    }

    //********************************************************************************************************************************
    void StartAnimation::AddToGroup(const MinerSpeed::CellView *cellView, const size_t shift)
    {
        King::Engine::Texture texture = cellView->GetTexture();
        glm::vec2 posA = cellView->GetPositionOnScreen();
        glm::vec2 posB = posA;
        posB.y -= shift * MinerSpeed::CELL_HEIGHT;
        mGroupTween->AddTween(new MinerSpeed::PositionTween(*mEngine, texture, posB, posA, 1.2f, 2.0f));
    }
}
