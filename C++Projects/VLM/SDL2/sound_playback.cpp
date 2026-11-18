
#define SDL_MAIN_HANDLED

#include <iostream>
#include <SDL2/SDL.h>


/* This program reads a wav file from disk and plays using an udio device*/
int main()
{
    // initialise audio subsystem
    /*********************************************************************************** */

    std::cout<<"\n\nProgram started..."<<"\n\nInitialising audio subsystem...";
    if(SDL_Init(SDL_INIT_AUDIO)!=0){
        std::cout<<"\n\nInitialisation failed";
        return 1;
    }

    // print available audio-playback devices

    int count = SDL_GetNumAudioDevices(0);// number of playback devices
    std::cout<<"\n\nAvailable playback devices are: \n";
    for(int i=0;i<count;i++)
    {
        const char *device_name = SDL_GetAudioDeviceName(i,0);

        std::cout<<"\ndevice "<<i<<" : "<< device_name;

    }

    // load wav file from disk for reading samples
    /************************************************************************************/

    //SDL_LoadWAV() gives audio specifications, pointer to audio data and length of audio data
    //declare a pointer that points to audio_data after calling SDL_LoadWAV()
    //SDL stores audio data as raw bytes, so pointer type is Uint8
    //declare a structure for storing audio spec of wav file
   
    Uint8   *audio_data;
    Uint32   audio_len;
    SDL_AudioSpec spec;

    // Load wav file
    if(SDL_LoadWAV("C:/Users/hp/Desktop/VLM/whisper.cpp/samples/jfk.wav",&spec, &audio_data, &audio_len) == NULL)
    {
        std::cout<<"\n\nWAV file could not be loaded";
        return 1;
    }


    // print audio specifications of the loaded wav file
    std::cout<<"\n\nWAV file is loaded. Specifications are: \n";
    std::cout<<"\naudio frequency : "<<spec.freq;
    std::cout<<"\naudio format    : "<<spec.format;
    std::cout<<"\nchannels        : "<<spec.channels;

    // open audio device for playback with wav specifications
    /*********************************************************************************** */

    SDL_AudioDeviceID device = SDL_OpenAudioDevice(
        SDL_GetAudioDeviceName(0,0),
        0,
        &spec,
        NULL,
        0
    );
    if(device == 0)
    {
        std::cout<<"\ndevice could not be opened for playback";
        return 1;
    }
    // Put audio data of wav to SDL internal audio queue and start playback 
    /************************************************************************************ */

        if(SDL_QueueAudio(device, audio_data, audio_len)!=0)
    {
        std::cout<<"\naudio data could not be queued";
        return 1;
    }

    //start playback
    SDL_PauseAudioDevice(device, 0);

    // need to wait till sdl queue becomes empty, which indicates that audio has been fully played
    //wait till all audio has been played
    while(SDL_GetQueuedAudioSize(device) > 0){
        SDL_Delay(10);
    }

    
    
    // free the memory allocated for loading WAV file
    SDL_FreeWAV(audio_data);

    // close audio device
    SDL_CloseAudioDevice(device);
    SDL_Quit();

    return 0;
}

