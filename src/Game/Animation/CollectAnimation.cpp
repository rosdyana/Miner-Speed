#include "CollectAnimation.h"
#include "../View/BoardView.h"
#include "../View/CellView.h"
#include "../Tween/GroupTween.h"
#include "../Tween/ProportionalScaleTween.h"

#include <king/Engine.h>

namespace MinerSpeed
{

    const char *CollectAnimation::WOW_TEXT = "WOOOWW!";
    const char *CollectAnimation::AMAZING_TEXT = "AMAZIIING!";
    const char *CollectAnimation::AWESOME_TEXT = "AWESEOOMMEE!";

    const char *const CollectAnimation::COLLECTED_TEXT[] = {CollectAnimation::WOW_TEXT, CollectAnimation::AMAZING_TEXT, CollectAnimation::AWESOME_TEXT };

    //********************************************************************************************************************************
    CollectAnimation::CollectAnimation(King::Engine &engine, glm::vec2 explostionTextPositionOnScreen)
        : Animation(engine), mCurrentExplosionTextIndex(0), mRotationDirectionMask(1.0f), mExplosionTextPositionOnScreen(explostionTextPositionOnScreen), mRotationStep(0)
    {
    }

    //********************************************************************************************************************************
    CollectAnimation::~CollectAnimation()
    {
    }

    void CollectAnimation::Update()
    {
        Animation::Update();
        UpdateExplosionLabel();
    }

    //********************************************************************************************************************************
    void CollectAnimation::Start(const MinerSpeed::BoardView::VPCellViews &cellViews)
    {
        if (IsRunning()) {
            return;
        }
        mIsRunning = true;

        mRotationStep = 0;
        mCurrentExplosionTextIndex = GetRandomExplostionTextIndex();
        mRotationDirectionMask = 1.0f;
        if (0 == (rand() % 2)) {
            mRotationDirectionMask = -1.0f;
        }

        for (BoardView::VPCellViews::const_iterator it = cellViews.begin(); it != cellViews.end(); ++it) {
            MinerSpeed::CellView *cellView = (*it);
            King::Engine::Texture texture = cellView->GetTexture();
            glm::vec2 pos = cellView->GetPositionOnScreen();
            mGroupTween->AddTween(new MinerSpeed::ProportionalScaleTween(*mEngine, texture, pos, 1.0f, 0.5f, 1.2f, 0.5f));
        }

        mGroupTween->Start();
    }

    //********************************************************************************************************************************
    void CollectAnimation::UpdateExplosionLabel()
    {
        ++mRotationStep;
        const char *label = CollectAnimation::COLLECTED_TEXT[mCurrentExplosionTextIndex];
        float labelWidth = mEngine->CalculateStringWidth(label);
        float rotation = mRotationStep * mRotationDirectionMask * 3.14f / 180;
        rotation = (rotation > 75 ? 75 : rotation);
        mEngine->Write(label, mExplosionTextPositionOnScreen.x - labelWidth / 2, mExplosionTextPositionOnScreen.y, rotation);
    }

    //********************************************************************************************************************************
    const size_t CollectAnimation::GetRandomExplostionTextIndex()
    {
        return CollectAnimation::CollectedTexts::TEXT_WOW + (rand() % ((CollectAnimation::CollectedTexts::TEXT_MAX) - CollectAnimation::CollectedTexts::TEXT_WOW));
    }
} // namespace MinerSpeed
