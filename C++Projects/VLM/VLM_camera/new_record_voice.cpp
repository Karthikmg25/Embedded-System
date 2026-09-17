#define SDL_MAIN_HANDLED

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <cstring>
#include <cstdint>

/* This program records voice based on keybord events */
/* stores recorded audio as a WAV file in the directory*/

int main()
{

	//std::cout<<"\n\nProgram started";

	// initilaize audio and vedio subsystem
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) != 0)
	{
		std::cout<< "SDL init failed!!";
		return 1;
	}

	// open recording device with desired specifications
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SDL_AudioSpec spec;
	spec.freq     = 16000;
	spec.samples  = 1024;
	spec.format   = AUDIO_S16;
	spec.channels = 1;
	spec.callback = NULL;// use sdl queue for data

	SDL_AudioDeviceID device = SDL_OpenAudioDevice(SDL_GetAudioDeviceName(2,1), 1, &spec,NULL, 0);
	if(device == 0)
	{
		std::cout<<"\n\ndevice could not be opened";
		return 1;
	}

	// create a window for keyboards event capture
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	SDL_Window *window = SDL_CreateWindow("AUDIO RECORD", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600,500,0);
	if(window == NULL)
	{
		std::cout<<"\n\nFailed to open a window";
		return 1;
	}

	// start recording once pressed 'R'
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool running = true;

	//std::cout<<"\n\nPress R to start recording";


	while(running)
	{
		SDL_Event event;
		// if an event occured,
		if(SDL_PollEvent(&event))
		{
			// check the event
			if(event.type == SDL_KEYDOWN &&
			event.key.keysym.sym == 'r')
			{
				//start recording
				SDL_PauseAudioDevice(device, 0);

				//std::cout<<"\n\nPress S to stop recording";
			}

			// stop recording once pressed 'S'

			if(event.type == SDL_KEYDOWN &&
			event.key.keysym.sym == 's')
			{
				// stop recording
				SDL_PauseAudioDevice(device, 1);

				//std::cout<<"\n\nAudio recorded";
				// exit from the loop
				running = false;
			}
		}
	}

	//after recording, destroy window
	SDL_DestroyWindow(window);

	
	// store recorded audio samples from internal queue to a std vector
	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::vector<int16_t> recording;

	int16_t audio_buff[1024];

	while(SDL_GetQueuedAudioSize(device) > 0)
	{
		int bytes_read   = SDL_DequeueAudio(device, audio_buff, sizeof(audio_buff));
		int samples_read = bytes_read / sizeof(int16_t);

		// add audio buffer at end of vector
		recording.insert(recording.end(), audio_buff, audio_buff + samples_read);

	}

	
	// stop sdl subsystems
	SDL_CloseAudioDevice(device);
	SDL_Quit();


	// create a WAV header with device specifications
	////////////////////////////////////////////////////////////////////////////////////////////////////

	Uint32 total_audio_data = recording.size() * sizeof(recording[0]);// size of audio data recorded

	typedef struct
	{
		// RIFF Information

        char riff[4];            // tells the program that it's a RIFF format file
        uint32_t file_size;      // file size after this field (eg 160036 if total size is 160044)
        char wave[4];            // RIFF file contains wave audio

        // Format Information

        char fmt[4];              // beginning of format information
        uint32_t fmt_size;        // size of format information
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
    header.file_size = total_audio_data + 44 - 8; // 44 is size of header, 8 - avoid first two fields
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
    header.data_size  = total_audio_data;
	

	// write header and audio samples to generate a wav file in directory

	// open a file for writing data
	FILE *fp = fopen("samples/audio_test.wav", "wb");
	if(fp == NULL)
	{
		std::cout<<"\nFile could not be opened";
		return 1;
	}

	//write the header first
	fwrite(&header, sizeof(header), 1, fp);

	//write audio data recorded
	fwrite(recording.data(), // pointer to recorded audio samples
	sizeof(int16_t),         // size of one sample
	recording.size(),        // number of samples 
	fp                       // to the file
	);

	//close file
	fclose(fp);

	return 0;

}

