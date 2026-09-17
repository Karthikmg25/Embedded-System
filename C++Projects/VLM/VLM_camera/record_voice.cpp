#define SDL_MAIN_HANDLED

/* 
This program records audio samples for 5 seconds and stores wav header and samples to create a wav file
*/

#include <iostream>    // for std in and out
#include <SDL2/SDL.h>  // for audio handling
#include <vector>      // for storing samples
#include <stdio.h>     // for file handling

#define BUFFER_SIZE 1024 // for storing single audio buffer
int main()
{

    //std::cout<<"program started\n";

    // Initialize SDL2 audio subsystem
    /*****************************************************************************************/
    
    // Set required audio format
    SDL_AudioSpec spec = {};
    spec.freq    = 16000;            // number of samples per second
    spec.format  = AUDIO_S16;        // Signed 16bit
    spec.samples = BUFFER_SIZE;      // collect 1024 samples in a buffer
    spec.channels = 1;               // mono
    spec.callback = NULL;            // use queue mode: stores audio data in an internal queue

    
   if(SDL_Init(SDL_INIT_AUDIO)!=0)
   {
    std::cout<<"SDL initialization failed: "
                  << SDL_GetError() << "\n";

        return 1;
   } 

    //std::cout << "SDL audio initialized successfully\n";


    // Open microphone
    /*****************************************************************************************/


    // print the available devices
    int count = SDL_GetNumAudioDevices(1);

    for(int i = 0; i < count; i++)
    {
         //std::cout << i << ": "
        //<< SDL_GetAudioDeviceName(i, 1)
        //<< '\n';
    }

    //open microphone: device index 2

    SDL_AudioSpec obtained;
    SDL_AudioDeviceID device = SDL_OpenAudioDevice(
        SDL_GetAudioDeviceName(2, 1),       // open device2 - microphone
        1,               // 1 = for recording
        &spec,           // requested audio format
        &obtained,       // resulting audio format of device
        0                // don't allow changes for now
    );

    if(device == 0)
    {
        std::cout << "Failed to open microphone: "
                  << SDL_GetError() << "\n";

        SDL_Quit();
        return 1;
    }

    //std::cout << "Microphone opened successfully\n";

    //print the audio specifications
    //std::cout << "Actual frequency: " << obtained.freq << '\n';
    //std::cout << "Actual channels: " << (int)obtained.channels << '\n';
    //std::cout << "Actual format: " << obtained.format << '\n';

    // Capture audio using the device
    /*************************************************************************************/
    
    // find total number of samples for 5 seconds
    int total_samples = spec.freq * 5;// 

    //declare a vector to store samples for 5 seconds
    std::vector<int16_t> recording;
    //allocate space for storing samples: avoids dynamic allocation
    recording.reserve(total_samples);

    // Start capturing
    SDL_PauseAudioDevice(device, 0);

    // loop till vector stores samples for 5 seconds
    while(recording.size() < total_samples)
    {
        // declare a buffer to store audio samples from SDL queue
        int16_t samples[BUFFER_SIZE];

        // wait until 1024 samples are available
        while(SDL_GetQueuedAudioSize(device) < sizeof(samples))
        {
         SDL_Delay(10);// wait 10ms to not waste cpu cycles.
        }

        // Take audio samples from the internal queue
        SDL_DequeueAudio(device, samples, sizeof(samples));

        //push buffer into vector
        for(int i=0;i<BUFFER_SIZE;i++)
        {

            // stop capture if vector is full
            if(recording.size()>= total_samples)
            {
                break;
            }
            recording.push_back(samples[i]);
        }

    }

    SDL_CloseAudioDevice(device);
    SDL_Quit();

    // print minimum and maximum samples in the recording
    int16_t min_sample = recording[0];
    int16_t max_sample = recording[0];

    for(size_t i = 1; i < recording.size(); i++)
    {
        if(recording[i] < min_sample)
        min_sample = recording[i];

        if(recording[i] > max_sample)
        max_sample = recording[i];
    }

   // std::cout << "Minimum sample: " << min_sample << '\n';
    //std::cout << "Maximum sample: " << max_sample << '\n';


    // Store samples in the vector into a wav file.
    /*********************************************************************************************/

    // create a wav header
    typedef struct
    {
        // RIFF Information

        char riff[4];            // tells the program that it's a RIFF format file
        uint32_t file_size;      // file size after this field (eg 160036 if total size is 160044)
        char wave[4];            // RIFF file contains wave audio

        // Format Information

        char fmt[4];              // beginning of format information
        uint32_t fmt_size;        // size of format information after this field
        uint16_t audio_format;    // type of audio encoding (eg: PCM)
        uint16_t num_channels;    // number of audio channels
        uint32_t sample_rate;     // number of samples per second
        uint32_t byte_rate;       // number of bytes per second
        uint16_t block_align;     // number of bytes in one sample frame (eg 2 for single channel 16 bit)
        uint16_t bits_per_sample; // number of bits in one sample

        // audio data 

        char data[4];             // beginning of audio data
        uint32_t data_size;       // size of audio data (size of vector in this case)

    }WAVHeader_t;
    
    WAVHeader_t header;

    // RIFF info
    strcpy(header.riff , "RIFF");
    header.file_size = 160036; 
    strcpy(header.wave, "WAVE");

    // Format info
    strcpy(header.fmt, "fmt ");
    header.fmt_size        = 16;
    header.audio_format    = 1;// PCM
    header.num_channels    = 1;
    header.sample_rate     = 16000;
    header.byte_rate       = 32000;
    header.block_align     = 2;
    header.bits_per_sample = 16;

    //Data info
    strcpy(header.data, "data");
    header.data_size  = 160000;// total audio data

    // create a recording file to store header and audio data in vector

    FILE *fp = fopen("C:\\Users\\hp\\Desktop\\VLM\\VLM_camera\\samples\\recording.wav", "wb");
    if(fp == nullptr)
    {
        std::cout<<"FILE COULD NOT BE OPENED!!!";
        return -1;
    }
    else
    {
       // std::cout<<"file created in the directory\n\n";

    }

    // store header into file : size of wav header must be 44 bytes
    fwrite(&header, sizeof(header), 1, fp);

    // store recorded audio samples into file
    for(int i=0;i<sizeof(recording);i++)
    {
        fwrite(&recording[i], sizeof(int16_t), 1, fp);
    }

    //close file
    fclose(fp);


}