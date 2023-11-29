#include "formats.h"

namespace nodeml_audio
{
    namespace stream
    {
        namespace formats
        {
            Napi::Object Init(Napi::Env env, Napi::Object exports)
            {

                auto myExports = Napi::Object::New(env);
                myExports.Set("float32", Napi::Number::New(env, paFloat32));
                myExports.Set("int32", Napi::Number::New(env, paInt32));
                myExports.Set("int24", Napi::Number::New(env, paInt24));
                myExports.Set("int16", Napi::Number::New(env, paInt16));
                myExports.Set("int8", Napi::Number::New(env, paInt8));
                myExports.Set("uint8", Napi::Number::New(env, paUInt8));

                exports.Set("formats", myExports);
                return exports;
            }

        }

    }

}