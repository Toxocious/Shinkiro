#pragma once

#ifndef SHINKIRO_AUDIO_AUDIOMANAGERFACTORY_H
#    define SHINKIRO_AUDIO_AUDIOMANAGERFACTORY_H

#    include <Audio/_Defs.h>

#    include <Core/Interfaces/AudioManagerInterface.h>

#    include <memory>

namespace Shinkiro::Audio
{
    AUDIO_API std::unique_ptr<IAudioManager> CreateAudioManager();
}

#endif
