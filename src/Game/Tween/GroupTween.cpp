#include "GroupTween.h"

namespace MinerSpeed
{

    //********************************************************************************************************************************
    GroupTween::GroupTween()
    {
    }

    //********************************************************************************************************************************
    GroupTween::~GroupTween()
    {
        RemoveTweens();
    }

    //********************************************************************************************************************************
    void GroupTween::AddTween(ITween *tween)
    {
        mTweens.push_back(tween);
    }

    //********************************************************************************************************************************
    void GroupTween::RemoveTweens()
    {
        for (VPTweens::iterator it = mTweens.begin(); it != mTweens.end(); ++it) {
            delete *it;
        }
        mTweens.clear();
    }

    //********************************************************************************************************************************
    void GroupTween::Start()
    {
        for (VPTweens::iterator it = mTweens.begin(); it != mTweens.end(); ++it) {
            (*it)->Start();
        }
    }

    //********************************************************************************************************************************
    void GroupTween::Update()
    {
        for (VPTweens::iterator it = mTweens.begin(); it != mTweens.end(); ++it) {
            (*it)->Update();
        }
    }

    //********************************************************************************************************************************
    bool GroupTween::IsCompleted()
    {
        bool retVal = true;
        for (VPTweens::iterator it = mTweens.begin(); it != mTweens.end(); ++it) {
            if (false == (*it)->IsCompleted()) {
                retVal = false;
                break;
            }
        }
        return retVal;
    }
} // namespace MinerSpeed
