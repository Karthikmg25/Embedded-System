#define SDL_MAIN_HANDLED

/* This program is for testing APIs of SDL2 library*/

#include <iostream>
#include <SDL2/SDL.h>
#include <vector>

int main()
{
    // initialize SDL audio subsystem

    if(SDL_Init(SDL_INIT_AUDIO) != 0)
    {
        std::cout<<"SDL Init failed";
        return 1;
    }
    // Print available playback devices for playback

    int num_devices = SDL_GetNumAudioDevices(0);
    std::cout<<"\n\nAvailable playback devices are : \n";
    for( int i=0;i< num_devices; i++)
    {
        const char* dev_name = SDL_GetAudioDeviceName(i, 0);

        std::cout<<"\n device "<<i<<" : "<<dev_name;
    }

    // Print available cature devices for recording 

    num_devices = SDL_GetNumAudioDevices(1);
    std::cout<<"\n\nAvailable capture devices are : \n";
    for( int i=0;i< num_devices; i++)
    {
        const char* dev_name = SDL_GetAudioDeviceName(i, 1);

        std::cout<<"\n device "<<i<<" : "<<dev_name;
    }

    // open microphone for recording with following specs
    SDL_AudioSpec spec= {};
    spec.channels = 1;         // need mono
    spec.format   = AUDIO_S16; // sample foramt is int16_t
    spec.callback = NULL;      // use queue based recording
    spec.samples  = 1024;      // buffer size
    spec.freq     = 16000;     // collect 16000 samples per second

    SDL_AudioDeviceID device = SDL_OpenAudioDevice(
        SDL_GetAudioDeviceName(2,1),// opens microphone
        1,                          // for capture
        &spec,                      // with desired audio specifications
        NULL,                       // dont need obtained specs now
        0                           // allow no changes
    );
    //check if opened correctly
    if(device == 0)
    {
        std::cout<<"\n\ndevice could not be opened for recording";
        return 1;
    }

    //Initialize SDL_VIDEO subsystem and create a window for Keyboard event tracking

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
    "My SDL Window",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    640,
    480,
    0
    );
    
    // Loop to test events APIs
    bool running = true;


    while(running)
    {
        SDL_Event event;

        if(SDL_PollEvent(&event)) // if there is an event polled
        {
            //check the keyboard events

            // if r pressed, start recording
            if(event.type == SDL_KEYDOWN && event.key.keysym.sym == 'r')
            {
                std::cout<<"\n\nStart recording......";
                SDL_PauseAudioDevice(device, 0);

                // The SDL queue from now on will start collecting audio samples.
                // ! keep recording small that the queue won't run out of memory
            }

            // if s pressed, stop recording
            if(event.type == SDL_KEYDOWN && event.key.keysym.sym == 's')
            {
                std::cout<<"\n\nrecording stopped";
                SDL_PauseAudioDevice(device, 1);
                running = false;
                
                //  recording is complete: Puase the device and stop the loop
            }
        }
       
       
    }
    // can destroy window once the recording is over
    SDL_DestroyWindow(window);

    // Dequeue the sdl queue to collect recorded samples

    //declare a vector to collect samples
    std::vector<int16_t> recording;
    //declare a buffer to collect audio chunks
    int16_t buffer[1024];

    //dequeue till sdl queue is empty
    while(SDL_GetQueuedAudioSize(device) > 0)
    {
        Uint32 bytes_read   = SDL_DequeueAudio(device, buffer, sizeof(buffer));

        Uint32 samples_read = bytes_read / sizeof(int16_t);

        //insert dequeued samples into vector
        recording.insert(recording.end(), buffer, buffer + samples_read);

    }
    //close the capture device
    SDL_CloseAudioDevice(device);

    // now, recorded samples are inside the vector. Next, play that audio by opening a playback device

    // open a device for playback
    device = SDL_OpenAudioDevice(
        NULL, // Open the default device
        0,    // for playback
        &spec,// with specifications of recorded audio
        NULL, // dont need obtained specifications 
        0     // no changes allowed
    );
    //check if device opened for playback
    if(device == 0)
    {
        std::cout<<"\n\ndevice not opened for playback";
        return 1;
    }

    //queue the sdl queue with collected samples in vector
    if(SDL_QueueAudio(device, recording.data(), recording.size()*sizeof(int16_t)) != 0)
    {
        std::cout<<"\n\naudio data could not be queued!!";
        return 1;
    }

    // start playback to hear the recorded sound
    SDL_PauseAudioDevice(device, 0);

    // wait till playback is over.
    while(SDL_GetQueuedAudioSize(device) > 0)
    {
        SDL_Delay(10);// delay 10ms 
    }

    // close playback device
    SDL_CloseAudioDevice(device);
 
    //clean up
    SDL_Quit();

    return 0;
}

/*

        Overall flow of the program

    Initialize SDL
      ↓
    Open capture device
      ↓
    Press R
      ↓
    SDL starts capturing
      ↓
    Audio accumulates in SDL queue
      ↓
    Press S
      ↓
    Pause capture
      ↓
    Dequeue queue in 1024-sample chunks
      ↓
    Store samples in vector
      ↓
    Close capture device
      ↓
    Open playback device
      ↓
    Queue vector's samples
      ↓
    Start playback
      ↓
    Wait until queue empty
      ↓
    Close playback
      ↓
    SDL_Quit()


*/