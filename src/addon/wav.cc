#include <addon/wav.h>
#include <addon/utils.h>
#include "wav.h"

namespace nodeml_audio
{
    namespace wav
    {
        Napi::Value write(const Napi::CallbackInfo &info)
        {
            auto env = info.Env();
            try
            {
                auto fileName = info[0].ToString().Utf8Value();
                auto data = info[1].As<Napi::TypedArray>();
                auto channels = info[2].ToNumber().Int32Value();
                auto sampleRate = info[3].ToNumber().Int32Value();
                drwav wav;

                drwav_data_format format;
                format.container = drwav_container_riff; // <-- drwav_container_riff = normal WAV files, drwav_container_w64 = Sony Wave64.
                format.format = DR_WAVE_FORMAT_PCM;      // <-- Any of the DR_WAVE_FORMAT_* codes.
                format.channels = channels;
                format.sampleRate = sampleRate;
                format.bitsPerSample = 16;

                drwav_init_file_write(&wav, "data/recording.wav", &format, NULL);

                drwav_write_pcm_frames(&wav,data.ByteLength(),utils::getTypedArrayDataPtr(data));

                drwav_uninit(&wav);
            }
            catch (const std::exception &e)
            {
                throw Napi::Error::New(env, e.what());
            }

            return env.Undefined();
        }

        Napi::Object Init(Napi::Env env, Napi::Object exports)
        {
            auto myExports = Napi::Object::New(env);
            myExports.Set("write",Napi::Function::New(env,write));

            exports.Set("wav",myExports);
            
            return exports;
        }
    }
}
