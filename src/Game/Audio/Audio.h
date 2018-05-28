#pragma once
#include <vector>
#include <memory>
#include <utility>
#include <sdl/SDL_mixer.h>

template <typename T, T *func>
struct function_caller
{
    template <typename... Args>
    auto operator()(Args &&... arg) const
    {
        return func(std::forward<Args...>(arg...));
    }
};

namespace MinerSpeed
{
class Audio
{
  public:
    enum SoundFx
    {
        SOUNDMIN,
        DIAMOND_LANDING,
        EXPLOSION,
        MOVE_SUCCESFUL,
        HURRYUP,
        SOUNDMAX
    };

    Audio();
    virtual ~Audio() noexcept;

  public:
    void PlayMusic() const
    {
        Mix_HaltMusic();
        Mix_RewindMusic();
        Mix_FadeInMusic(music_.get(), -1, 500);
    }
    void FadeoutMusic(int ms) const
    {
        Mix_FadeOutMusic(ms);
    }

    void StopMusic() const
    {
        Mix_HaltMusic();
    }

    void PlaySound(SoundFx effect, int time_in_ms = -1) const
    {
        Mix_PlayChannelTimed(-1, sound_effects_.at(effect).get(), 0, time_in_ms);
    }

    void StopSound() const
    {
        Mix_HaltChannel(-1);
    }

  private:
    using UniqueMusicPtr = std::unique_ptr<Mix_Music, function_caller<void(Mix_Music *), &Mix_FreeMusic>>;
    using UniqueChunkPtr = std::unique_ptr<Mix_Chunk, function_caller<void(Mix_Chunk *), &Mix_FreeChunk>>;

    UniqueMusicPtr music_;
    std::vector<UniqueChunkPtr> sound_effects_;
};
} // namespace MinerSpeed
