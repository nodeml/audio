#include <addon/Stream.h>
#include <addon/utils.h>
#include <future>
#include "Stream.h"
#include <iostream>

namespace nodeml_audio
{

    Napi::FunctionReference Stream::constructor;

    Napi::Object Stream::Init(Napi::Env env, Napi::Object exports)
    {
        auto func = DefineClass(env, "Stream",
                                {Stream::InstanceMethod("write", &Stream::Write),
                                 Stream::InstanceMethod("start", &Stream::Start),
                                 Stream::InstanceMethod("stop", &Stream::Stop),
                                 Stream::InstanceMethod("close", &Stream::Close)});

        constructor = Napi::Persistent(func);
        constructor.SuppressDestruct();
        exports.Set("Stream", func);
        exports.Set("createStream",Napi::Function::New(env,&Stream::Create));
        return exports;
    }

    Stream::Stream(const Napi::CallbackInfo &info) : ObjectWrap(info)
    {
        stream = NULL;
        streamInfo = NULL;
    }

    Stream *Stream::FromObject(Napi::Value value)
    {
        return Napi::ObjectWrap<Stream>::Unwrap(value.ToObject());
    }

    Napi::Value Stream::Create(const Napi::CallbackInfo &info)

    {
        auto env = info.Env();

        try
        {
            Napi::EscapableHandleScope scope(env);

            auto newStream = Stream::constructor.New({});

            auto unwraped = Napi::ObjectWrap<Stream>::Unwrap(newStream);

            unwraped->prepare(info);

            return scope.Escape(newStream).ToObject();
        }
        catch (const std::exception &e)
        {
            throw Napi::Error::New(env, e.what());
        }
    }
    Napi::Value Stream::IsActive(const Napi::CallbackInfo &info)
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

    Napi::Value Stream::Write(const Napi::CallbackInfo &info){
        auto env = info.Env();

        if(streamInfo->outputParams.device == paNoDevice){
            return Napi::Boolean::New(env,false);
        }

        try
        {
            auto arr = info[0].As<Napi::TypedArray>();

            auto ptr = utils::copyDataFromTypedArray(arr,streamInfo->outputParams.sampleFormat);

            if(ptr != nullptr){
                outputQueue.Put(ptr,arr.ElementLength());

                return Napi::Boolean::New(env,true);
            }
        }
        catch (const std::exception &e)
        {
            throw Napi::Error::New(env, e.what());
        }

        return Napi::Boolean::New(env,false);
    }

    Napi::Value Stream::Start(const Napi::CallbackInfo &info)
    {
        auto env = info.Env();

        auto startResult = Pa_StartStream(stream);

        if (startResult != paNoError)
        {
            throw Napi::Error::New(env, Pa_GetErrorText(startResult));
        }

        return env.Undefined();
    }
    Napi::Value Stream::Stop(const Napi::CallbackInfo &info)
    {
        auto env = info.Env();

        auto stopResult = Pa_StopStream(stream);

        if (stopResult != paNoError)
        {
            throw Napi::Error::New(env, Pa_GetErrorText(stopResult));
        }

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

    Napi::Value Stream::Close(const Napi::CallbackInfo &info)
    {
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

    void Stream::prepare(const Napi::CallbackInfo &info)
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

            if (info[0].IsObject()){
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
                    

                    auto owner = (Stream *)userData;

                    auto numInputData = frameCount * owner->streamInfo->inputParams.channelCount;

                    if(input != nullptr){

                        if(!owner->hasAquiredInputTsfn){
                            owner->streamInfo->tsfn.Acquire();
                            owner->hasAquiredInputTsfn = true;
                        }

                        auto streamInputInfo = new StreamInputInfo(input,frameCount,owner->streamInfo);

                        owner->streamInfo->tsfn.NonBlockingCall(streamInputInfo,[](Napi::Env env, Napi::Function jsCallback, StreamInputInfo * value){

                            jsCallback.Call({utils::copyToTypedArray(env,value->data,value->totalCount,value->format), Napi::Number::New(env,value->frameCount)});

                            delete value;
                        });
                    }


                    if(output != nullptr){
                        auto dataGiven = owner->outputQueue.GetData(output,owner->streamInfo->outputParams.channelCount * frameCount);
                        // std::cout << "Sent " << dataGiven << " Elements" << std::endl;
                    }


                    return 0;//returnCode;
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

    void Stream::cleanup()
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
