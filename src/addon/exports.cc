#define DR_WAV_IMPLEMENTATION
#include <napi.h>
#include <portaudio.h>
#include <addon/hosts.h>
#include <addon/Stream.h>
#include <addon/formats.h>
#include <addon/wav.h>

void cleanup()
{
    Pa_Terminate();
}

Napi::Object InitModule(Napi::Env env, Napi::Object exports)
{
    if(Pa_Initialize() != paNoError){
        Napi::Error::New(env,"Failed To Initialize PortAudio");
    }

    env.AddCleanupHook(cleanup);

    nodeml_audio::hosts::Init(env,exports);

    nodeml_audio::Stream::Init(env,exports);

    nodeml_audio::formats::Init(env,exports);

    nodeml_audio::wav::Init(env,exports);

    return exports;
}

NODE_API_MODULE(nodeml_audio, InitModule);