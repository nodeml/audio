#include <addon/utils.hh>
#include <iostream>

namespace nodeml_audio
{
    namespace utils
    {
        void getStreamParameters(PaStreamParameters &params, const Napi::Object object)
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

        Napi::Value typedArrayForFormat(Napi::Env env, size_t dataLength, int format, void *data)
        {
            auto arr = typedArrayForFormat(env, dataLength, format);

            if (arr.IsTypedArray())
            {
                memcpy(utils::getTypedArrayDataPtr(arr.As<Napi::TypedArray>()), data, dataLength * sizeOfFormat(format));
            }

            return arr;
        }

        Napi::Value typedArrayForFormat(Napi::Env env, size_t dataLength, int format)
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

        int sizeOfFormat(int format)
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

        void *allocFormat(size_t numElements, int format)
        {
            auto byteSize = numElements * sizeOfFormat(format);

            if (byteSize == 0)
            {
                return nullptr;
            }

            return malloc(byteSize);
        }

        void *copyDataFromTypedArray(Napi::TypedArray source, int format)
        {
            auto arrPtr = getTypedArrayDataPtr(source);

            if (arrPtr == nullptr)
            {
                return nullptr;
            }

            void *ptr = allocFormat(source.ElementLength(), format);

            if (ptr == nullptr)
            {
                return nullptr;
            }

            memcpy(ptr, arrPtr, source.ElementLength() * sizeOfFormat(format));

            return ptr;
        }
        void *getTypedArrayDataPtr(Napi::TypedArray source)
        {
            switch (source.TypedArrayType())
            {
            case napi_float32_array:
                return source.As<Napi::TypedArrayOf<float>>().Data();
                break;

            case napi_int32_array:
                return source.As<Napi::TypedArrayOf<int32_t>>().Data();
                break;

            case napi_int16_array:
                return source.As<Napi::TypedArrayOf<int16_t>>().Data();
                break;

            case napi_int8_array:
                return source.As<Napi::TypedArrayOf<int8_t>>().Data();
                break;

            case napi_uint8_array:
                return source.As<Napi::TypedArrayOf<uint8_t>>().Data();
                break;

            default:
                return nullptr;
                break;
            }

            return nullptr;
        }
    }
}
