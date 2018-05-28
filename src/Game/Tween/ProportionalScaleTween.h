#pragma once

#include "ITween.h"

#include <king/Engine.h>
#include <glm/glm.hpp>

namespace MinerSpeed
{

class ProportionalScaleTween : public MinerSpeed::ITween
{

  public:
    ProportionalScaleTween(King::Engine &engine, const King::Engine::Texture &texture, const glm::vec2 &pos, const float startScale,
                           const float endScale, const float duration, const float acceleration);
    virtual ~ProportionalScaleTween();

  public:
    virtual void Start();
    virtual void Update();
    virtual bool IsCompleted();

  private:
    King::Engine *mEngine;

  private:
    glm::vec2 mPos;
    King::Engine::Texture mTexture;
    float mStartScale;
    float mEndScale;
    float mTime;
    float mDuration;
    float mAcceleration;
    float mCurrentVelocity;
    float mCurrentScale;
    float mScaleDirectionMask;
    bool mIsCompleted;
};
} // namespace MinerSpeed
