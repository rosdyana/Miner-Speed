#include "CellView.h"
#include "BoardView.h"
#include "../Model/Cell.h"
#include "../Game.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

namespace MinerSpeed
{

    King::Engine::Texture CellView::GetTextureByColor(const MinerSpeed::Item::Color color)
    {
        King::Engine::Texture texture;
        switch (color) {
            case Item::Color::COLOR_BLUE : {
                texture = King::Engine::Texture::TEXTURE_BLUE;
                break;
            }
            case Item::Color::COLOR_GREEN : {
                texture = King::Engine::Texture::TEXTURE_GREEN;
                break;
            }
            case Item::Color::COLOR_PURPLE: {
                texture = King::Engine::Texture::TEXTURE_PURPLE;
                break;
            }
            case Item::Color::COLOR_RED: {
                texture = King::Engine::Texture::TEXTURE_RED;
                break;
            }
            case Item::Color::COLOR_YELLOW: {
                texture = King::Engine::Texture::TEXTURE_YELLOW;
                break;
            }
            default: {
                assert(false);
            }
        }
        return texture;
    }

    //********************************************************************************************************************************
    CellView::CellView(King::Engine &engine, const MinerSpeed::Cell &cell, const glm::uvec2 positionOnBoard, const glm::vec2 positionOnScreen)
        : mEngine(&engine)
        , mCell(&cell)
        , mPositionOnBoard(positionOnBoard)
        , mPositionOnScreen(positionOnScreen)
        , mRotation(0.0f)
        , mIsDrawEnabled(false)
    {
    }

    //********************************************************************************************************************************
    CellView::~CellView()
    {
    }

    //********************************************************************************************************************************
    void CellView::Init(const bool drawEnabled)
    {
        UpdateData();
        SetDrawEnabled(drawEnabled);
    }

    //********************************************************************************************************************************
    void CellView::Update()
    {
        if (false == mIsDrawEnabled) {
            return;
        }

        if (nullptr == mCell) {
            return;
        }

        const MinerSpeed::Item *item = &mCell->GetItem();

        if (nullptr == item) {
            return;
        }

        Item::State state = item->GetState();
        switch (state) {
            case Item::State::STATE_SELECTED: {
                DrawSelected((&mCell->GetItem())->GetColor());
                break;
            }
            default: {
                DrawNeutreal();
                break;
            }
        }
    }

    //********************************************************************************************************************************
    void CellView::SetDrawEnabled(bool value)
    {
        mIsDrawEnabled = value;
    }

    //********************************************************************************************************************************
    void CellView::UpdateData()
    {
        assert(mCell != nullptr);
        assert(&mCell->GetItem() != nullptr);
        mTexture = GetTextureByColor((&mCell->GetItem())->GetColor());
    }

    //********************************************************************************************************************************
    const glm::uvec2 &CellView::GetPositionOnBoard() const
    {
        return mPositionOnBoard;
    }

    //********************************************************************************************************************************
    const glm::vec2 &CellView::GetPositionOnScreen() const
    {
        return mPositionOnScreen;
    }

    //********************************************************************************************************************************
    const King::Engine::Texture &CellView::GetTexture() const
    {
        return mTexture;
    }

    //********************************************************************************************************************************
    void CellView::DrawNeutreal()
    {
        mEngine->Render(mTexture, mPositionOnScreen.x, mPositionOnScreen.y);
    }

    //********************************************************************************************************************************
    void CellView::DrawSelected(const MinerSpeed::Item::Color color)
    {
        King::Engine::Texture textureMark;
        switch (color) {
            case Item::Color::COLOR_BLUE: {
                textureMark = King::Engine::Texture::TEXTURE_BLUE_SELECTED;
                break;
            }
            case Item::Color::COLOR_GREEN: {
                textureMark = King::Engine::Texture::TEXTURE_GREEN_SELECTED;
                break;
            }
            case Item::Color::COLOR_PURPLE: {
                textureMark = King::Engine::Texture::TEXTURE_PURPLE_SELECTED;
                break;
            }
            case Item::Color::COLOR_RED: {
                textureMark = King::Engine::Texture::TEXTURE_RED_SELECTED;
                break;
            }
            case Item::Color::COLOR_YELLOW: {
                textureMark = King::Engine::Texture::TEXTURE_YELLOW_SELECTED;
                break;
            }
            default: {
                assert(false);
            }
        }
        mRotation += mEngine->GetLastFrameSeconds();

        int textWidth = mEngine->GetTextureWidth(textureMark);
        int textHeight = mEngine->GetTextureHeight(textureMark);

        glm::mat4 transformation(1.0);

        transformation = glm::translate(transformation, glm::vec3(mPositionOnScreen.x, mPositionOnScreen.y, 0.0f));

        mRotation += mEngine->GetLastFrameSeconds();
        transformation = glm::translate(transformation, glm::vec3(textWidth / 2.0f, textHeight / 2.0f, 0.0f));
        transformation = glm::rotate(transformation, mRotation * 2.5f, glm::vec3(0.0f, 0.0f, 1.0f));
        transformation = glm::translate(transformation, glm::vec3(-textWidth / 2.0f, -textHeight / 2.0f, 0.0f));

        mEngine->Render(mTexture, mPositionOnScreen.x, mPositionOnScreen.y);
        mEngine->Render(textureMark, transformation);
    }
}
