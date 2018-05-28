#pragma once

#include "ITween.h"

#include <king/Engine.h>
#include <glm/glm.hpp>

namespace MinerSpeed
{

class PositionTween : public MinerSpeed::ITween
{

  public:
    PositionTween(King::Engine &engine, const King::Engine::Texture &texture, const glm::vec2 &startPos,
                  const glm::vec2 &endPos, const float duration, const float acceleration);
    virtual ~PositionTween();

  public:
    virtual void Start();
    virtual void Update();
    virtual bool IsCompleted();

  private:
    King::Engine *mEngine;

  private:
    glm::vec2 mStartPos;
    glm::vec2 mEndPos;
    King::Engine::Texture mTexture;
    float mTime;
    float mDuration;
    float mAcceleration;
    float mCurrentVelocity;
    glm::vec2 mCurrentPos;
    glm::vec2 mDirectionMask;
    bool mIsCompleted;
};
} // namespace MinerSpeed
