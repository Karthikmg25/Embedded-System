#define SDL_MAIN_HANDLED
#include "record.h"


AudioDevice mic;


int main()
{
   

    // Initialize SDL Audio subsystem
    if(SDL_Init(SDL_INIT_AUDIO) != 0){
        std::cerr <<"Init failed"<<std::endl;
        return 1;
    }

    mic.PrintAvailableAudioDevices();// microphone is device[1] 

    mic.SetAudioDeviceName(1);// set device name as microphone

    mic.SetDefaultAudioSpec();

    mic.OpenAudioDevice();

    std::cout << "Recording started..."<< std::endl;
    mic.RecordAudioForSeconds(5);// record audio for 5 seconds
    std::cout << "Recording stopped"<< std::endl;

    mic.CreateWavFile("record_test.wav");
    std::cout << "wav file created!"<< std::endl;

    mic.CloseAudioDevice();

    SDL_Quit();
    return 0;
}