#pragma once

namespace MinerSpeed
{
    class GroupTween;
    class Audio;
} // namespace MinerSpeed

namespace King
{
    class Engine;
}

namespace MinerSpeed
{

    class Animation
    {
    public:
        Animation(King::Engine &engine);
        virtual ~Animation();

    public:
        void Init();
        virtual void Update();
        bool IsRunning();
        bool IsCompleted();
        void Reset();

    protected:
        bool mIsRunning;
        King::Engine *mEngine;
        MinerSpeed::GroupTween *mGroupTween;
        MinerSpeed::Audio *mAudio;
    };
} // namespace MinerSpeed
