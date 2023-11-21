#include <napi.h>
#include <portaudio.h>
#include <nodeml_portaudio/hosts.h>
#include <nodeml_portaudio/Stream.h>
#include <nodeml_portaudio/formats.h>

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

    nodeml_portaudio::hosts::Init(env,exports);

    nodeml_portaudio::Stream::Init(env,exports);

    nodeml_portaudio::formats::Init(env,exports);

    return exports;
}

NODE_API_MODULE(nodeml_portaudio, InitModule);