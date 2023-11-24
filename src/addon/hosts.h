#pragma once

#include <napi.h>
#include <functional>

namespace nodeml_portaudio
{

    namespace hosts
    {
        Napi::Value getHosts(const Napi::CallbackInfo &info);

        Napi::Value getHostDevices(const Napi::CallbackInfo &info);

        Napi::Value getDefaultHostIndex(const Napi::CallbackInfo &info);

        Napi::Object Init(Napi::Env env, Napi::Object exports);
    }
}
