#include "SwitchAnimation.h"
#include "../View/CellVIew.h"
#include "../Tween/GroupTween.h"
#include "../Tween/PositionTween.h"
#include "../Audio/Audio.h"

#include <king/Engine.h>

namespace MinerSpeed
{

    //********************************************************************************************************************************
    SwitchAnimation::SwitchAnimation(King::Engine &engine)
        : Animation(engine)
    {
    }

    //********************************************************************************************************************************
    SwitchAnimation::~SwitchAnimation()
    {
    }

    //********************************************************************************************************************************
    void SwitchAnimation::Start(const MinerSpeed::BoardView::VPCellViews &cellViews)
    {
        if (IsRunning()) {
            return;
        }
        mIsRunning = true;

        const King::Engine::Texture textureA = cellViews[0]->GetTexture();
        glm::vec2 posA = cellViews[0]->GetPositionOnScreen();
        const King::Engine::Texture textureB = cellViews[1]->GetTexture();
        glm::vec2 posB = cellViews[1]->GetPositionOnScreen();
        mGroupTween->AddTween(new MinerSpeed::PositionTween(*mEngine, textureA, posA, posB, 1.2f, 2.0f));
        mGroupTween->AddTween(new MinerSpeed::PositionTween(*mEngine, textureB, posB, posA, 1.2f, 2.0f));
        mGroupTween->Start();
		mAudio->PlaySound(Audio::SoundFx::DiamondLanding);
    }
}
