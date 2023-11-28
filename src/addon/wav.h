#pragma once
#include <napi.h>
#include <functional>
#include <dr_wav.h>

namespace nodeml_audio
{
    namespace wav
    {

        Napi::Value write(const Napi::CallbackInfo &info);

        Napi::Object Init(Napi::Env env, Napi::Object exports);
    }
}
