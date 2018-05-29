#include "ProportionalScaleTween.h"

#include <glm/gtc/matrix_transform.hpp>

namespace MinerSpeed
{

    //********************************************************************************************************************************
    ProportionalScaleTween::ProportionalScaleTween(King::Engine &engine, const King::Engine::Texture &texture, const glm::vec2 &pos, const float startScale,
            const float endScale, const float duration, const float acceleration)
        : mEngine(&(engine)), mTexture(texture), mPos(pos), mStartScale(startScale), mEndScale(endScale), mDuration(duration), mAcceleration(acceleration), mTime(0.0f), mCurrentScale(0.0f), mScaleDirectionMask(0), mIsCompleted(false)
    {
    }

    //********************************************************************************************************************************
    ProportionalScaleTween::~ProportionalScaleTween()
    {
    }

    //********************************************************************************************************************************
    void ProportionalScaleTween::Start()
    {
        mCurrentScale = mStartScale;
        if (mEndScale > mStartScale) {
            mScaleDirectionMask = 1.0f;
        } else {
            mScaleDirectionMask = -1.0f;
        }
    }

    //********************************************************************************************************************************
    void ProportionalScaleTween::Update()
    {
        mTime += mEngine->GetLastFrameSeconds();
        float oldVelocity = mCurrentVelocity;
        mCurrentVelocity = oldVelocity + mAcceleration * mTime;

        int textWidth = mEngine->GetTextureWidth(mTexture);
        int textHeight = mEngine->GetTextureHeight(mTexture);

        float newScale = mCurrentScale + mScaleDirectionMask * ((oldVelocity + mCurrentVelocity) / 2);

        if ((mScaleDirectionMask > 0 && newScale > mEndScale) || (mScaleDirectionMask < 0 && newScale < mEndScale)) {
            newScale = mEndScale;
        }

        glm::mat4 transformation(1.0);
        transformation = glm::translate(transformation, glm::vec3(mPos.x, mPos.y, 0.0f));

        transformation = glm::translate(transformation, glm::vec3(textWidth * 0.5f, textHeight * 0.5f, 0.0f));
        transformation = glm::scale(transformation, glm::vec3(newScale, newScale, 0.0f));
        transformation = glm::translate(transformation, glm::vec3(textWidth * -0.5f, textHeight * -0.5f, 0.0f));

        mEngine->Render(mTexture, transformation);

        mCurrentScale = newScale;

        if (mCurrentScale == mEndScale) {
            mIsCompleted = true;
        }
    }

    //********************************************************************************************************************************
    bool ProportionalScaleTween::IsCompleted()
    {
        return mIsCompleted;
    }
} // namespace MinerSpeed
