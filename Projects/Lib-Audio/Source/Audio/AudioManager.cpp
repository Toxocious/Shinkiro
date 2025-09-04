#include <Audio/_Common.h>

#include <Audio/AudioManager.h>

#include <Log/Log.h>

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio/miniaudio.h>

namespace Shinkiro::Audio
{
    struct AudioManager::AudioImpl
    {
        ma_engine m_Engine;

        bool m_IsInitialized = false;

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

    AudioManager::AudioManager()
        : m_Impl( std::make_unique<AudioImpl>() )
    {
        if ( ma_engine_init( NULL, &m_Impl->m_Engine ) != MA_SUCCESS ||
             m_Impl->m_Engine.pDevice == nullptr || m_Impl->m_Engine.pDevice->pContext == nullptr )
        {
            SHNK_CORE_ERROR( "[AudioManager] Failed to initialize audio engine or device" );
            return;
        }

        m_Impl->m_Running = true;
        m_Impl->m_Thread  = std::thread( &AudioManager::WorkerLoop, this );
    }

    AudioManager::~AudioManager()
    {
        Shutdown();
    }

    void AudioManager::Shutdown()
    {
        if ( !m_Impl->m_Running )
        {
            return;
        }

        m_Impl->m_Running = false;
        m_Impl->m_CV.notify_all();

        if ( m_Impl->m_Thread.joinable() )
        {
            m_Impl->m_Thread.join();
        }

        StopOST();

        ma_engine_uninit( &m_Impl->m_Engine );
    }

    void AudioManager::PlaySoundAsync( std::vector<uint8_t> data, int durationMs, bool loop )
    {
        if ( !m_Impl->m_Running || data.empty() )
        {
            return;
        }

        SoundData sound;
        sound.buffer.assign( data.begin(), data.end() );
        sound.durationMs = durationMs;
        sound.loop       = loop;

        {
            std::lock_guard<std::mutex> lock( m_Impl->m_QueueMutex );
            m_Impl->m_Queue.push( std::move( sound ) );
        }

        m_Impl->m_CV.notify_one();
    }

    void AudioManager::PlayOST( std::vector<uint8_t> data )
    {
        if ( m_Impl->m_OSTPlaying )
        {
            StopOST();
        }

        m_Impl->m_OSTData = std::move( data );

        if ( ma_decoder_init_memory( m_Impl->m_OSTData.data(), m_Impl->m_OSTData.size(), NULL, &m_Impl->m_OSTDecoder ) != MA_SUCCESS )
        {
            SHNK_CORE_ERROR( "[AudioManager] Failed to init OST decoder." );
            return;
        }

        if ( ma_sound_init_from_data_source( &m_Impl->m_Engine, &m_Impl->m_OSTDecoder, MA_SOUND_FLAG_ASYNC, NULL, &m_Impl->m_OSTSound ) != MA_SUCCESS )
        {
            SHNK_CORE_ERROR( "[AudioManager] Failed to init OST sound." );
            ma_decoder_uninit( &m_Impl->m_OSTDecoder );
            return;
        }

        ma_sound_set_looping( &m_Impl->m_OSTSound, MA_TRUE );
        ma_sound_start( &m_Impl->m_OSTSound );
        m_Impl->m_OSTPlaying = true;
    }

    void AudioManager::SetOSTVolume( float volume )
    {
        if ( !m_Impl->m_OSTPlaying )
        {
            return;
        }
        if ( volume < 0.0f )
        {
            volume = 0.0f;
        }

        ma_sound_set_volume( &m_Impl->m_OSTSound, volume );
    }

    void AudioManager::StopOST()
    {
        if ( !m_Impl->m_OSTPlaying )
        {
            return;
        }

        ma_sound_stop( &m_Impl->m_OSTSound );
        ma_sound_uninit( &m_Impl->m_OSTSound );
        ma_decoder_uninit( &m_Impl->m_OSTDecoder );
        m_Impl->m_OSTPlaying = false;
    }

    void AudioManager::StopAll()
    {
        if ( m_Impl->m_Engine.pDevice )
        {
            ma_engine_stop( &m_Impl->m_Engine );
        }
        StopOST();
    }

    void AudioManager::WorkerLoop()
    {
        while ( m_Impl->m_Running )
        {
            SoundData sound;

            {
                std::unique_lock<std::mutex> lock( m_Impl->m_QueueMutex );
                m_Impl->m_CV.wait( lock, [&]()
                                   {
                                       return !m_Impl->m_Queue.empty() || !m_Impl->m_Running;
                                   } );

                if ( !m_Impl->m_Running )
                {
                    break;
                }

                sound = std::move( m_Impl->m_Queue.front() );
                m_Impl->m_Queue.pop();
            }

            ma_decoder decoder;
            ma_sound   maSound;

            if ( ma_decoder_init_memory( sound.buffer.data(), sound.buffer.size(), NULL, &decoder ) != MA_SUCCESS )
            {
                continue;
            }

            if ( ma_sound_init_from_data_source( &m_Impl->m_Engine, &decoder, MA_SOUND_FLAG_ASYNC, NULL, &maSound ) != MA_SUCCESS )
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
                while ( m_Impl->m_Running && ma_sound_is_playing( &maSound ) )
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
