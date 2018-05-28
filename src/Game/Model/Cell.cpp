#include "Cell.h"
#include "Item.h"

#include <algorithm>
#include <assert.h>

namespace MinerSpeed
{

//********************************************************************************************************************************
Cell::Cell()
    : mItem(nullptr), mPosition(0, 0), mIsMarked(false)
{
}

//********************************************************************************************************************************
Cell::Cell(const glm::uvec2 position)
    : mItem(nullptr), mPosition(position), mIsMarked(false)
{
}

//********************************************************************************************************************************
Cell::Cell(const Cell &instance)
{
    mItem = new MinerSpeed::Item();
    mItem->SetColor(instance.GetItem().GetColor());
    mPosition = instance.mPosition;
    mIsMarked = instance.mIsMarked;
}

//********************************************************************************************************************************
Cell &Cell::operator=(const Cell &r)
{
    if (this != &r)
    {
        Cell temp(r);
        Swap(temp);
    }
    return *this;
}

//********************************************************************************************************************************
void Cell::Swap(Cell &r)
{
    MinerSpeed::Item *temp = nullptr;
    temp = mItem;
    mItem = r.mItem;
    r.mItem = temp;

    glm::uvec2 tempPosition = mPosition;
    mPosition = r.mPosition;
    r.mPosition = tempPosition;

    std::swap(mIsMarked, r.mIsMarked);
}

//********************************************************************************************************************************
Cell::~Cell()
{
}

//********************************************************************************************************************************
void Cell::SetItem(MinerSpeed::Item &item)
{
    mItem = &item;
}

//********************************************************************************************************************************
MinerSpeed::Item &Cell::GetItem() const
{
    return *mItem;
}

//********************************************************************************************************************************
void Cell::SetMarked(const bool value)
{
    mIsMarked = value;
}

//********************************************************************************************************************************
const bool Cell::IsMarked() const
{
    return mIsMarked;
}

//********************************************************************************************************************************
const glm::uvec2 &Cell::GetPosition() const
{
    return mPosition;
}
} // namespace MinerSpeed
