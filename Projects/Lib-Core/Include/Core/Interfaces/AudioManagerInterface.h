#pragma once

#ifndef SHINKIRO_AUDIO_IAUDIOMANAGER_H
#    define SHINKIRO_AUDIO_IAUDIOMANAGER_H

#    include <Core/_Defs.h>

#    include <cstdint>
#    include <memory>
#    include <vector>

namespace Shinkiro::Audio
{
    class AudioManager;

    class CORE_API IAudioManager
    {
    public:
        IAudioManager();
        virtual ~IAudioManager();

        virtual void PlaySoundAsync( std::vector<uint8_t> data, int durationMs = 1500, bool loop = false ) = 0;
        virtual void PlayOST( std::vector<uint8_t> data )                                                  = 0;
        virtual void SetOSTVolume( float volume )                                                          = 0;
        virtual void StopOST()                                                                             = 0;
        virtual void StopAll()                                                                             = 0;
        virtual void Shutdown()                                                                            = 0;
    };
}

#endif
