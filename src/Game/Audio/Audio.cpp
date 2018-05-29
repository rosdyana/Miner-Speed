#include "Audio.h"

#include <iostream>
#include <string>
#include <tuple>

namespace MinerSpeed
{
    const std::vector<std::pair<std::string, int>> mSoundEffects = {
        {"diamond-land.wav", MIX_MAX_VOLUME / 4},
        {"explosion.wav", MIX_MAX_VOLUME},
        {"move-successful.wav", MIX_MAX_VOLUME / 2},
        {"hurryup.wav", MIX_MAX_VOLUME}
    };

    const int mMixChannels = 16;
    const std::string mAssetFolder = "./assets/sfx/";

    //********************************************************************************************************************************
    Audio::Audio()
    {
        if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) != 0) {
            std::cout << "Mix_OpenAudio Error: " << Mix_GetError() << std::endl;
            exit(-1);
        }
        std::string full_path = mAssetFolder + "epicsong.mp3";

        music_ = UniqueMusicPtr{Mix_LoadMUS(full_path.c_str())};

        if (nullptr == music_) {
            std::cout << "Failed to load: " << full_path << ". Error: " << Mix_GetError() << std::endl;
            exit(-1);
        }
        Mix_AllocateChannels(mMixChannels);
        Mix_VolumeMusic(MIX_MAX_VOLUME / 3);
        std::string effect;
        int volume;
        for (auto effect_entry : mSoundEffects) {
            std::tie(effect, volume) = effect_entry;

            full_path = mAssetFolder + effect;

            auto chunk = UniqueChunkPtr{Mix_LoadWAV(full_path.c_str())};

            if (nullptr == chunk) {
                std::cout << "Failed to load: " << full_path << ". Error: " << Mix_GetError() << std::endl;
                exit(-1);
            }
            Mix_VolumeChunk(chunk.get(), volume);
            sound_effects_.push_back(std::move(chunk));
        }
    }

    //********************************************************************************************************************************
    Audio::~Audio() noexcept
    {
        Mix_HaltMusic();
        Mix_HaltChannel(-1);
    }
} // namespace MinerSpeed