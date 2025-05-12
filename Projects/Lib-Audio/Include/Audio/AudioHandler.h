#pragma once

#include <Windows.h>
#include <unordered_map>

#ifdef PLATFORM_WINDOWS
#    ifdef SHINKIRO_AUDIO_EXPORTS
#        define AUDIO_API __declspec( dllexport )
#    else
#        define AUDIO_API __declspec( dllimport )
#    endif
#else
#    define AUDIO_API
#endif

class AUDIO_API AudioHandler
{
public:
    AudioHandler();
    ~AudioHandler();
    AudioHandler( const AudioHandler & )             = delete;
    AudioHandler & operator=( const AudioHandler & ) = delete;

private:
    int GetAudioDeviceID() const
    {
        return m_AudioDeviceID;
    }

private:
    int m_AudioDeviceID { 0 };
};
