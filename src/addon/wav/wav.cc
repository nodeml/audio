#include <addon/utils.hh>
#include <addon/wav/wav.hh>

namespace nodeml_audio
{
    namespace wav
    {
        Napi::Object Init(Napi::Env env, Napi::Object exports)
        {
            auto myExports = Napi::Object::New(env);

            exports.Set("wav",myExports);
            
            return exports;
        }
    }
}
