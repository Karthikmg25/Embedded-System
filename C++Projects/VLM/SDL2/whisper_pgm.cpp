#include "whisper.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "wav.h"

// define path to model file
const std::string model_path = "F:/stt_model/ggml-base.en.bin";

// define function for leading wav file and converting it to float type 
// Whisper expects floats normalized to the range -1.0 to +1.0,
// where ±1.0 means full-scale (the loudest the format can represent).
bool LoadWavFile(const std::string& file_path, std::vector<float>& data);

int main()
{

    // set parameters for creating the context
    whisper_context_params cparams = whisper_context_default_params();
    cparams.use_gpu = false;

    // load the model from file: creates a context

    whisper_context* ctx = whisper_init_from_file_with_params(model_path.c_str(), cparams);

    if(ctx == NULL){
        std::cerr << "Couldn't load the model";
        return 1;
    }
    std::cout << "Model loaded successfully !"<< std::endl;

    // load wav file and store the audio samples
    std::vector<float> samples;
    LoadWavFile("audio_test.wav", samples);

    // // DEBUG: print audio size
    // std::cout << "Audio size: "<< samples.size()<< std::endl;


    // set parameters for running full whisper
    whisper_full_params wparams = whisper_full_default_params(WHISPER_SAMPLING_GREEDY);// sampling type as GREEDY
    wparams.language = "en";
    wparams.n_threads = 4;
    wparams.translate = false;
    wparams.print_progress = false;
    wparams.print_timestamps = false;
    wparams.print_realtime = false;
    wparams.print_special = false;
    
    int run = whisper_full(ctx, wparams, samples.data(), samples.size());
    if (run != 0){
        std::cerr << "whisper full failed"<< std::endl;
        return 1;
    }

    // read results
    int n_segments = whisper_full_n_segments(ctx);

    // print output:
    std::cout << " \n\nWhisper output \n\n"<< std::endl;
    for(int i=0;i<n_segments;i++){
        const char* text = whisper_full_get_segment_text(ctx, i);
        std::cout << text << "\n";
    }


    // free the loaded model
    whisper_free(ctx);
    
    return 0;

}


bool LoadWavFile(const std::string& file_path, std::vector<float>& data)
{
    std::ifstream wav;

    wav.open(file_path, std::ios::binary);// opens file for reading
    if(!wav.is_open()){
        std::cerr<<"failed to open wav file"<<std::endl;
        return false;
    }

    // read the wav header
    wav_header hdr;

    wav.read(reinterpret_cast<char*>(&hdr), sizeof(hdr));
    if(!wav){
        std::cerr<<"header read failed"<<std::endl;
        return false;
    }

    // whisper expects 16Khz, mono pcm samples
    if(hdr.sample_rate != 16000 || hdr.num_channels != 1 || hdr.bits_per_sample != 16 || hdr.audio_format != 1){
        std::cerr<<"incompatiable wav file"<< std::endl;
        return false;
    }


    // read the audio samples
    int samples_count = hdr.data_size/sizeof(int16_t);
    std::vector<int16_t> audio_data(samples_count);

    wav.read(reinterpret_cast<char*>(audio_data.data()), hdr.data_size);
    if(!wav){
        std::cerr<<"data read failed"<<std::endl;
        return false;
    }

    // convert to float samples which whisper requires
    data.resize(samples_count);// resizes number of elements to samples_count

    for(int i=0;i<samples_count;i++){
        data[i] = audio_data[i]/32768.0f;// whisper asks for values between -1.0 and +1.0
    }
    return true;
}