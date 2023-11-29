#include <addon/stream/PortAudioStream.h>
#include <addon/utils.h>
#include <future>
#include "Stream.h"
#include <iostream>

namespace nodeml_audio
{
    namespace stream
    {
        Napi::FunctionReference PortAudioStream::constructor;

        Napi::Object PortAudioStream::Init(Napi::Env env, Napi::Object exports)
        {
            auto func = DefineClass(env, "Stream",
                                    {PortAudioStream::InstanceMethod("write", &PortAudioStream::Write),
                                     PortAudioStream::InstanceMethod("start", &PortAudioStream::Start),
                                     PortAudioStream::InstanceMethod("stop", &PortAudioStream::Stop),
                                     PortAudioStream::InstanceMethod("close", &PortAudioStream::Close)});

            constructor = Napi::Persistent(func);
            constructor.SuppressDestruct();
            // exports.Set("Stream", func);
            exports.Set("open", Napi::Function::New(env, &PortAudioStream::Open));
            return exports;
        }

        PortAudioStream::PortAudioStream(const Napi::CallbackInfo &info) : ObjectWrap(info)
        {
            stream = NULL;
            streamInfo = NULL;
        }

        PortAudioStream *PortAudioStream::FromObject(Napi::Value value)
        {
            return Napi::ObjectWrap<PortAudioStream>::Unwrap(value.ToObject());
        }

        Napi::Value PortAudioStream::Open(const Napi::CallbackInfo &info)

        {
            auto env = info.Env();

            try
            {
                Napi::EscapableHandleScope scope(env);

                auto newStream = PortAudioStream::constructor.New({});

                auto unwraped = Napi::ObjectWrap<PortAudioStream>::Unwrap(newStream);

                unwraped->prepare(info);

                return scope.Escape(newStream).ToObject();
            }
            catch (const std::exception &e)
            {
                throw Napi::Error::New(env, e.what());
            }
        }
        Napi::Value PortAudioStream::IsActive(const Napi::CallbackInfo &info)
        {
            auto env = info.Env();

            try
            {
                return Napi::Boolean::New(env, Pa_IsStreamActive(stream));
            }
            catch (const std::exception &e)
            {
                throw Napi::Error::New(env, e.what());
            }
        }

        Napi::Value PortAudioStream::Write(const Napi::CallbackInfo &info)
        {
            auto env = info.Env();

            if (streamInfo->outputParams.device == paNoDevice)
            {
                return Napi::Boolean::New(env, false);
            }

            try
            {
                auto arr = info[0].As<Napi::TypedArray>();

                auto ptr = utils::copyDataFromTypedArray(arr, streamInfo->outputParams.sampleFormat);

                if (ptr != nullptr)
                {
                    outputQueue.Put(ptr, arr.ElementLength());

                    return Napi::Boolean::New(env, true);
                }
            }
            catch (const std::exception &e)
            {
                throw Napi::Error::New(env, e.what());
            }

            return Napi::Boolean::New(env, false);
        }

        Napi::Value PortAudioStream::Start(const Napi::CallbackInfo &info)
        {
            auto env = info.Env();

            auto startResult = Pa_StartStream(stream);

            if (startResult != paNoError)
            {
                throw Napi::Error::New(env, Pa_GetErrorText(startResult));
            }

            return env.Undefined();
        }
        Napi::Value PortAudioStream::Stop(const Napi::CallbackInfo &info)
        {
            auto env = info.Env();

            auto stopResult = Pa_StopStream(stream);

            if (stopResult != paNoError)
            {
                throw Napi::Error::New(env, Pa_GetErrorText(stopResult));
            }

            return env.Undefined();
        }

        Napi::Value PortAudioStream::Close(const Napi::CallbackInfo &info)
        {

            if(Pa_IsStreamActive(stream)){
                Stop(info);
            }

            auto env = info.Env();

            auto closeResult = Pa_CloseStream(stream);

            if (closeResult != paNoError)
            {
                throw Napi::Error::New(env, Pa_GetErrorText(closeResult));
            }

            stream = NULL;

            try
            {
                cleanup();
            }
            catch (const std::exception &e)
            {
                throw Napi::Error::New(env, e.what());
            }

            return env.Undefined();
        }

        void PortAudioStream::prepare(const Napi::CallbackInfo &info)
        {
            cleanup();
            auto env = info.Env();

            try
            {
                streamInfo = new StreamInfo();

                PaStreamParameters inputParams;
                PaStreamParameters outputParams;
                inputParams.device = paNoDevice;
                outputParams.device = paNoDevice;

                if (info[0].IsObject())
                {
                    utils::getStreamParameters(inputParams, info[0].ToObject());
                    auto callback = info[0].ToObject().Get("callback").As<Napi::Function>();

                    streamInfo->tsfn = Napi::ThreadSafeFunction::New(
                        env, callback, "StreamCallback", 0, 1, [](Napi::Env env, StreamInfo *pAudioDataCreated)
                        { delete pAudioDataCreated; },
                        streamInfo);

                    streamInfo->bHasBeenAquired = false;
                }

                if (info[1].IsObject())
                    utils::getStreamParameters(outputParams, info[1].ToObject());

                if (inputParams.device == outputParams.device && inputParams.device == paNoDevice)
                {
                    throw Napi::Error::New(env, "Cannot open a stream with no input or output");
                }

                auto sampleRate = info[2].ToNumber().DoubleValue();

                unsigned long framesPerBuffer = info[3].ToNumber().Int64Value();

                streamInfo->inputParams = inputParams;
                streamInfo->outputParams = outputParams;

                outputQueue.SetFormat(outputParams.sampleFormat);
                outputQueue.SetChannels(outputParams.channelCount);

                auto openResult = Pa_OpenStream(
                    &stream, inputParams.device == paNoDevice ? NULL : &inputParams, outputParams.device == paNoDevice ? NULL : &outputParams,
                    sampleRate,
                    framesPerBuffer,
                    paNoFlag, [](const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
                    {
                        auto owner = (PortAudioStream *)userData;

                        auto numInputData = frameCount * owner->streamInfo->inputParams.channelCount;

                        if (input != nullptr)
                        {

                            if (!owner->hasAquiredInputTsfn)
                            {
                                owner->streamInfo->tsfn.Acquire();
                                owner->hasAquiredInputTsfn = true;
                            }

                            auto streamInputInfo = new StreamInputInfo(input, frameCount, owner->streamInfo);

                            owner->streamInfo->tsfn.NonBlockingCall(streamInputInfo, [](Napi::Env env, Napi::Function jsCallback, StreamInputInfo *value)
                                                                    {

                            jsCallback.Call({utils::copyToTypedArray(env,value->data,value->totalCount,value->format), Napi::Number::New(env,value->frameCount)});

                            delete value; });
                        }

                        if (output != nullptr)
                        {
                            auto dataGiven = owner->outputQueue.GetData(output, owner->streamInfo->outputParams.channelCount * frameCount);
                            // std::cout << "Sent " << dataGiven << " Elements" << std::endl;
                        }

                        return 0; // returnCode;
                    },

                    this);

                if (openResult != paNoError)
                {
                    delete streamInfo;
                    throw Napi::Error::New(env, Pa_GetErrorText(openResult));
                }
            }
            catch (const std::exception &e)
            {
                throw Napi::Error::New(env, e.what());
            }
        }

        void PortAudioStream::cleanup()
        {
            if (streamInfo != NULL)
            {

                if (streamInfo->bHasBeenAquired)
                {
                    streamInfo->tsfn.Release();
                }

                delete streamInfo;

                streamInfo = NULL;
            }
        }

    }
}
