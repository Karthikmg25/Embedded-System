
/* This program asks user the recording time and records audio */

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

int main()
{
	std::cout<< "\n\n      Program started....." << "\n\n" << std::endl;
	int seconds;
	std::cout<< "How many seconds you want to record audio ?\n";
	std::cin>> seconds;
	
	// Initialize audio subsystem
	////////////////////////////////////////////////////////////////////////////////////////

	// required audio specifications
	SDL_AudioSpec desired;
	desired.freq       = 16000;     // store 16000 samples per second
	desired.samples    = 1024;      // set buffer size
	desired.channels   = 1;         // mono
	desired.callback   = NULL;      // use internal queue for audio data, no callbacks
	desired.format     = AUDIO_S16; // each sample is signed 16 bit

	if(SDL_Init(SDL_INIT_AUDIO)!=0)
	{
		std::cout<<"Initialization failed";
		return 1;
	}
		std::cout<<"\n Audio subsystem initialized successfully\n";

	// List available audio devices in the system
	////////////////////////////////////////////////////////////////////////////////////////

	//get number of devices available for recording(1)
	int num_devices = SDL_GetNumAudioDevices(1);

	//print available devices
	std::cout<<"\n Available audio devices \n";
	for(int i=0;i<num_devices;i++)
	{
		const char* device_name = SDL_GetAudioDeviceName(i,1);

		std::cout<<"\ndevice "<<i<<" : "<< device_name;
	}

	// Open required audio device : microphone
	////////////////////////////////////////////////////////////////////////////////////////

	SDL_AudioSpec obtained; // for getting specs of opened device

	SDL_AudioDeviceID deviceID = SDL_OpenAudioDevice(
		                SDL_GetAudioDeviceName(2,1),// open microphone
                        1,                          // for capture
						&desired,                   // with desired specifications
					    &obtained,                  // return obtained specid==fications
					    0                           // no changes
						);

	if(deviceID == 0)
	{
		std::cout<<"\nDevice could not be opened";
		return 1;
	}					
	std::cout<<"\n\nMicrophone is now opened for recording with following specifications \n";

	std::cout<< "\nSampling frequency : "<< obtained.freq;
	std::cout<< "\nSample format      : "<< obtained.format;
	std::cout<< "\nNumber of channels : "<< (int)obtained.channels;
					


	// Start recording and store samples in vector
	////////////////////////////////////////////////////////////////////////////////////////

	//declare a vector and allocate space for storing samples for seconds
	std::vector<int16_t> recording;
	int total_samples = obtained.freq * seconds;
	recording.reserve(total_samples);

	// Start recording
	SDL_PauseAudioDevice(deviceID, 0);

	std::cout<<"\n\nRecording started....\n\n";

	// store audio samples into the vector
	while(recording.size()< total_samples)
	{
		//declare a temp buffer to store 1024 samples
		int16_t temp[1024];

		// wait till 1024 samples are available in queue
		while(SDL_GetQueuedAudioSize(deviceID) < sizeof(temp))
		{
			SDL_Delay(10);//small delay
		}

		// store 1024 samples into temp buffer
		SDL_DequeueAudio(deviceID, temp, sizeof(temp));

		// store audio samples into vector
		for(int i=0;i<1024;i++)
		{
			// stop recording once we fill vector
			if(recording.size()>= total_samples)
			{break;}

			recording.push_back(temp[i]);
		}
	}
	// close audio device after recording
	SDL_CloseAudioDevice(deviceID);
	SDL_Quit();

	// create a wav header 
	////////////////////////////////////////////////////////////////////////////////////////

	//declare a structure for header
	typedef struct 
	{
		// RIFF information
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

	// RIFF info    (RIFF - a type of file)
	strcpy(header.riff, "RIFF"); 
	header.file_size = (recording.size()* sizeof(int16_t))+ 44 - 8; // 44 - header size, -8 avoid first two fields
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
    header.data_size  = (recording.size()* sizeof(int16_t));// total audio data

	// Store audio samples with the header into a wav file
	////////////////////////////////////////////////////////////////////////////////////////

	FILE *fp = fopen("recordings/audio.wav", "wb");
	if(fp == nullptr)
    {
        std::cout<<"FILE COULD NOT BE OPENED!!!";
        return 1;
    }
    else
    {
       std::cout<<"audio file is created in the directory\n\n";

    }

	//write the header into file
	fwrite(&header, sizeof(header), 1, fp);

	//write full audio recording
	for(int i=0;i<recording.size();i++)
	{
		fwrite(&recording[i], sizeof(int16_t), 1, fp);
	}

	//close wav file
	fclose(fp);

	return 0;
}
