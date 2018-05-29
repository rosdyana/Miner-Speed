#include "BoardView.h"
#include "../Game.h"
#include "../Model/Cell.h"
#include "CellView.h"

#include "../Animation/StartAnimation.h"
#include "../Animation/SwitchAnimation.h"
#include "../Animation/CollectAnimation.h"
#include "../Animation/CollapseAnimation.h"

#include <assert.h>

namespace MinerSpeed
{

    //********************************************************************************************************************************
    BoardView::BoardView()
        : View(), mBoardWidth(0.0f), mBoardHeight(0.0f), mPrevCellRowIndex(-1), mPrevCellColIndex(-1), mCapturedCellRowIndex(-1), mCapturedCellColIndex(-1), mRowCount(0), mColCount(0), mIsEnabled(false), mParentViewPosition(0.0f, 0.0f)
    {
    }

    //********************************************************************************************************************************
    BoardView::~BoardView()
    {
        for (BoardView::VPCellViews::const_iterator it = mCellViews.begin(); it != mCellViews.end(); ++it) {
            delete (*it);
        }
        mCellViews.clear();
    }

    //********************************************************************************************************************************
    void BoardView::Init(Game &game, glm::vec2 parentViewPosition)
    {
        mParentViewPosition = parentViewPosition;
        View::Init(game);
        InitBoardInfo();
        InitCellViews();
        InitAnimations();
    }

    //********************************************************************************************************************************
    void BoardView::InitPosition()
    {
        mPosition = glm::vec2(mParentViewPosition.x + 310.0f, mParentViewPosition.y + 100.0f);
    }

    //********************************************************************************************************************************
    void BoardView::Start()
    {
        mStartAnimation->Start(mCellViews, mRowCount);
    }

    //********************************************************************************************************************************
    void BoardView::Update()
    {
        if (IsInputEnabled()) {
            size_t colIndex;
            size_t rowIndex;
            if (GetUserInput(colIndex, rowIndex)) {
                mBoard->HandleSelectCell(colIndex, rowIndex);

                if (mBoard->IsSwitchExists()) {
                    FillCellViews(mBoard->GetSelectedCells(), mSelectedCellViews);
                    SetDrawEnabledCellViews(mSelectedCellViews, false, false);
                    mBoard->SwitchItems();
                    mSwitchAnimation->Start(mSelectedCellViews);
                }
            }
        }
        UpdateCells();
        UpdateAnimations();
    }

    //********************************************************************************************************************************
    void BoardView::setEnabled(bool value)
    {
        mIsEnabled = value;
    }

    //********************************************************************************************************************************
    void BoardView::InitBoardInfo()
    {
        mBoard = &(mGame->GetBoard());
        mBoardWidth = mBoard->GetColCount() * MinerSpeed::CELL_WIDTH;
        mBoardHeight = mBoard->GetRowCount() * MinerSpeed::CELL_HEIGHT;
    }

    //********************************************************************************************************************************
    void BoardView::InitCellViews()
    {
        mRowCount = mBoard->GetRowCount();
        mColCount = mBoard->GetColCount();
        for (size_t i = 0; i < mRowCount; ++i) {
            for (size_t j = 0; j < mColCount; ++j) {
                const MinerSpeed::Cell &cell = mBoard->GetCell(i, j);
                MinerSpeed::CellView *cellView = new CellView(*mEngine, cell, glm::uvec2(j, i), GetPositionOnScreen(j, i));
                mCellViews.push_back(cellView);
                cellView->Init(false);
            }
        }
    }

    //********************************************************************************************************************************
    void BoardView::InitAnimations()
    {
        mStartAnimation = std::make_unique<StartAnimation>(*mEngine);
        mSwitchAnimation = std::make_unique<SwitchAnimation>(*mEngine);
        mSwitchBackAnimation = std::make_unique<SwitchAnimation>(*mEngine);
        mCollectAnimation = std::make_unique<CollectAnimation>(*mEngine, glm::vec2(mPosition.x + 270, mPosition.y + 420));
        mCollapseAnimation = std::make_unique<CollapseAnimation>(*mEngine);
    }

