#pragma once

#include "Item.h"

#include "../ObjectPool/ObjectPool.h"

#include <vector>
#include <memory>

namespace MinerSpeed
{
    class Cell;
    class Item;
    class MatchFinder;
    class Audio;
} // namespace MinerSpeed

namespace MinerSpeed
{
    struct CellShiftDownInfo {
        MinerSpeed::Cell *cell;
        size_t shift;

        CellShiftDownInfo(MinerSpeed::Cell *pCell, size_t pShift) : cell(pCell), shift(pShift) {};
    };
} // namespace MinerSpeed

namespace MinerSpeed
{
    struct ShiftRowCol {
        int deltaCol;
        int deltaRow;

        ShiftRowCol(const int pDeltaCol, const int pDeltaRow) : deltaCol(pDeltaCol), deltaRow(pDeltaRow) {};
    };
} // namespace MinerSpeed

namespace MinerSpeed
{

    class Board
    {

    public:
        typedef std::vector<MinerSpeed::Cell *> VPCells;
        typedef std::vector<std::unique_ptr<MinerSpeed::CellShiftDownInfo>> TVShiftedCells;

    public:
        Board(const size_t colCount, const size_t rowCount);
        virtual ~Board();

    public:
        const size_t GetColCount() const;
        const size_t GetRowCount() const;
        const MinerSpeed::Cell &GetCell(const size_t row, const size_t col);

    public:
        void InitRandom(MatchFinder &matchFinder);

    public:
        void HandleSelectCell(const size_t col, const size_t row);
        void DeselectSelectedCells();
        void SwitchItems();

    public:
        const Board::VPCells &GetSelectedCells();
        const Board::VPCells &GetMatchedCells();
        bool IsSwitchExists();
        bool IsMatchExists();
        void CheckForMatch();
        void ProcessMatch();
        const Board::TVShiftedCells &GetShiftedCells();
        const Board::VPCells &GetCreatedCells();
        void GetColCells(const size_t colIndex, Board::VPCells &cells);
        void GetRowCells(const size_t rowIndex, Board::VPCells &cells);

    private:
        void SwitchItemsInternal(MinerSpeed::Cell &cellA, MinerSpeed::Cell &cellB);
        MinerSpeed::Cell &GetNotConstCell(const size_t row, const size_t col);
        void SetCell(const size_t row, size_t col, MinerSpeed::Cell &value);
        MinerSpeed::Item &CreateRandomItem();
        void SelectCell(MinerSpeed::Cell &cell);
        bool IsNeighbours(const MinerSpeed::Cell &cellA, const MinerSpeed::Cell &cellB);

    private:
        size_t mColCount;
        size_t mRowCount;
        Board::VPCells mCells;
        Board::VPCells mSelectedCells;
        Board::VPCells mMatchedCells;
        MinerSpeed::MatchFinder *mMatchFinder;
        TVShiftedCells mShiftedCells;
        Board::VPCells mCreatedCells;
        MinerSpeed::ObjectPool<MinerSpeed::Item> mItemPool;
        MinerSpeed::Audio *mAudio;
    };
} // namespace MinerSpeed
