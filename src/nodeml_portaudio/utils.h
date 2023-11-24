#pragma once

#include <napi.h>
#include <functional>
#include <portaudio.h>

namespace nodeml_portaudio
{

    namespace utils
    {
        void getStreamParameters(PaStreamParameters &params,const Napi::Object object);

        
        Napi::Value inputDataToNapiArray(Napi::Env env, void * data,size_t dataLength,int format);

        void * copyInputData(const void * data,size_t dataLength,int format);

        
        void memCpyFormat(const void * src,void * dest,size_t dataLength,int format);

        void * createFormatPtr(size_t size,int format);

        template <typename T>
        void memCpyTyped(const T * src,const T * dest,size_t numElements);

        Napi::Value copyToTypedArray(Napi::Env env, void * data,size_t dataLength,int format);

        Napi::Value emptyTypedArray(Napi::Env env,size_t dataLength,int format);


        
        void * copyDataFromTypedArray(Napi::TypedArray source,int format);

        void * getTypedArrayDataPtr(Napi::TypedArray source,int format);

        template <typename T>
        void memCpyTyped(const T * src,T * dest,size_t numElements)
        {
            memcpy(dest,src,numElements * sizeof(T));
        }
        
        int sizeOfFormat(int format);

        Napi::Object Init(Napi::Env env, Napi::Object exports);
    }
}
