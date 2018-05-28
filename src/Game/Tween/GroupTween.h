#pragma once

#include "ITween.h"

#include <vector>

namespace MinerSpeed
{

class GroupTween : public ITween
{

  private:
    typedef std::vector<MinerSpeed::ITween *> VPTweens;

  public:
    GroupTween();
    virtual ~GroupTween();

  public:
    void AddTween(ITween *tween);
    void RemoveTweens();

  public:
    virtual void Start();
    virtual void Update();
    virtual bool IsCompleted();

  private:
    std::vector<MinerSpeed::ITween *> mTweens;
};
} // namespace MinerSpeed
