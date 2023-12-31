#pragma once

#include <napi.h>
#include <functional>
#include <portaudio.h>

namespace nodeml_audio
{
    namespace stream
    {
        namespace formats
        {
            Napi::Object Init(Napi::Env env, Napi::Object exports);
        }
    }
}
