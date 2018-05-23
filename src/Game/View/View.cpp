#include "View.h"
#include "../Game.h"

#include <king/Engine.h>

namespace MinerSpeed
{

    //********************************************************************************************************************************
    View::View()
        : mGame(nullptr)
        , mEngine(nullptr)
        , mPosition(0, 0)
    {
    }

    //********************************************************************************************************************************
    View::~View()
    {
    }

    //********************************************************************************************************************************
    void View::Init(Game &game)
    {
        mGame = &game;
        mEngine = &(mGame->GetEngine());
        InitPosition();
    }
}