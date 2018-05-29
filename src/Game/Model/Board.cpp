#include "Board.h"
#include "Cell.h"

#include "../MatchFinder/MatchFinder.h"
#include "../Audio/Audio.h"
#include <assert.h>
#include <ctime>
#include <cmath>

namespace MinerSpeed
{

    //********************************************************************************************************************************
    Board::Board(const size_t colCount, const size_t rowCount)
        : mColCount(colCount), mRowCount(rowCount), mMatchFinder(nullptr), mCells(mColCount * mRowCount), mAudio(nullptr)
    {
        mAudio = new Audio();
        assert(mAudio != nullptr);
    }

    //********************************************************************************************************************************
    Board::~Board()
    {
        for (Board::VPCells::iterator it = mCells.begin(); it != mCells.end(); ++it) {
            mItemPool.Delete(&(*it)->GetItem());
            delete (*it);
        }
        mCells.clear();
    }

    //********************************************************************************************************************************
    const size_t Board::GetColCount() const
    {
        return mColCount;
    }

    //********************************************************************************************************************************
    const size_t Board::GetRowCount() const
    {
        return mRowCount;
    }

    //********************************************************************************************************************************
    const MinerSpeed::Cell &Board::GetCell(const size_t row, const size_t col)
    {
        assert(row < mRowCount);
        assert(col < mColCount);
        return *mCells[row * mColCount + col];
    }

    //********************************************************************************************************************************
    void Board::InitRandom(MatchFinder &matchFinder)
    {
        mMatchFinder = &matchFinder;
        for (size_t i = 0; i < mRowCount; ++i) {
            for (size_t j = 0; j < mColCount; ++j) {
                MinerSpeed::Cell *cell = new Cell(glm::uvec2(j, i));
                cell->SetItem(CreateRandomItem());

                Item::Color colorToCompare = cell->GetItem().GetColor();
                Item::Color color1;
                Item::Color color2;
                if (j >= 2) {
                    color1 = GetCell(i, j - 1).GetItem().GetColor();
                    color2 = GetCell(i, j - 2).GetItem().GetColor();
                    if ((color1 == colorToCompare) && (color2 == colorToCompare)) {
                        --j;
                        mItemPool.Delete(&cell->GetItem());
                        continue;
                    }
                }

                if (i >= 2) {
                    color1 = GetCell(i - 1, j).GetItem().GetColor();
                    color2 = GetCell(i - 2, j).GetItem().GetColor();

                    if ((color1 == colorToCompare) && (color2 == colorToCompare)) {
                        --j;
                        mItemPool.Delete(&cell->GetItem());
                        continue;
                    }
                }
                SetCell(i, j, *cell);
            }
        }
    }

    //********************************************************************************************************************************
    void Board::HandleSelectCell(const size_t col, const size_t row)
    {
        assert(row < mRowCount);
        assert(col < mColCount);

        DeselectSelectedCells();
        MinerSpeed::Cell *cell = mCells[row * mColCount + col];
        if (mSelectedCells.size() == 1) {
            if (IsNeighbours(*mSelectedCells[0], *cell)) {
                SelectCell(*cell);
            } else {
                mSelectedCells.clear();
                SelectCell(*cell);
            }
        } else {
            mSelectedCells.clear();
            SelectCell(*cell);
            mAudio->PlaySound(Audio::SoundFx::DIAMOND_LANDING);
        }
    }

    //********************************************************************************************************************************
    void Board::SwitchItems()
    {
        assert(mSelectedCells.size() > 1);
        MinerSpeed::Cell &cellA = GetNotConstCell(mSelectedCells[0]->GetPosition().y, mSelectedCells[0]->GetPosition().x);
        MinerSpeed::Cell &cellB = GetNotConstCell(mSelectedCells[1]->GetPosition().y, mSelectedCells[1]->GetPosition().x);
        SwitchItemsInternal(cellA, cellB);
    }

    //********************************************************************************************************************************
    const Board::VPCells &Board::GetSelectedCells()
    {
        return mSelectedCells;
    }

    //********************************************************************************************************************************
    const Board::VPCells &Board::GetMatchedCells()
    {
        return mMatchedCells;
    }

    //********************************************************************************************************************************
    bool Board::IsSwitchExists()
    {
        return mSelectedCells.size() > 1;
    }

    //********************************************************************************************************************************
    bool Board::IsMatchExists()
    {
        return mMatchedCells.size() > 0;
    }

