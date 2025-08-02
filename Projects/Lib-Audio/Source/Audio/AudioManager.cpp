#include <Audio/AudioManager.h>

namespace Shinkiro::Audio
{
    AudioManager::AudioManager()
    {
        if ( ma_engine_init( NULL, &m_Engine ) != MA_SUCCESS ||
             m_Engine.pDevice == nullptr || m_Engine.pDevice->pContext == nullptr )
        {
            std::cerr << "[AudioManager] Failed to initialize audio engine.\n";
            return;
        }

        m_Running = true;
        m_Thread  = std::thread( &AudioManager::WorkerLoop, this );
    }

    AudioManager::~AudioManager()
    {
        Shutdown();
    }

    void AudioManager::Shutdown()
    {
        if ( !m_Running )
        {
            return;
        }

        m_Running = false;
        m_CV.notify_all();

        if ( m_Thread.joinable() )
        {
            m_Thread.join();
        }

        ma_engine_uninit( &m_Engine );
    }

    void AudioManager::PlaySoundAsync( std::vector<uint8_t> soundData, int durationMs )
    {
        if ( !m_Running || soundData.empty() )
        {
            return;
        }

        SoundData sound;
        sound.buffer.assign( soundData.begin(), soundData.end() );
        sound.durationMs = durationMs;

        {
            std::lock_guard<std::mutex> lock( m_QueueMutex );
            m_Queue.push( std::move( sound ) );
        }
        m_CV.notify_one();
    }

    void AudioManager::WorkerLoop()
    {
        while ( m_Running )
        {
            SoundData sound;

            {
                std::unique_lock<std::mutex> lock( m_QueueMutex );
                m_CV.wait( lock, [&]()
                           {
                               return !m_Queue.empty() || !m_Running;
                           } );

                if ( !m_Running )
                {
                    break;
                }

                sound = std::move( m_Queue.front() );
                m_Queue.pop();
            }

            ma_decoder decoder;
            ma_sound   maSound;

            if ( ma_decoder_init_memory( sound.buffer.data(), sound.buffer.size(), NULL, &decoder ) != MA_SUCCESS )
            {
                continue;
            }

            if ( ma_sound_init_from_data_source( &m_Engine, &decoder, MA_SOUND_FLAG_ASYNC, NULL, &maSound ) != MA_SUCCESS )
            {
                ma_decoder_uninit( &decoder );
                continue;
            }

            ma_sound_start( &maSound );
            std::this_thread::sleep_for( std::chrono::milliseconds( sound.durationMs ) );

            ma_sound_uninit( &maSound );
            ma_decoder_uninit( &decoder );
        }
    }
}
