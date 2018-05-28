#pragma once

#include "View.h"
#include "../Model/Board.h"

#include <king/Engine.h>
#include <glm/glm.hpp>

#include <vector>

namespace MinerSpeed
{
class Board;
class Cell;
class StartAnimation;
class SwitchAnimation;
class CollectAnimation;
class CollapseAnimation;
class Animation;
class CellView;
} // namespace MinerSpeed

namespace MinerSpeed
{
struct CellViewShiftDownInfo
{
    MinerSpeed::CellView *cellView;
    size_t shift;

    CellViewShiftDownInfo(MinerSpeed::CellView *pCellView, size_t pShift) : cellView(pCellView), shift(pShift){};
};
} // namespace MinerSpeed

namespace MinerSpeed
{

class BoardView : public View
{

  public:
    typedef std::vector<MinerSpeed::CellView *> VPCellViews;
    typedef std::vector<std::unique_ptr<MinerSpeed::CellViewShiftDownInfo>> VPShiftedCellViews;

  public:
    BoardView();
    virtual ~BoardView();

  public:
    virtual void Init(Game &game, glm::vec2 parentViewPosition);
    virtual void InitPosition();
    virtual void Start();
    virtual void Update();

  public:
    void setEnabled(bool value);

  private:
    void InitBoardInfo();
    void InitCellViews();
    void InitAnimations();

  private:
    glm::vec2 GetPositionOnScreen(const size_t colIndex, const size_t rowIndex);
    void SetDrawEnabledCellViews(const BoardView::VPCellViews &cellViews, bool state, bool update);
    void SetDrawEnabledMatchAffectedCellViews(bool state, bool update);
    void UpdateAnimations();
    void UpdateAnimation(Animation *animation, void (MinerSpeed::BoardView::*completeHandler)());
    bool GetUserInput(size_t &colIndex, size_t &rowIndex);
    void UpdateCells();
    MinerSpeed::CellView *GetCellViewByCell(const Cell &cell);
    void FillCellViews(const Board::VPCells &boardCells, BoardView::VPCellViews &cellViews);
    void FillMatchAffectedCellViews(const Board::TVShiftedCells &shiftedCells, BoardView::VPShiftedCellViews &shiftedCellViews);

  private:
    bool IsInputEnabled();

  private:
    void StartAnimationCompleteHandler();
    void SwitchAnimationCompleteHandler();
    void SwitchBackAnimationCompleteHandler();
    void CollectAnimationCompleteHandler();
    void CollapseAnimationCompleteHandler();

  private:
    Board *mBoard;
    float mBoardWidth;
    float mBoardHeight;
    size_t mRowCount;
    size_t mColCount;
    bool mIsEnabled;
    glm::vec2 mParentViewPosition;

    VPCellViews mCellViews;

    VPCellViews mSelectedCellViews;
    VPCellViews mMatchedCellViews;
    VPShiftedCellViews mItemShiftedCellViews;
    VPCellViews mItemCreatedCellViews;

    int mPrevCellRowIndex;
    int mPrevCellColIndex;
    int mCapturedCellRowIndex;
    int mCapturedCellColIndex;

    std::unique_ptr<MinerSpeed::StartAnimation> mStartAnimation;
    std::unique_ptr<MinerSpeed::SwitchAnimation> mSwitchAnimation;
    std::unique_ptr<MinerSpeed::SwitchAnimation> mSwitchBackAnimation;
    std::unique_ptr<MinerSpeed::CollectAnimation> mCollectAnimation;
    std::unique_ptr<MinerSpeed::CollapseAnimation> mCollapseAnimation;
};
} // namespace MinerSpeed
