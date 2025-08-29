#pragma once

#ifndef SHINKIRO_AUDIO_AUDIOHANDLER_H
#    define SHINKIRO_AUDIO_AUDIOHANDLER_H

#    include <Audio/_Defs.h>

#    include <miniaudio/miniaudio.h>

#    include <atomic>
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

    public:
        /**
         * @brief Plays a sound asynchronously.
         * @param data The sound data as a byte vector.
         * @param durationMs The duration to play the sound in milliseconds.
         * @param loop Whether to loop the sound.
         */
        void PlaySoundAsync( std::vector<uint8_t> data, int durationMs = 1500, bool loop = false );

        /**
         * @brief Plays the original soundtrack (OST) in the background. Allows async sounds to be played over it.
         * @param data The sound data as a byte vector.
         */
        void PlayOST( std::vector<uint8_t> data );

        /**
         * @brief Sets the volume for the original soundtrack (OST).
         * @param volume The volume level (0.0 to 1.0).
         */
        void SetOSTVolume( float volume );

        /**
         * @brief Stops the original soundtrack (OST).
         */
        void StopOST();

        /**
         * @brief Stops all currently playing sounds.
         */
        void StopAll();

        /**
         * @brief Shuts down the audio manager and cleans up resources. Stops all sounds if any are playing.
         */
        void Shutdown();

    private:
        /**
         * @brief The worker loop that processes the sound queue.
         */
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
