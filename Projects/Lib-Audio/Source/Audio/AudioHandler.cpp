#include <Audio/AudioHandler.h>

namespace Shinkiro::Audio
{
    AudioHandler::AudioHandler()
    {
        m_AudioDeviceID = 1;
    }

    AudioHandler::~AudioHandler()
    {
        // Add cleanup if needed
    }
}
