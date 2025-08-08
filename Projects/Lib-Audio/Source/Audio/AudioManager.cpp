#include <Audio/AudioManager.h>

namespace Shinkiro::Audio
{
    AudioManager::AudioManager()
    {
        if ( ma_engine_init( NULL, &m_Engine ) != MA_SUCCESS ||
             m_Engine.pDevice == nullptr || m_Engine.pDevice->pContext == nullptr )
        {
            std::cerr << "[AudioManager] Failed to initialize audio engine or device.\n";
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

        StopOST();

        ma_engine_uninit( &m_Engine );
    }

    void AudioManager::PlaySoundAsync( std::vector<uint8_t> data, int durationMs, bool loop )
    {
        if ( !m_Running || data.empty() )
        {
            return;
        }

        SoundData sound;
        sound.buffer.assign( data.begin(), data.end() );
        sound.durationMs = durationMs;
        sound.loop       = loop;

        {
            std::lock_guard<std::mutex> lock( m_QueueMutex );
            m_Queue.push( std::move( sound ) );
        }

        m_CV.notify_one();
    }

    void AudioManager::PlayOST( std::vector<uint8_t> data )
    {
        if ( m_OSTPlaying )
        {
            StopOST();
        }

        m_OSTData = std::move( data );

        if ( ma_decoder_init_memory( m_OSTData.data(), m_OSTData.size(), NULL, &m_OSTDecoder ) != MA_SUCCESS )
        {
            std::cerr << "[AudioManager] Failed to init OST decoder.\n";
            return;
        }

        if ( ma_sound_init_from_data_source( &m_Engine, &m_OSTDecoder, MA_SOUND_FLAG_ASYNC, NULL, &m_OSTSound ) != MA_SUCCESS )
        {
            std::cerr << "[AudioManager] Failed to init OST sound.\n";
            ma_decoder_uninit( &m_OSTDecoder );
            return;
        }

        ma_sound_set_looping( &m_OSTSound, MA_TRUE );
        ma_sound_start( &m_OSTSound );
        m_OSTPlaying = true;
    }

    void AudioManager::SetOSTVolume( float volume )
    {
        if ( !m_OSTPlaying )
        {
            return;
        }
        if ( volume < 0.0f )
        {
            volume = 0.0f;
        }

        ma_sound_set_volume( &m_OSTSound, volume );
    }

    void AudioManager::StopOST()
    {
        if ( !m_OSTPlaying )
        {
            return;
        }

        ma_sound_stop( &m_OSTSound );
        ma_sound_uninit( &m_OSTSound );
        ma_decoder_uninit( &m_OSTDecoder );
        m_OSTPlaying = false;
    }

    void AudioManager::StopAll()
    {
        if ( m_Engine.pDevice )
        {
            ma_engine_stop( &m_Engine ); // Stops all active sounds
        }
        StopOST();
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

            if ( sound.loop )
            {
                ma_sound_set_looping( &maSound, MA_TRUE );
            }

            ma_sound_start( &maSound );

            if ( sound.loop )
            {
                while ( m_Running && ma_sound_is_playing( &maSound ) )
                {
                    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                }
            }
            else
            {
                std::this_thread::sleep_for( std::chrono::milliseconds( sound.durationMs ) );
            }

            ma_sound_stop( &maSound );
            ma_sound_uninit( &maSound );
            ma_decoder_uninit( &decoder );
        }
    }
}
