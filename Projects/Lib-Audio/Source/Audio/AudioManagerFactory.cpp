#include <Audio/_Common.h>

#include <Audio/AudioManagerFactory.h>

#include <Audio/AudioManager.h>

namespace Shinkiro::Audio
{
    std::unique_ptr<IAudioManager> CreateAudioManager()
    {
        return std::make_unique<AudioManager>();
    }
}
