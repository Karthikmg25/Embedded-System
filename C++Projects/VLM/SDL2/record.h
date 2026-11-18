#ifndef RECORD_H
#define RECORD_H

#include "SDL2/SDL.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "wav.h"

class AudioDevice
{
    private:

        // declare required data
        const char* device_name;
        SDL_AudioDeviceID deviceID;
        SDL_AudioSpec desired;
        SDL_AudioSpec obtained;
        std::vector<int16_t> audio_data;

    public:

        // declare functions needed
       
        void PrintAvailableAudioDevices();
        void SetAudioDeviceName(int device_index);
        bool SetDesiredAudioSpec(const SDL_AudioSpec& desired);
        bool SetDefaultAudioSpec();
        bool OpenAudioDevice();
        bool GetObtainedAudioSpec(SDL_AudioSpec& obt);
        bool GetAudioData(std::vector<int16_t>& Audio_data);
        bool RecordAudioForSeconds(int n);
        bool CreateWavFile(const char* wav_name);
        bool CloseAudioDevice();
        

};
#endif