    //********************************************************************************************************************************
    bool BoardView::GetUserInput(size_t &colIndex, size_t &rowIndex)
    {
        bool retVal = false;
        float mouseX = mEngine->GetMouseX();
        if (mouseX < mPosition.x || mouseX > mPosition.x + mBoardWidth) {
            return retVal;
        }
        float mouseY = mEngine->GetMouseY();
        if (mouseY < mPosition.y || mouseY > mPosition.y + mBoardHeight) {
            return retVal;
        }

        int cellColIndex = static_cast<int>((mouseX - mPosition.x) / MinerSpeed::CELL_WIDTH);
        int cellRowIndex = static_cast<int>((mouseY - mPosition.y) / MinerSpeed::CELL_HEIGHT);

        if (false == mEngine->GetMouseButtonDown()) {
            mCapturedCellColIndex = cellColIndex;
            mCapturedCellRowIndex = cellRowIndex;
            mPrevCellColIndex = -1;
            mPrevCellRowIndex = -1;
        } else {
            if (cellColIndex == mCapturedCellColIndex && cellRowIndex == mCapturedCellRowIndex) {
                if (cellColIndex != mPrevCellColIndex && cellRowIndex != mPrevCellRowIndex) {
                    retVal = true;
                    colIndex = cellColIndex;
                    rowIndex = cellRowIndex;
                    mPrevCellColIndex = cellColIndex;
                    mPrevCellRowIndex = cellRowIndex;
                    mCapturedCellColIndex = -1;
                    mCapturedCellRowIndex = -1;
                }
            } else {
                if ((cellColIndex != mPrevCellColIndex && cellRowIndex == mPrevCellRowIndex) || (cellColIndex == mPrevCellColIndex && cellRowIndex != mPrevCellRowIndex)) {
                    retVal = true;
                    colIndex = cellColIndex;
                    rowIndex = cellRowIndex;
                    mPrevCellColIndex = -1;
                    mPrevCellRowIndex = -1;
                    mCapturedCellColIndex = -1;
                    mCapturedCellRowIndex = -1;
                }
            }
        }

        return retVal;
    }

    //********************************************************************************************************************************
    glm::vec2 BoardView::GetPositionOnScreen(const size_t colIndex, const size_t rowIndex)
    {
        float posX = mPosition.x + colIndex * MinerSpeed::CELL_WIDTH;
        float posY = mPosition.y + rowIndex * MinerSpeed::CELL_HEIGHT;
        return glm::vec2(posX, posY);
    }

    //********************************************************************************************************************************
    void BoardView::SetDrawEnabledCellViews(const BoardView::VPCellViews &cellViews, bool state, bool update)
    {
        for (BoardView::VPCellViews::const_iterator it = cellViews.begin(); it != cellViews.end(); ++it) {
            if (*it) {
                if (update) {
                    (*it)->UpdateData();
                }
                (*it)->SetDrawEnabled(state);
            }
        }
    }

    //********************************************************************************************************************************
    void BoardView::SetDrawEnabledMatchAffectedCellViews(bool state, bool update)
    {
        for (BoardView::VPShiftedCellViews::const_iterator it = mItemShiftedCellViews.begin(); it != mItemShiftedCellViews.end(); ++it) {
            MinerSpeed::CellView *cellView = (*it)->cellView;
            if (cellView) {
                if (update) {
                    cellView->UpdateData();
                }
                cellView->SetDrawEnabled(state);
            }
        }
        SetDrawEnabledCellViews(mItemCreatedCellViews, state, update);
    }

    //********************************************************************************************************************************
    void BoardView::UpdateAnimations()
    {
        UpdateAnimation(mStartAnimation.get(), &BoardView::StartAnimationCompleteHandler);
        UpdateAnimation(mSwitchAnimation.get(), &BoardView::SwitchAnimationCompleteHandler);
        UpdateAnimation(mSwitchBackAnimation.get(), &BoardView::SwitchBackAnimationCompleteHandler);
        UpdateAnimation(mCollectAnimation.get(), &BoardView::CollectAnimationCompleteHandler);
        UpdateAnimation(mCollapseAnimation.get(), &BoardView::CollapseAnimationCompleteHandler);
    }

