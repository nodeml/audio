#pragma once
#include <napi.h>
#include <functional>

namespace nodeml_audio
{
    namespace wav
    {
        Napi::Object Init(Napi::Env env, Napi::Object exports);
    }
}
