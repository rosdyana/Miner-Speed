#pragma once

namespace MinerSpeed
{

    class ITween
    {
    public:
        ITween() {};
        virtual ~ITween() {};

    public:
        virtual void Start() = 0;
        virtual void Update() = 0;
        virtual bool IsCompleted() = 0;
    };
} // namespace MinerSpeed
