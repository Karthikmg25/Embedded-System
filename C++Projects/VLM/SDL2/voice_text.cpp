#define SDL_MAIN_HANDLED

#include "whisper.h"
#include "record.h"

AudioDevice mic;

const char* model_path = "F:/stt_model/ggml-base.en.bin";

int main()
{
    // initialize SDL audio subsystem
    if(SDL_Init(SDL_INIT_AUDIO) != 0){
        std::cerr << "SDL init failed" << std::endl;
        return 1;
    }

    // first record sound for 5 seconds

    mic.SetAudioDeviceName(1);// set name based on available devices !
    mic.SetDefaultAudioSpec();
    mic.OpenAudioDevice();
    std::cout << "Recording started...."<< std::endl;
    mic.RecordAudioForSeconds(10);
    std::cout << "recording stopped"<< std::endl;
    mic.CloseAudioDevice();
    mic.CreateWavFile("audio_record.wav");
                                                                       // RUNNING WHISPER //   

    // set the context params of whisper context
    whisper_context_params cparams = whisper_context_default_params();
    cparams.use_gpu = false;

    // load the context and model from the file
    whisper_context* ctx = whisper_init_from_file_with_params(model_path, cparams);
    if(ctx == NULL){
        std::cerr << "Couldn't load the model"<< std::endl;
        return 1;
    }
    std::cout << "Model loaded successfully !"<< std::endl;

    // set the params for running the whisper model: with GREEDY sampling strategy
    whisper_full_params wparams = whisper_full_default_params(WHISPER_SAMPLING_GREEDY);
    wparams.language = "en";
    wparams.n_threads = 4;
    wparams.translate = false;
    wparams.print_progress = false;
    wparams.print_timestamps = false;
    wparams.print_realtime = false;
    wparams.print_special = false;

    // before running the model, get the recorded audio data and convert it to compatiable type for whisper
    // whisper needs float type samples witin range: -1.0 and +1.0
    std::vector<int16_t> audio_data;
    mic.GetAudioData(audio_data);
    std::vector<float> samples(audio_data.size());// for storing whisper compatiable samples
    for(int i=0; i< audio_data.size(); i++)
    {
        samples[i] = audio_data[i]/ 32768.0f;
    }


    // run model
    int run = whisper_full(ctx, wparams, samples.data(), samples.size());
    if (run != 0){
        std::cerr << "whisper run failed"<< std::endl;
        return 1;
    }

    // print output

    std::string output="";
    int n_segments = whisper_full_n_segments(ctx);
    for(int i=0; i< n_segments;i++)
    {
        const char* text = whisper_full_get_segment_text(ctx, i);
        output += text;

       

    }
    std::cout << "\nConverted text:\n"<< std::endl;
    std::cout << output << std::endl;

    SDL_Quit();

    return 0;
}