#include "Animation.h"
#include "../Audio/Audio.h"
#include "../Tween/GroupTween.h"

#include <king/Engine.h>

namespace MinerSpeed
{

//********************************************************************************************************************************
Animation::Animation(King::Engine &engine)
    : mIsRunning(false), mEngine(&engine), mGroupTween(nullptr)
{
    Init();
}

//********************************************************************************************************************************
Animation::~Animation()
{
    delete mGroupTween;
}

//********************************************************************************************************************************
void Animation::Init()
{
    mGroupTween = new MinerSpeed::GroupTween();
    mAudio = new MinerSpeed::Audio();
}

//********************************************************************************************************************************
void Animation::Update()
{
    if (false == IsRunning() || IsCompleted())
    {
        return;
    }
    mGroupTween->Update();
}

//********************************************************************************************************************************
bool Animation::IsRunning()
{
    return mIsRunning;
}

//********************************************************************************************************************************
bool Animation::IsCompleted()
{
    return mGroupTween->IsCompleted();
}

//********************************************************************************************************************************
void Animation::Reset()
{
    mGroupTween->RemoveTweens();
    mIsRunning = false;
}
} // namespace MinerSpeed
