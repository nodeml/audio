#include <napi.h>

Napi::Object InitModule(Napi::Env env, Napi::Object exports)
{
    return exports;
}


NODE_API_MODULE(nodeml_gpt4all, InitModule);