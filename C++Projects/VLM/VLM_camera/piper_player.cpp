#define SDL_MAIN_HANDLED

#include <iostream>
#include <SDL2/SDL.h>

/* This program runs the piper program and plays the generated audio file*/

int main(int argc, char *argv[])
{
	
	// run piper program using enetered text to run the TTS model
	std::string text = argv[1];
	std::string command = "echo \""  + text +  "\" | "
	
	    "C:\\Users\\hp\\Desktop\\VLM\\Piper\\piper.exe "
        "-m C:\\Users\\hp\\Desktop\\VLM\\Piper\\model.onnx "
        "-c C:\\Users\\hp\\Desktop\\VLM\\Piper\\config.json "
        "-f output.wav";
	
	system(command.c_str());

	// This generates a wav file in the directory 

	//play the output.wav audio file
	//////////////////////////////////////////////

	
	// 2) initialise audio subsystem
	if(SDL_Init(SDL_INIT_AUDIO)!=0)
	{
		std::cout<<"Audio subsystem initialization failed";
		return 1;
	}

	//2) load the wav file

	SDL_AudioSpec spec;
	Uint8 *audio_data;
	Uint32 audio_len;

	if(SDL_LoadWAV("output.wav",&spec, &audio_data, &audio_len) == NULL)
	{
		std::cout<<"\n audio file could not be loaded";
		return 1;
	}

	//3) Open audio device with output.wav's specifications

	SDL_AudioDeviceID device = SDL_OpenAudioDevice(
        SDL_GetAudioDeviceName(0,0), // device name
        0,                           // open for playback
        &spec,                       // with these specifications
        NULL,                        // dont need specifications of opened device now
        0                            // no changes allowed 
    );
    if(device == 0)
    {
        std::cout<<"\n\nDevice could not be opened for playback!!";
        return 1;
    }

	//4) Queue audio data into SDL internal buffer for playback
	SDL_QueueAudio(device, audio_data, audio_len);

	//5)start playback
	SDL_PauseAudioDevice(device, 0);

	// 6) wait till audio played: queue becomes empty
	while(SDL_GetQueuedAudioSize(device)>0)
	{
		SDL_Delay(10);
	}

	//7) close the audio device
	SDL_CloseAudioDevice(device);
	SDL_Quit();


	return 0;

}
