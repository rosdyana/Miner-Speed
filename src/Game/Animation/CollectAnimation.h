#pragma once

#include "Animation.h"
#include "../View/BoardView.h"

namespace MinerSpeed
{

    class CollectAnimation : public Animation
    {

    private:
        static const char *BOOM_TEXT;
        static const char *BABAX_TEXT;
        static const char *TADASH_TEXT;

        enum ExplostionTexts {
            TEXT_BOOM,
            TEXT_BABAX,
            TEXT_TADASH,
            TEXT_MAX
        };

        static const char *const EXPLOSION_TEXTS[];

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
