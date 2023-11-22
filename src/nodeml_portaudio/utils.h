#pragma once

#include <napi.h>
#include <functional>
#include <portaudio.h>

namespace nodeml_portaudio
{

    namespace utils
    {
        void GetStreamParameters(PaStreamParameters &params,const Napi::Object object);

        
        Napi::Value InputDataToNapiArray(Napi::Env env, void * data,size_t dataLength,int format);

        void * CopyInputData(const void * data,size_t dataLength,int format);

        template <typename T>
        void * CopyInputData_Internal(const T * data,size_t dataLength);

        Napi::Value CopyToTypedArray(Napi::Env env, void * data,size_t dataLength,int format);

        Napi::Value EmptyTypedArray(Napi::Env env,size_t dataLength,int format);


        template <typename T>
        void CopyFromTypedArray(Napi::TypedArrayOf<T> source,T * dest);

        template <typename T>
        void *CopyInputData_Internal(const T *data, size_t dataLength)
        {
            T* newData = new T[dataLength];

            for(size_t i = 0; i < dataLength; i++){
                *newData = *data++;
            }

            std::cout << "Copied " << dataLength << " Elements" << std::endl;
            return newData;
        }

        template <typename T>
        void CopyFromTypedArray(Napi::TypedArrayOf<T> source, T *dest)
        {
            auto sourceStart = source.Data();
            auto destStart = dest;

            std::cout << "Converting typed Array to C++ Array" << std::endl;
            for(int i = 0; i < source.ElementLength(); i++){
                dest[i] = sourceStart[i];
            }
        }
        
        int SizeOfFormat(int format);

        Napi::Object Init(Napi::Env env, Napi::Object exports);
    }
}
