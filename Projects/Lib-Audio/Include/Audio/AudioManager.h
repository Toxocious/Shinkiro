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
#    include <span>
#    include <thread>
#    include <vector>

namespace Shinkiro::Audio
{
    struct SoundData
    {
        std::vector<uint8_t> buffer;
        int                  durationMs = 1500;
    };

    class AUDIO_API AudioManager
    {
    public:
        AudioManager();
        ~AudioManager();

        // Thread-safe sound enqueue
        void PlaySoundAsync( std::vector<uint8_t> soundData, int durationMs = 1500 );

        // Explicit shutdown (optional if you let it auto-cleanup)
        void Shutdown();

    private:
        void WorkerLoop();

        std::thread             m_Thread;
        std::mutex              m_QueueMutex;
        std::condition_variable m_CV;
        std::queue<SoundData>   m_Queue;
        std::atomic<bool>       m_Running = false;

        ma_engine m_Engine;
    };

} // namespace Audio

#endif
