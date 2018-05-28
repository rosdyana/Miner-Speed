#pragma once

#include "../Model/Board.h"

namespace MinerSpeed
{

class MatchFinder
{

  public:
    MatchFinder();
    virtual ~MatchFinder();

  public:
    void Init(Board &board);
    void CheckForMatch(Board::VPCells &selectedCells, Board::VPCells &matchedCells);

  private:
    void CheckMatchInCells(Board::VPCells &inputCells, Board::VPCells &outputCells, const size_t pivotCellIndex, const MinerSpeed::Item::Color colorToCompare);
    void FindMatchInCells(const Board::VPCells &inputCells, Board::VPCells &outputCells, const size_t pivotCellIndex, const Item::Color colorToCompare);
    void MatchFinder::SetMarked(const Board::VPCells &cells, bool value);

  private:
    MinerSpeed::Board *mBoard;
};
} // namespace MinerSpeed
