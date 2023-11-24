#include "utils.h"
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

        void *copyInputData(const void *data, size_t dataLength, int format)
        {
            if (data == nullptr || data == NULL)
            {
                return nullptr;
            }

            auto formatSize = sizeOfFormat(format);

            // switch (format)
            // {
            // case paFloat32:
            //     return memCpyTyped((float *)(data), dataLength);
            //     break;

            // case paInt32:
            //     return memCpyTyped((int32_t *)(data), dataLength);
            //     break;

            // case paInt24:
            //     return nullptr;
            //     break;

            // case paInt16:
            //     return memCpyTyped((int16_t *)(data), dataLength);
            //     break;

            // case paInt8:
            //     return memCpyTyped((int8_t *)(data), dataLength);
            //     break;

            // case paUInt8:
            //     return memCpyTyped((uint8_t *)(data), dataLength);
            //     break;

            // default:
            //     return nullptr;
            //     break;
            // }

            return nullptr;
        }

        Napi::Value copyToTypedArray(Napi::Env env, void *data, size_t dataLength, int format)
        {

            auto formatSize = sizeOfFormat(format);

            Napi::Value result = env.Undefined();

            void *arrDataPtr = nullptr;

            Napi::EscapableHandleScope scope(env);

            switch (format)
            {
            case paFloat32:
            {
                auto arr = Napi::TypedArrayOf<float>::New(env, dataLength);
                arrDataPtr = arr.Data();
                result = arr;
            }
            break;

            case paInt32:
            {
                auto arr = Napi::TypedArrayOf<int32_t>::New(env, dataLength);
                arrDataPtr = arr.Data();
                result = arr;
            }
            break;

            case paInt24:
                break;

            case paInt16:
            {
                auto arr = Napi::TypedArrayOf<int16_t>::New(env, dataLength);
                arrDataPtr = arr.Data();
                result = arr;
            }
            break;

            case paInt8:
            {
                auto arr = Napi::TypedArrayOf<int8_t>::New(env, dataLength);
                arrDataPtr = arr.Data();
                result = arr;
            }
            break;

            case paUInt8:
            {
                auto arr = Napi::TypedArrayOf<uint8_t>::New(env, dataLength);
                arrDataPtr = arr.Data();
                result = arr;
            }
            break;

            default:
                break;
            }

            if (arrDataPtr != nullptr)
            {

                memcpy(arrDataPtr, data, dataLength * formatSize);

                // float* ptr2 = (float *)arrDataPtr;

                // std::cout << "COPYING DATA " << dataLength << std::endl;
                // for(int i = 0; i < 10; i++){
                //     std::cout << "DATA VALUES : " << *ptr2<< std::endl;
                //     ptr2++;
                // }
                

                return scope.Escape(result);
            }

            return result;
        }

        Napi::Value emptyTypedArray(Napi::Env env, size_t dataLength, int format)
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

        void memCpyFormat(const void *src, void *dest, size_t dataLength, int format)
        {
            switch (format)
            {
            case paFloat32:
                memCpyTyped((float *)src, (float *)dest, dataLength);
                break;

            case paInt32:
                memCpyTyped((int32_t *)src, (int32_t *)dest, dataLength);
                break;

            case paInt24:
                break;

            case paInt16:
                memCpyTyped((int16_t *)src, (int16_t *)dest, dataLength);
                break;

            case paInt8:
                memCpyTyped((int8_t *)src, (int8_t *)dest, dataLength);
                break;

            case paUInt8:
                memCpyTyped((uint8_t *)src, (uint8_t *)dest, dataLength);
                break;

            default:

                break;
            }
        }

        void *createFormatPtr(size_t size, int format)
        {
            switch (format)
            {
            case paFloat32:
                return new float[size];
                break;

            case paInt32:
                return new int32_t[size];
                break;

            case paInt24:
                return nullptr;
                break;

            case paInt16:
                return new int16_t[size];
                break;

            case paInt8:
                return new int8_t[size];
                break;

            case paUInt8:
                return new uint8_t[size];
                break;

            default:
                return nullptr;
                break;
            }
        }

        void *copyDataFromTypedArray(Napi::TypedArray source, int format)
        {
            auto arrPtr = getTypedArrayDataPtr(source, format);

            if (arrPtr == nullptr)
            {
                return nullptr;
            }

            void *ptr = nullptr;

            switch (format)
            {
            case paFloat32:

                ptr = new float[source.ElementLength()];
                break;

            case paInt32:
                ptr = new int32_t[source.ElementLength()];
                break;

            case paInt24:
                return nullptr;
                break;

            case paInt16:
                ptr = new int16_t[source.ElementLength()];
                break;

            case paInt8:
                ptr = new int8_t[source.ElementLength()];
                break;

            case paUInt8:
                ptr = new uint8_t[source.ElementLength()];
                break;

            default:
                break;
            }

            memCpyFormat(arrPtr, ptr, source.ElementLength(), format);

            return ptr;
        }
        void *getTypedArrayDataPtr(Napi::TypedArray source, int format)
        {
            switch (format)
            {
            case paFloat32:
                return source.As<Napi::TypedArrayOf<float>>().Data();
                break;

            case paInt32:
                return source.As<Napi::TypedArrayOf<int32_t>>().Data();
                break;

            case paInt24:
                return nullptr;
                break;

            case paInt16:
                return source.As<Napi::TypedArrayOf<int16_t>>().Data();
                break;

            case paInt8:
                return source.As<Napi::TypedArrayOf<int8_t>>().Data();
                break;

            case paUInt8:
                return source.As<Napi::TypedArrayOf<uint8_t>>().Data();
                break;

            default:
                return nullptr;
                break;
            }
        }
    }
}
