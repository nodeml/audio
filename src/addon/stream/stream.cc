#include <addon/stream/stream.h>
#include <addon/stream/PortAudioStream.h>
#include <portaudio.h>
#include <addon/stream/hosts.h>
#include <addon/stream/formats.h>

namespace nodeml_audio
{
    namespace stream
    {

        void cleanup()
        {
            Pa_Terminate();
        }

        Napi::Object Init(Napi::Env env, Napi::Object exports)
        {
            auto myExports = Napi::Object::New(env);

            if (Pa_Initialize() != paNoError)
            {
                Napi::Error::New(env, "Failed To Initialize PortAudio");
            }

            env.AddCleanupHook(cleanup);

            nodeml_audio::stream::PortAudioStream::Init(env, myExports);
            nodeml_audio::stream::hosts::Init(env, myExports);
            nodeml_audio::stream::formats::Init(env, myExports);

            exports.Set("stream", myExports);

            return exports;
        }
    }
}