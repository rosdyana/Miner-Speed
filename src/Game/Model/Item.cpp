#include "Item.h"

#include <algorithm>

namespace MinerSpeed
{

//********************************************************************************************************************************
Item::Item()
    : mColor(Item::Color::COLOR_MIN), mState(Item::State::STATE_MIN)
{
}

//********************************************************************************************************************************
Item::Item(const Item &instance)
{
    mColor = instance.mColor;
    mState = instance.mState;
}

//********************************************************************************************************************************
Item &Item::operator=(const Item &r)
{
    if (this != &r)
    {
        Item temp(r);
        Swap(temp);
    }
    return *this;
}

//********************************************************************************************************************************
void Item::Swap(Item &r)
{
    std::swap(mColor, r.mColor);
    std::swap(mState, r.mState);
}

//********************************************************************************************************************************
Item::~Item()
{
}

//********************************************************************************************************************************
void Item::SetColor(const Item::Color color)
{
    mColor = color;
}

//********************************************************************************************************************************
const Item::Color Item::GetColor() const
{
    return mColor;
}

//********************************************************************************************************************************
Item::State Item::GetState() const
{
    return mState;
}

//********************************************************************************************************************************
void Item::SetState(Item::State state)
{
    mState = state;
}
} // namespace MinerSpeed
