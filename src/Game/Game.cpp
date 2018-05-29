#include "Game.h"
#include "View/GameView.h"
#include "Model/Board.h"
#include "MatchFinder/MatchFinder.h"

#include <ctime>
#include <king/Engine.h>

namespace MinerSpeed
{

    //********************************************************************************************************************************
    Game::Game()
        : mEngine(nullptr), mMatchFinder(nullptr), mBoard(nullptr), mView(nullptr)
    {
        Init();
    }

    //********************************************************************************************************************************
    Game::~Game()
    {
        delete mView;
        delete mBoard;
        delete mMatchFinder;
        delete mEngine;
    }

    //********************************************************************************************************************************
    void Game::Start()
    {
        mView->Start();
        mEngine->Start(*this);
    }

    //********************************************************************************************************************************
    void Game::Update()
    {
        mView->Update();
    }

    //********************************************************************************************************************************
    King::Engine &Game::GetEngine()
    {
        return *mEngine;
    }

    //********************************************************************************************************************************
    Board &Game::GetBoard()
    {
        return *mBoard;
    }

    //********************************************************************************************************************************
    void Game::Init()
    {
        srand(static_cast<unsigned int>(time(NULL)));
        mEngine = new King::Engine("./assets");
        assert(nullptr != mEngine);
        mMatchFinder = new MatchFinder();
        assert(nullptr != mMatchFinder);
        mBoard = new Board(8, 8);
        assert(nullptr != mBoard);
        mView = new GameView();
        assert(nullptr != mView);

        mMatchFinder->Init(*mBoard);
        mBoard->InitRandom(*mMatchFinder);
        mView->Init(*this);
    }
} // namespace MinerSpeed
