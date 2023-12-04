#pragma once

#include <napi.h>
#include <functional>
#include <portaudio.h>

namespace nodeml_audio
{

    namespace utils
    {
        void getStreamParameters(PaStreamParameters &params, const Napi::Object object);

        void *allocFormat(size_t size, int format);

        Napi::Value typedArrayForFormat(Napi::Env env, size_t dataLength, int format);

        Napi::Value typedArrayForFormat(Napi::Env env, size_t dataLength, int format, void *data);

        void *copyDataFromTypedArray(Napi::TypedArray source, int format);

        void *getTypedArrayDataPtr(Napi::TypedArray source);

        int sizeOfFormat(int format);

        Napi::Object Init(Napi::Env env, Napi::Object exports);
    }
}