    //********************************************************************************************************************************
    void BoardView::UpdateAnimation(Animation *animation, void (MinerSpeed::BoardView::*completeHandler)())
    {
        if (animation->IsRunning()) {
            animation->Update();
            if (animation->IsCompleted()) {
                animation->Reset();
                if (nullptr != completeHandler) {
                    (*this.*completeHandler)();
                }
            }
        }
    }

    //********************************************************************************************************************************
    void BoardView::UpdateCells()
    {
        for (BoardView::VPCellViews::const_iterator it = mCellViews.begin(); it != mCellViews.end(); ++it) {
            (*it)->Update();
        }
    }

    //********************************************************************************************************************************
    MinerSpeed::CellView *BoardView::GetCellViewByCell(const Cell &cell)
    {
        return mCellViews[cell.GetPosition().y * mColCount + cell.GetPosition().x];
    }

    //********************************************************************************************************************************
    void BoardView::FillCellViews(const Board::VPCells &boardCells, BoardView::VPCellViews &cellViews)
    {
        cellViews.clear();
        for (Board::VPCells::const_iterator it = boardCells.begin(); it != boardCells.end(); ++it) {
            MinerSpeed::CellView *cellView = GetCellViewByCell(*(*it));
            cellViews.push_back(cellView);
        }
    }

    //********************************************************************************************************************************
    void BoardView::FillMatchAffectedCellViews(const Board::TVShiftedCells &shiftedCells, BoardView::VPShiftedCellViews &shiftedCellViews)
    {
        mItemShiftedCellViews.clear();
        for (Board::TVShiftedCells::const_iterator it = shiftedCells.begin(); it != shiftedCells.end(); ++it) {
            MinerSpeed::CellView *cellView = GetCellViewByCell(*(*it)->cell);
            if (cellView) {
                mItemShiftedCellViews.push_back(std::make_unique<CellViewShiftDownInfo>(cellView, (*it)->shift));
            }
        }
    }

    //********************************************************************************************************************************
    bool BoardView::IsInputEnabled()
    {
        return mIsEnabled && !mSwitchAnimation->IsRunning() && !mSwitchBackAnimation->IsRunning() && !mCollectAnimation->IsRunning() && !mCollapseAnimation->IsRunning();
    }

    //********************************************************************************************************************************
    void BoardView::StartAnimationCompleteHandler()
    {
        SetDrawEnabledCellViews(mCellViews, true, false);
    }

    //********************************************************************************************************************************
    void BoardView::SwitchAnimationCompleteHandler()
    {
        SetDrawEnabledCellViews(mSelectedCellViews, true, true);
        mBoard->CheckForMatch();
        if (mBoard->IsMatchExists()) {
            FillCellViews(mBoard->GetMatchedCells(), mMatchedCellViews);
            SetDrawEnabledCellViews(mMatchedCellViews, false, false);
            mCollectAnimation->Start(mMatchedCellViews);
        } else {
            SetDrawEnabledCellViews(mSelectedCellViews, false, false);
            mBoard->SwitchItems();
            mSwitchBackAnimation->Start(mSelectedCellViews);
        }
    }

    //********************************************************************************************************************************
    void BoardView::SwitchBackAnimationCompleteHandler()
    {
        SetDrawEnabledCellViews(mSelectedCellViews, true, true);
    }

    //********************************************************************************************************************************
    void BoardView::CollectAnimationCompleteHandler()
    {
        SetDrawEnabledCellViews(mMatchedCellViews, true, true);
        mBoard->ProcessMatch();
        FillMatchAffectedCellViews(mBoard->GetShiftedCells(), mItemShiftedCellViews);
        FillCellViews(mBoard->GetCreatedCells(), mItemCreatedCellViews);
        SetDrawEnabledMatchAffectedCellViews(false, true);
        mCollapseAnimation->Start(mItemShiftedCellViews, mItemCreatedCellViews);
    }

    //********************************************************************************************************************************
    void BoardView::CollapseAnimationCompleteHandler()
    {
        SetDrawEnabledMatchAffectedCellViews(true, false);
    }
} // namespace MinerSpeed
