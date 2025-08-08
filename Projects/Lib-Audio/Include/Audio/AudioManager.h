#pragma once

#ifndef SHINKIRO_AUDIO_AUDIOHANDLER_H
#    define SHINKIRO_AUDIO_AUDIOHANDLER_H

#    include <Audio/_Defs.h>

#    include <miniaudio/miniaudio.h>

#    include <atomic>
#    include <condition_variable>
#    include <cstdint>
#    include <iostream>
#    include <mutex>
#    include <queue>
#    include <thread>
#    include <vector>

namespace Shinkiro::Audio
{
    struct SoundData
    {
        std::vector<uint8_t> buffer;
        int                  durationMs = 1500;
        bool                 loop       = false;
    };

    class AUDIO_API AudioManager
    {
    public:
        AudioManager();
        ~AudioManager();

        void PlaySoundAsync( std::vector<uint8_t> data, int durationMs = 1500, bool loop = false );
        void PlayOST( std::vector<uint8_t> data );
        void SetOSTVolume( float volume );

        void StopOST();
        void StopAll();

        void Shutdown();

    private:
        void WorkerLoop();

    private:
        ma_engine m_Engine;

        std::thread             m_Thread;
        std::mutex              m_QueueMutex;
        std::condition_variable m_CV;
        std::queue<SoundData>   m_Queue;
        std::atomic<bool>       m_Running = false;

        std::vector<uint8_t> m_OSTData;
        ma_sound             m_OSTSound {};
        ma_decoder           m_OSTDecoder {};
        bool                 m_OSTPlaying = false;
    };
}

#endif
