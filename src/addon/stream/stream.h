#pragma once
#include <napi.h>
#include <functional>

namespace nodeml_audio
{
    namespace stream
    {
        Napi::Object Init(Napi::Env env, Napi::Object exports);
    }
}
