#pragma once

#include "../Model/Item.h"

#include <king/Engine.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace MinerSpeed
{
    class Cell;
}

namespace MinerSpeed
{

    class CellView
    {

    public:
        static King::Engine::Texture GetTextureByColor(const MinerSpeed::Item::Color color);

    public:
        CellView(King::Engine &engine, const MinerSpeed::Cell &cell, const glm::uvec2 positionOnBoard, const glm::vec2 positionOnScreen);
        virtual	~CellView();

    public:
        virtual	void	Init(const bool drawEnabled);
        virtual	void	Update();
        void			SetDrawEnabled(bool value);
        void			UpdateData();
        const			glm::uvec2 &GetPositionOnBoard()	const;
        const			glm::vec2 &GetPositionOnScreen()	const;
        const			King::Engine::Texture &GetTexture()	const;

    private:
        void	DrawNeutreal();
        void	DrawSelected(const MinerSpeed::Item::Color color);

    private:
        King::Engine			*mEngine;
        const MinerSpeed::Cell		*mCell;
        King::Engine::Texture	mTexture;
        glm::uvec2				mPositionOnBoard;
        glm::vec2				mPositionOnScreen;
        float					mRotation;
        bool					mIsDrawEnabled;
    };
}
