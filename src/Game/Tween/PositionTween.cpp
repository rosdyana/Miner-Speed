#include "PositionTween.h"

#include <glm/gtc/matrix_transform.hpp>

namespace MinerSpeed
{

    //********************************************************************************************************************************
    PositionTween::PositionTween(King::Engine &engine, const King::Engine::Texture &texture, const glm::vec2 &startPos,
                                 const glm::vec2 &endPos, const float duration, const float acceleration)
        : mEngine(&(engine))
        , mTexture(texture)
        , mStartPos(startPos)
        , mEndPos(endPos)
        , mDuration(duration)
        , mAcceleration(acceleration)
        , mTime(0.0f)
        , mCurrentPos(0.0f, 0.0f)
        , mDirectionMask(0.0f, 0.0f)
        , mIsCompleted(false)
    {
    }

    //********************************************************************************************************************************
    PositionTween::~PositionTween()
    {
    }

    //********************************************************************************************************************************
    void PositionTween::Start()
    {
        mCurrentPos = mStartPos;
        if (mEndPos.x == mStartPos.x) {
            mDirectionMask.x = 0.0f;
            mDirectionMask.y = (mEndPos.y > mStartPos.y ? 1.0f : -1.0f);
        } else {
            mDirectionMask.x = (mEndPos.x > mStartPos.x ? 1.0f : -1.0f);
            mDirectionMask.y = 0.0f;
        }
    }

    //********************************************************************************************************************************
    void PositionTween::Update()
    {
        mTime += mEngine->GetLastFrameSeconds();

        float oldVelocity = mCurrentVelocity;
        mCurrentVelocity = oldVelocity + mAcceleration * mTime;

        glm::vec2 newPos(mCurrentPos.x + mDirectionMask.x * ((oldVelocity + mCurrentVelocity) / 2), mCurrentPos.y + mDirectionMask.y * ((oldVelocity + mCurrentVelocity) / 2));

        if ((mDirectionMask.x > 0 && newPos.x > mEndPos.x) || (mDirectionMask.x < 0 && newPos.x < mEndPos.x)) {
            newPos.x = mEndPos.x;
        }
        if ((mDirectionMask.y > 0 && newPos.y > mEndPos.y) || (mDirectionMask.y < 0 && newPos.y < mEndPos.y)) {
            newPos.y = mEndPos.y;
        }

        glm::mat4 transformationA(1.0);
        transformationA = glm::translate(transformationA, glm::vec3(newPos.x, newPos.y, 0.0f));
        mEngine->Render(mTexture, transformationA);

        mCurrentPos = newPos;

        if (mCurrentPos == mEndPos) {
            mIsCompleted = true;
        }
    }

    //********************************************************************************************************************************
    bool PositionTween::IsCompleted()
    {
        return mIsCompleted;
    }
}
