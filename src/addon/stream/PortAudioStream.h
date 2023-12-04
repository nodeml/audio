#pragma once

#include <napi.h>
#include <functional>
#include <portaudio.h>
#include <queue>
#include <addon/utils.h>
#include <addon/stream/ChunkStream.h>

namespace nodeml_audio
{

    namespace stream
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

            StreamInputInfo(const void *dataIn, unsigned long frameCountIn, StreamInfo *streamInfo) : frameCount(frameCountIn),
                                                                                                      format(streamInfo->inputParams.sampleFormat),
                                                                                                      totalCount(streamInfo->inputParams.channelCount * frameCountIn),
                                                                                                      data(utils::allocFormat(streamInfo->inputParams.channelCount * frameCountIn, streamInfo->inputParams.sampleFormat))
            {
                memcpy(data, dataIn, totalCount * utils::sizeOfFormat(format));
            }

            ~StreamInputInfo()
            {
                delete[] data;
            }
        };

        class PortAudioStream : public Napi::ObjectWrap<PortAudioStream>
        {

        public:
            static Napi::FunctionReference constructor;

            PaStream *stream = NULL;

            StreamInfo *streamInfo = NULL;

            ChunkStream outputQueue;

            bool hasAquiredInputTsfn = false;

            static Napi::Object Init(Napi::Env env, Napi::Object exports);

            PortAudioStream(const Napi::CallbackInfo &info);

            static Napi::Object FromPaStream(Napi::Env env, PaStream *paStream, StreamInfo *InStreamInfo);

            static PortAudioStream *FromObject(Napi::Value value);

            static Napi::Value Open(const Napi::CallbackInfo &info);

            Napi::Value IsActive(const Napi::CallbackInfo &info);

            Napi::Value Write(const Napi::CallbackInfo &info);

            Napi::Value Start(const Napi::CallbackInfo &info);

            Napi::Value Stop(const Napi::CallbackInfo &info);

            Napi::Value Close(const Napi::CallbackInfo &info);

            void prepare(const Napi::CallbackInfo &info);

            void cleanup();
        };

    }
}