#include "utils.h"
#include <iostream>

namespace nodeml_portaudio
{
    namespace utils
    {
        void GetStreamParameters(PaStreamParameters &params, const Napi::Object object)
        {
            if (object.Has("channelCount"))
            {
                params.channelCount = object.Get("channelCount").ToNumber().Int32Value();
            }

            if (object.Has("device"))
            {
                params.device = object.Get("device").ToNumber().Int32Value();
            }

            if (object.Has("sampleFormat"))
            {
                params.sampleFormat = object.Get("sampleFormat").ToNumber().Int64Value();
            }

            if (object.Has("suggestedLatency"))
            {
                params.suggestedLatency = object.Get("suggestedLatency").ToNumber().DoubleValue();
            }

            params.hostApiSpecificStreamInfo = NULL;
        }

        void *CopyInputData(const void *data, size_t dataLength, int format)
        {
            if (data == nullptr || data == NULL)
            {
                return nullptr;
            }

            auto formatSize = SizeOfFormat(format);

            switch (format)
            {
            case paFloat32:
                return CopyInputData_Internal((float *)(data), dataLength);
                break;

            case paInt32:
                return CopyInputData_Internal((int32_t *)(data), dataLength);
                break;

            case paInt24:
                return nullptr;
                break;

            case paInt16:
                return CopyInputData_Internal((int16_t *)(data), dataLength);
                break;

            case paInt8:
                return CopyInputData_Internal((int8_t *)(data), dataLength);
                break;

            case paUInt8:
                return CopyInputData_Internal((uint8_t *)(data), dataLength);
                break;

            default:
                return nullptr;
                break;
            }
        }

        Napi::Value CopyToTypedArray(Napi::Env env, void *data, size_t dataLength, int format)
        {

            auto formatSize = SizeOfFormat(format);

            Napi::EscapableHandleScope scope(env);

            switch (format)
            {
            case paFloat32:
            {
                auto arr = Napi::TypedArrayOf<float>::New(env, dataLength);
                memcpy(arr.Data(), data, dataLength * formatSize);
                return scope.Escape(arr);
            }
            break;

            case paInt32:
            {
                auto arr = Napi::TypedArrayOf<int32_t>::New(env, dataLength);
                memcpy(arr.Data(), data, dataLength * formatSize);
                return scope.Escape(arr);
            }
            break;

            case paInt24:
                return env.Undefined();
                break;

            case paInt16:
            {
                auto arr = Napi::TypedArrayOf<int16_t>::New(env, dataLength);
                memcpy(arr.Data(), data, dataLength * formatSize);
                return scope.Escape(arr);
            }
            break;

            case paInt8:
            {
                auto arr = Napi::TypedArrayOf<int8_t>::New(env, dataLength);
                memcpy(arr.Data(), data, dataLength * formatSize);
                return scope.Escape(arr);
            }
            break;

            case paUInt8:
            {
                auto arr = Napi::TypedArrayOf<uint8_t>::New(env, dataLength);
                memcpy(arr.Data(), data, dataLength * formatSize);
                return scope.Escape(arr);
            }
            break;

            default:
                return env.Undefined();
                break;
            }
        }

        Napi::Value EmptyTypedArray(Napi::Env env, size_t dataLength, int format)
        {
            Napi::EscapableHandleScope scope(env);

            switch (format)
            {
            case paFloat32:
            {
                auto arr = Napi::TypedArrayOf<float>::New(env, dataLength);

                return scope.Escape(arr);
            }
            break;

            case paInt32:
            {
                auto arr = Napi::TypedArrayOf<int32_t>::New(env, dataLength);

                return scope.Escape(arr);
            }
            break;

            case paInt24:
                return env.Undefined();
                break;

            case paInt16:
            {
                auto arr = Napi::TypedArrayOf<int16_t>::New(env, dataLength);

                return scope.Escape(arr);
            }
            break;

            case paInt8:
            {
                auto arr = Napi::TypedArrayOf<int8_t>::New(env, dataLength);

                return scope.Escape(arr);
            }
            break;

            case paUInt8:
            {
                auto arr = Napi::TypedArrayOf<uint8_t>::New(env, dataLength);

                return scope.Escape(arr);
            }
            break;

            default:
                return env.Undefined();
                break;
            }
        }

        int SizeOfFormat(int format)
        {
            switch (format)
            {
            case paFloat32:
                return sizeof(float);
                break;

            case paInt32:
                return sizeof(int32_t);
                break;

            case paInt24:
                return 0;
                break;

            case paInt16:
                return sizeof(int16_t);
                break;

            case paInt8:
                return sizeof(int8_t);
                break;

            case paUInt8:
                return sizeof(uint8_t);
                break;

            default:
                return 0;
                break;
            }
        }

        Napi::Object Init(Napi::Env env, Napi::Object exports)
        {
            return Napi::Object();
        }
    }
}
