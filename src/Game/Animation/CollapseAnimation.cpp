#include "CollapseAnimation.h"
#include "../Game.h"
#include "../View/CellView.h"
#include "../Tween/GroupTween.h"
#include "../Tween/PositionTween.h"
#include "../Audio/Audio.h"

#include <king/Engine.h>

namespace MinerSpeed
{

    //********************************************************************************************************************************
    CollapseAnimation::CollapseAnimation(King::Engine &engine)
        : Animation(engine)
    {
    }

    //********************************************************************************************************************************
    CollapseAnimation::~CollapseAnimation()
    {
    }

    //********************************************************************************************************************************
    void CollapseAnimation::Start(const BoardView::VPShiftedCellViews &shiftedCellViews, const BoardView::VPCellViews &createdCellViews)
    {
        if (IsRunning()) {
            return;
        }
        mIsRunning = true;
        int shiftsByColumn[8] = {0};
        int createdByColumn[8] = {0};
        for (BoardView::VPShiftedCellViews::const_iterator itShifted = shiftedCellViews.begin(); itShifted != shiftedCellViews.end(); ++itShifted) {
            AddToGroup((*itShifted)->cellView, (*itShifted)->shift);
            shiftsByColumn[(*itShifted)->cellView->GetPositionOnBoard().x] += 1;
        }

        for (BoardView::VPCellViews::const_iterator itCreated = createdCellViews.begin(); itCreated != createdCellViews.end(); ++itCreated) {
            createdByColumn[(*itCreated)->GetPositionOnBoard().x] += 1;
        }

        for (BoardView::VPCellViews::const_iterator itCreated = createdCellViews.begin(); itCreated != createdCellViews.end(); ++itCreated) {
            int shiftCalculated = (createdByColumn[(*itCreated)->GetPositionOnBoard().x] * 2) - (*itCreated)->GetPositionOnBoard().y + shiftsByColumn[(*itCreated)->GetPositionOnBoard().x];
            AddToGroup((*itCreated), (shiftCalculated));
        }

        mGroupTween->Start();
    }

    //********************************************************************************************************************************
    void CollapseAnimation::AddToGroup(const MinerSpeed::CellView *cellView, const size_t shift)
    {
        King::Engine::Texture texture = cellView->GetTexture();
        glm::vec2 posA = cellView->GetPositionOnScreen();
        glm::vec2 posB = posA;
        posB.y -= shift * MinerSpeed::CELL_HEIGHT;
        mGroupTween->AddTween(new MinerSpeed::PositionTween(*mEngine, texture, posB, posA, 1.2f, 2.0f));
        mAudio->PlaySound(Audio::SoundFx::MOVE_SUCCESFUL);
    }
} // namespace MinerSpeed