    //********************************************************************************************************************************
    void Board::SwitchItemsInternal(MinerSpeed::Cell &cellA, MinerSpeed::Cell &cellB)
    {
        MinerSpeed::Item &tempItem = cellB.GetItem();
        cellB.SetItem(cellA.GetItem());
        cellA.SetItem(tempItem);
    }

    //********************************************************************************************************************************
    MinerSpeed::Cell &Board::GetNotConstCell(const size_t row, const size_t col)
    {
        assert(row < mRowCount);
        assert(col < mColCount);
        return *mCells[row * mColCount + col];
    }

    //********************************************************************************************************************************
    void Board::SetCell(const size_t row, size_t col, MinerSpeed::Cell &value)
    {
        assert(row < mRowCount);
        assert(col < mColCount);
        mCells[row * mColCount + col] = &value;
    }

    //********************************************************************************************************************************
    MinerSpeed::Item &Board::CreateRandomItem()
    {
        MinerSpeed::Item *item = mItemPool.New();
        item->SetColor(static_cast<Item::Color>(Item::Color::COLOR_BLUE + (rand() % (Item::Color::COLOR_MAX - Item::Color::COLOR_BLUE))));
        return *item;
    }

    //********************************************************************************************************************************
    void Board::SelectCell(MinerSpeed::Cell &cell)
    {
        if (mSelectedCells.empty()) {
            (&cell)->GetItem().SetState(MinerSpeed::Item::State::STATE_SELECTED);
        }
        mSelectedCells.push_back(&cell);
    }

    //********************************************************************************************************************************
    void Board::DeselectSelectedCells()
    {
        for (Board::VPCells::const_iterator it = mSelectedCells.begin(); it != mSelectedCells.end(); ++it) {
            (*it)->GetItem().SetState(MinerSpeed::Item::State::STATE_NEUTRAL);
        }
    }

    //********************************************************************************************************************************
    bool Board::IsNeighbours(const MinerSpeed::Cell &cellA, const MinerSpeed::Cell &cellB)
    {
        bool retVal = false;
        int cellARow = cellA.GetPosition().y;
        int cellACol = cellA.GetPosition().x;
        int cellBRow = cellB.GetPosition().y;
        int cellBCol = cellB.GetPosition().x;

        if (((std::fabs(cellARow - cellBRow)) + (std::fabs(cellACol - cellBCol))) == 1) {
            retVal = true;
        }
        return retVal;
    }

    //********************************************************************************************************************************
    void Board::GetColCells(const size_t colIndex, Board::VPCells &cells)
    {
        for (size_t i = 0; i < mRowCount; ++i) {
            size_t cellIndex = i * mColCount + colIndex;
            cells.push_back(mCells[cellIndex]);
        }
    }

    //********************************************************************************************************************************
    void Board::GetRowCells(const size_t rowIndex, Board::VPCells &cells)
    {
        size_t beginIndex = mColCount * rowIndex;
        size_t endIndex = beginIndex + mColCount;
        for (size_t i = beginIndex; i < endIndex; ++i) {
            cells.push_back(mCells[i]);
        }
    }

    //********************************************************************************************************************************
    void Board::CheckForMatch()
    {
        mMatchFinder->CheckForMatch(mSelectedCells, mMatchedCells);
    }

    //********************************************************************************************************************************
    void Board::ProcessMatch()
    {
        mShiftedCells.clear();
        mCreatedCells.clear();

        std::vector<size_t> processedCols;
        for (Board::VPCells::const_iterator it = mMatchedCells.begin(); it != mMatchedCells.end(); ++it) {
            MinerSpeed::Cell cell = *(*it);
            size_t colIndex = cell.GetPosition().x;
            if (std::find(processedCols.begin(), processedCols.end(), colIndex) == processedCols.end()) {
                processedCols.push_back(colIndex);

                int targetRowIndex = mRowCount - 1;
                for (targetRowIndex; targetRowIndex >= 0; --targetRowIndex) {
                    MinerSpeed::Cell &targetCell = GetNotConstCell(targetRowIndex, colIndex);
                    if (targetCell.IsMarked()) {
                        //Finding source cell
                        int sourceRowIndex = targetRowIndex - 1;
                        bool isShifted = false;
                        for (sourceRowIndex; sourceRowIndex >= 0; --sourceRowIndex) {
                            MinerSpeed::Cell &sourceCell = GetNotConstCell(sourceRowIndex, colIndex);
                            if (false == sourceCell.IsMarked()) {
                                targetCell.SetMarked(false);
                                sourceCell.SetMarked(true);
                                SwitchItemsInternal(targetCell, sourceCell);
                                mShiftedCells.push_back(std::make_unique<CellShiftDownInfo>(&targetCell, targetRowIndex - sourceRowIndex));
                                isShifted = true;
                                break;
                            }
                        }
                        if (false == isShifted) {
                            mItemPool.Delete(&(targetCell.GetItem()));
                            targetCell.SetItem(CreateRandomItem());
                            targetCell.SetMarked(false);
                            mCreatedCells.push_back(&targetCell);
                        }
                    }
                }
            }
        }
    }

