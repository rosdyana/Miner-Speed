#include "MatchFinder.h"
#include "../Model/Cell.h"
#include "../Model/Item.h"

namespace MinerSpeed
{

    static const size_t MATCH_MIN_SIZE = 3;

    //********************************************************************************************************************************
    MatchFinder::MatchFinder()
        : mBoard(nullptr)
    {
    }

    //********************************************************************************************************************************
    MatchFinder::~MatchFinder()
    {
    }

    //********************************************************************************************************************************
    void MatchFinder::Init(Board &board)
    {
        mBoard = &board;
    }

    //********************************************************************************************************************************
    void MatchFinder::CheckForMatch(Board::VPCells &selectedCells, Board::VPCells &matchedCells)
    {
        SetMarked(matchedCells, false);
        matchedCells.clear();

        // 0 row
        Board::VPCells matchedCells_0;
        Board::VPCells inputCells;
        mBoard->GetRowCells(selectedCells[0]->GetPosition().y, inputCells);
        MinerSpeed::Item::Color colorToCompare = selectedCells[0]->GetItem().GetColor();
        CheckMatchInCells(inputCells, matchedCells_0, selectedCells[0]->GetPosition().x, colorToCompare);

        // 0 col
        Board::VPCells matchedCells_1;
        mBoard->GetColCells(selectedCells[0]->GetPosition().x, inputCells);
        colorToCompare = selectedCells[0]->GetItem().GetColor();
        CheckMatchInCells(inputCells, matchedCells_1, selectedCells[0]->GetPosition().y, colorToCompare);

        // repeat

        // 1 row
        Board::VPCells matchedCells_2;
        mBoard->GetRowCells(selectedCells[1]->GetPosition().y, inputCells);
        colorToCompare = selectedCells[1]->GetItem().GetColor();
        CheckMatchInCells(inputCells, matchedCells_2, selectedCells[1]->GetPosition().x, colorToCompare);

        // 1 col
        Board::VPCells matchedCells_3;
        mBoard->GetColCells(selectedCells[1]->GetPosition().x, inputCells);
        colorToCompare = selectedCells[1]->GetItem().GetColor();
        CheckMatchInCells(inputCells, matchedCells_3, selectedCells[1]->GetPosition().y, colorToCompare);

        matchedCells.insert(matchedCells.end(), matchedCells_0.begin(), matchedCells_0.end());
        matchedCells.insert(matchedCells.end(), matchedCells_1.begin(), matchedCells_1.end());
        matchedCells.insert(matchedCells.end(), matchedCells_2.begin(), matchedCells_2.end());
        matchedCells.insert(matchedCells.end(), matchedCells_3.begin(), matchedCells_3.end());

        for (Board::VPCells::iterator it = matchedCells.begin(); it != matchedCells.end();) {
            if ((*it)->IsMarked()) {
                it = matchedCells.erase(it);
            } else {
                (*it)->SetMarked(true);
                ++it;
            }
        }
    }

    //********************************************************************************************************************************
    void MatchFinder::CheckMatchInCells(Board::VPCells &inputCells, Board::VPCells &outputCells, const size_t pivotCellIndex, const MinerSpeed::Item::Color colorToCompare)
    {
        FindMatchInCells(inputCells, outputCells, pivotCellIndex, colorToCompare);
        if (outputCells.size() < MATCH_MIN_SIZE) {
            outputCells.clear();
        }
        inputCells.clear();
    }

    //********************************************************************************************************************************
    void MatchFinder::FindMatchInCells(const Board::VPCells &inputCells, Board::VPCells &outputCells, const size_t pivotCellIndex, const MinerSpeed::Item::Color colorToCompare)
    {
        int currentIndex = pivotCellIndex - 1;
        for (currentIndex; currentIndex >= 0; --currentIndex) {
            if (inputCells[currentIndex]->GetItem().GetColor() == colorToCompare) {
                outputCells.push_back(inputCells[currentIndex]);
            } else {
                break;
            }
        }

        outputCells.push_back(inputCells[pivotCellIndex]);

        currentIndex = pivotCellIndex + 1;
        int inputCellsSize = static_cast<int>(inputCells.size());
        for (currentIndex; currentIndex < inputCellsSize; ++currentIndex) {
            if (inputCells[currentIndex]->GetItem().GetColor() == colorToCompare) {
                outputCells.push_back(inputCells[currentIndex]);
            } else {
                break;
            }
        }
    }

    //********************************************************************************************************************************
    void MatchFinder::SetMarked(const Board::VPCells &cells, bool value)
    {
        for (Board::VPCells::const_iterator it = cells.begin(); it != cells.end(); ++it) {
            (*it)->SetMarked(value);
        }
    }
}
