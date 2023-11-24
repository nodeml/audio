#pragma once

#include <napi.h>
#include <functional>
#include <portaudio.h>
#include <queue>
#include <addon/utils.h>
#include <addon/ChunkStream.h>

namespace nodeml_audio
{

    struct StreamInfo
    {
        Napi::ThreadSafeFunction tsfn;
        bool bHasBeenAquired = false;
        PaStreamParameters inputParams;
        PaStreamParameters outputParams;
    };

    struct StreamInputInfo
    {
        void *data = nullptr;
        unsigned long frameCount;
        unsigned long totalCount;
        int format;

        StreamInputInfo(const void *dataIn, unsigned long frameCountIn,StreamInfo * streamInfo){

            frameCount = frameCountIn;
            format = streamInfo->inputParams.sampleFormat;
            totalCount = streamInfo->inputParams.channelCount * frameCountIn;
            data = utils::createFormatPtr(totalCount,format);
            utils::memCpyFormat(dataIn,data,totalCount,format);
        }

        ~StreamInputInfo(){
            delete[] data;
        }
    };

    class Stream : public Napi::ObjectWrap<Stream>
    {

    public:
        static Napi::FunctionReference constructor;

        PaStream *stream = NULL;

        StreamInfo *streamInfo = NULL;

        ChunkStream outputQueue;

        bool hasAquiredInputTsfn = false;

        static Napi::Object Init(Napi::Env env, Napi::Object exports);

        Stream(const Napi::CallbackInfo &info);

        static Napi::Object FromPaStream(Napi::Env env, PaStream *paStream, StreamInfo * InStreamInfo);

        static Stream *FromObject(Napi::Value value);

        static Napi::Value Create(const Napi::CallbackInfo &info);

        Napi::Value IsActive(const Napi::CallbackInfo &info);

        Napi::Value Write(const Napi::CallbackInfo &info);

        Napi::Value Start(const Napi::CallbackInfo &info);

        Napi::Value Stop(const Napi::CallbackInfo &info);

        Napi::Value Close(const Napi::CallbackInfo &info);

        void prepare(const Napi::CallbackInfo &info);

        void cleanup();


    };
}