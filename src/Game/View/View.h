#pragma once

#include <glm/glm.hpp>

namespace MinerSpeed
{
    class Game;
}

namespace King
{
    class Engine;
}

namespace MinerSpeed
{

    class View
    {

    public:
        View();
        virtual	~View();

    public:
        virtual	void	Init(Game &game);
        virtual	void	InitPosition()	=	0;
        virtual	void	Start()			=	0;
        virtual	void	Update()		=	0;

    protected:
        King::Engine	*mEngine;
        Game			*mGame;
        glm::vec2		mPosition;
    };
}
