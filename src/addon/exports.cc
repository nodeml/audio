
#include <napi.h>
#include <addon/wav/wav.hh>
#include <addon/stream/stream.hh>



Napi::Object InitModule(Napi::Env env, Napi::Object exports)
{
    nodeml_audio::stream::Init(env,exports);
    nodeml_audio::wav::Init(env,exports);
    
    return exports;
}

NODE_API_MODULE(nodeml_audio, InitModule);