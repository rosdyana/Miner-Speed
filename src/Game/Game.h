#pragma once

#include <king/Updater.h>

namespace MinerSpeed
{
    class GameView;
    class Board;
    class MatchFinder;
	class Audio;
}

namespace King
{
    class Engine;
}

namespace MinerSpeed
{
    static const float CELL_WIDTH = 47.0f;
    static const float CELL_HEIGHT = 47.0f;
}

namespace MinerSpeed
{

    class Game : public King::Updater
    {

    public:
        Game();
        virtual	~Game();

    public:
        void			Start();
        virtual void	Update();

    public:
        King::Engine	&GetEngine();
        Board			&GetBoard();

    private:
        void			Init();

    private:
        King::Engine	*mEngine;
        GameView		*mView;
        Board			*mBoard;
        MatchFinder	*mMatchFinder;
		Audio		*mAudio;
    };
}
