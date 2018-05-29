#pragma once

#include "Animation.h"
#include "../View/BoardView.h"

namespace MinerSpeed
{

    class CollectAnimation : public Animation
    {

    private:
        static const char *WOW_TEXT;
        static const char *AMAZING_TEXT;
        static const char *AWESOME_TEXT;

        enum CollectedTexts {
            TEXT_WOW,
            TEXT_AMAZING,
            TEXT_AWESOME,
            TEXT_MAX
        };

        static const char *const COLLECTED_TEXT[];

    public:
        CollectAnimation(King::Engine &engine, glm::vec2 explostionTextPositionOnScreen);
        virtual ~CollectAnimation();

    public:
        virtual void Update();
        void Start(const MinerSpeed::BoardView::VPCellViews &cellViews);

    private:
        void UpdateExplosionLabel();

    private:
        const size_t GetRandomExplostionTextIndex();

    private:
        glm::vec2 mExplosionTextPositionOnScreen;
        int mRotationStep;
        float mRotationDirectionMask;
        size_t mCurrentExplosionTextIndex;
    };
} // namespace MinerSpeed
