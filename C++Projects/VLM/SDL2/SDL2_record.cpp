
#define SDL_MAIN_HANDLED


#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>

#include "SDL2/SDL.h"
#include "wav.h"

int main()
{
    // SDL init audio subsystem
    if(SDL_Init(SDL_INIT_AUDIO) != 0){
        std::cerr << "Failed to init ";
        return 1;
    }

    std::cout << "Audio subsystem initialised successfully!"<< std::endl;


    // print availabel audio devices: 1 - recording devices

    int num_devices = SDL_GetNumAudioDevices(1);

    std::cout << "Available audio devices for recording: "<< std::endl;
    for (int i= 0;i<num_devices;i++){
        const char* dev_name = SDL_GetAudioDeviceName(i, 1);
        std::cout << "Device "<< i << " : "<< dev_name<< std::endl;
    }

    // Open audio device for recording
    //  - specify the configurations of recordings
    //  - also read the configs with which device opened

    // Requires audio specs
    SDL_AudioSpec desired={};
    // Obtained audio specs
    SDL_AudioSpec obtained;

    desired.freq = 16000;// record 16000 audio samples per second
    desired.format= AUDIO_S16;// each samples are of type- signed 16bit
    desired.callback = NULL;// no callbacks needed as we use audio queue for audio samples
    desired.channels = 1;// mono
    desired.samples = 1024;// more important when we use callbacks, but sets buffer configs

    SDL_AudioDeviceID device = SDL_OpenAudioDevice(SDL_GetAudioDeviceName(1,1),// dev name
                                                    
                        1,// for capture
                        &desired,
                        &obtained,
                        0// allow no changes, fails to open if specs cannnot be met
                        );

    if(device == 0){
        std::cerr << "Failed to open the device";
        return 1;
    }
    std::cout << "Device opened for recording"<< std::endl;


    // Start recording for 5 seconds

    // we record 16000 samples per second, so on 5 seconds,
    // total samples will be 16000*5 which will be 80000

    // so, declare a vector to store 80000 samples 
    // and stop recording once size of vector becomes 80000

    std::vector<int16_t> recording;

    recording.reserve(80000);// this sets capacity without changing size 
    // reserving space early avoids runtime allocations //

    // Start the recording
    std:: cout << "Recording started..."<< std::endl;

    SDL_PauseAudioDevice(device, 0);// 0- starts recording


    // recording loop:

    while(recording.size() < 80000){
        
        // store recorded samples from SDL queue into a temperory buffer
        int16_t temp[1024];

        int bytes_out = SDL_DequeueAudio(device, temp, sizeof(temp));
        int samples_out = bytes_out / 2;// since sample is 2 bytes

        // store samples back into vector
        recording.insert(recording.end(), temp, temp+samples_out);

        // add a small delay
        SDL_Delay(10);
    }

    std::cout << "Recording stopped !"<< std::endl;

    // Stop recording
    SDL_PauseAudioDevice(device, 1);// 1- pauses device

    std::cout << "Some recorded samples:"<< std::endl;

    for(int i=50; i< 100 ; i++){
        std::cout << "\nsample "<< i <<" : "<< recording[i];
    }

    std::cout << "\n recording size: "<< recording.size()<< std::endl;


    // write the recorded samples with a wav header to create a wav audio file

    // first fill the wav header with details
        
    wav_header header;

    // RIFF info
    memcpy(header.riff, "RIFF", 4);// it's a RIFF file
    memcpy(header.wave, "WAVE", 4);// it's a wav file
    int audio_size = recording.size()*2;// audio size in bytes: 1 sample = 2 bytes
    int header_size = sizeof(wav_header);// 44 bytes
    header.file_size = audio_size + header_size - 8;

    // Format info
    memcpy(header.fmt, "fmt ", 4);
    header.fmt_size = 16;
    header.audio_format    = 1;// PCM
    header.num_channels    = 1;
    header.sample_rate     = 16000;
    header.byte_rate       = 32000;
    header.block_align     = 2;
    header.bits_per_sample = 16;
    
    // audio info
    memcpy(header.data, "data", 4);
    header.data_size = audio_size;


    // write header and audio data into a file

    // open file for write/output
    std::ofstream file;

    file.open("audio_test.wav", std::ios::binary);

    if(!file.is_open()){
        std::cerr << "File could not be opened";
        return 1;
    }

    // write wav header: write() takes const char* as 1st argument
    file.write(reinterpret_cast<const char*>(&header), sizeof(header));

    // write audio data
    file.write(reinterpret_cast<const char*>(recording.data()), recording.size()*sizeof(int16_t));

    // close file
    file.close();

    //close device
    SDL_CloseAudioDevice(device);

    // Cleanup all initialized subsystems
    SDL_Quit();

    return 0;
}

