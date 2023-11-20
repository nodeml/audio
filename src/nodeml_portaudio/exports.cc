#include <napi.h>
#include <portaudio.h>

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

    return exports;
}

NODE_API_MODULE(nodeml_portaudio, InitModule);