    //********************************************************************************************************************************
    const Board::TVShiftedCells &Board::GetShiftedCells()
    {
        return mShiftedCells;
    }

    //********************************************************************************************************************************
    const Board::VPCells &Board::GetCreatedCells()
    {
        return mCreatedCells;
    }

    //********************************************************************************************************************************
    bool Board::LookForPossibles()
    {
        for (size_t col = 0; col < mColCount; ++col) {
            for (size_t row = 0; row < mRowCount; ++row) {

                if (MatchPattern(col, row, mPatternMustHave1, mPatternNeedOne1, 6)) {
                    return true;
                }

                if (MatchPattern(col, row, mPatternMustHave2, mPatternNeedOne2, 2)) {
                    return true;
                }

                if (MatchPattern(col, row, mPatternMustHave3, mPatternNeedOne3, 6)) {
                    return true;
                }

                if (MatchPattern(col, row, mPatternMustHave4, mPatternNeedOne4, 2)) {
                    return true;
                }
            }
        }
        return false;
    }

    //********************************************************************************************************************************
    bool Board::MatchPattern(const size_t row, const size_t col, MinerSpeed::ShiftRowCol mustHave[], MinerSpeed::ShiftRowCol needOne[], int needOneLength)
    {
        Item::Color color = GetCell(row, col).GetItem().GetColor();

        for (int i = 0; i < 1; ++i) {
            if (false == MatchColor(col + mustHave[i].deltaCol, row + mustHave[i].deltaRow, color)) {
                return false;
            }
        }

        for (int j = 0; j < needOneLength; ++j) {
            if (MatchColor(col + needOne[j].deltaCol, row + needOne[j].deltaRow, color)) {
                return true;
            }
        }
        return false;
    }

    //********************************************************************************************************************************
    bool Board::MatchColor(const int row, const int col, Item::Color color)
    {
        int colCount = static_cast<int>(mColCount);
        int rowCount = static_cast<int>(mRowCount);
        if ((col < 0) || (col > colCount - 1) || (row < 0) || (row > rowCount - 1)) {
            return false;
        }
        return (color == GetCell(row, col).GetItem().GetColor());
    }

    //********************************************************************************************************************************
    bool Board::CreatePossibles()
    {
        for (size_t col = 0; col < mColCount; ++col) {
            for (size_t row = 0; row < mRowCount; ++row) {
                if (CreatePossiblesPattern(col, row, mPatternMustHave1, mPatternNeedOne1, 6)) {
                    return true;
                }
                if (CreatePossiblesPattern(col, row, mPatternMustHave2, mPatternNeedOne2, 2)) {
                    return true;
                }
                if (CreatePossiblesPattern(col, row, mPatternMustHave3, mPatternNeedOne3, 6)) {
                    return true;
                }
                if (CreatePossiblesPattern(col, row, mPatternMustHave4, mPatternNeedOne4, 2)) {
                    return true;
                }
            }
        }
        return false;
    }

    //********************************************************************************************************************************
    bool Board::CreatePossiblesPattern(const size_t row, const size_t col, MinerSpeed::ShiftRowCol mustHave[], MinerSpeed::ShiftRowCol needOne[], int needOneLength)
    {
        MinerSpeed::Item::Color color = GetCell(row, col).GetItem().GetColor();

        for (int i = 0; i < 1; ++i) {
            if (false == MatchColor(col + mustHave[i].deltaCol, row + mustHave[i].deltaRow, color)) {
                return false;
            }
        }

        for (int i = 0; i < needOneLength; ++i) {
            if (SetPossiblesColor(col + needOne[i].deltaCol, row + needOne[i].deltaRow, color)) {
                return true;
            }
        }
        return false;
    }

    //********************************************************************************************************************************
    bool Board::SetPossiblesColor(int row, int col, MinerSpeed::Item::Color color)
    {
        int colCount = static_cast<int>(mColCount);
        int rowCount = static_cast<int>(mRowCount);
        if ((col < 0) || (col > colCount - 1) || (row < 0) || (row > rowCount - 1)) {
            return false;
        }
        GetCell(row, col).GetItem().SetColor(color);
        return true;
    }
} // namespace MinerSpeed
