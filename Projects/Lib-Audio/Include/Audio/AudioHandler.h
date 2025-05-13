#pragma once

#ifndef SHINKIRO_AUDIO_AUDIOHANDLER_H
#    define SHINKIRO_AUDIO_AUDIOHANDLER_H

#    include <Audio/_Defs.h>

#    include <Windows.h>
#    include <unordered_map>

namespace Shinkiro::Audio
{
    class AUDIO_API AudioHandler
    {
    public:
        AudioHandler();
        virtual ~AudioHandler();
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
}

#endif
