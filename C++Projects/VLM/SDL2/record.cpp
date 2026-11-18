#include "record.h"

void AudioDevice::PrintAvailableAudioDevices()
{
    // get number of available devices for capture/record
    int num_dev = SDL_GetNumAudioDevices(1);

    std::cout << "\nAvailable devices for capture: \n";

    for (int i=0; i< num_dev;i++)
    {
        std::string dev_name = SDL_GetAudioDeviceName(i, 1);
        std::cout << " Device["<<i<<"]"<<dev_name<<std::endl; 
    }

}
void AudioDevice::SetAudioDeviceName(int device_index)
{
    this->device_name = SDL_GetAudioDeviceName(device_index, 1);
}
bool AudioDevice::SetDesiredAudioSpec(const SDL_AudioSpec& desired)
{
    this->desired.freq = desired.freq;
    this->desired.callback = desired.callback;
    this->desired.channels = desired.channels;
    this->desired.format   = desired.format;
    this->desired.samples  = desired.samples;
    this->desired.silence  = desired.silence;
    this->desired.size     = desired.size;
    this->desired.userdata = desired.userdata;

    return true;

}
bool AudioDevice::SetDefaultAudioSpec()
{
   
    desired.freq     = 16000;    // record 16000 audio samples per second
    desired.format   = AUDIO_S16;// each samples are of type- signed 16bit
    desired.callback = NULL;     // no callbacks needed as we use audio queue for audio samples
    desired.channels = 1;        // mono
    desired.samples  = 1024;     // more important when we use callbacks, but sets buffer configs
    desired.silence  = 0;
    desired.size     = 0;
    desired.userdata = 0;

    return true;

}
bool AudioDevice::OpenAudioDevice()
{
    // first check if SDL Audiosubsystem is initialzed
    if(!SDL_WasInit(SDL_INIT_AUDIO))
    {
        std::cerr << "Audio Subsystem is not initialized"<< std::endl;
        return false;
    }

    // Open device for recording
    int device= SDL_OpenAudioDevice(this->device_name,
    1, &desired, &obtained, 0);
    if(device == 0){
        std::cerr << "Device could not be opened" << std::endl;
        return false;
    }
    this->deviceID = device;

    return true;
}

bool AudioDevice::GetObtainedAudioSpec(SDL_AudioSpec& obt)
{
    
    obt.freq     = obtained.freq;    // record 16000 audio samples per second
    obt.format   = obtained.format;// each samples are of type- signed 16bit
    obt.callback = obtained.callback;     // no callbacks needed as we use audio queue for audio samples
    obt.channels = obtained.channels;        // mono
    obt.samples  = obtained.samples;     // more important when we use callbacks, but sets buffer configs
    obt.silence  = obtained.silence;
    obt.size     = obtained.size;
    obt.userdata = obtained.userdata;

    return true;
}
bool AudioDevice::RecordAudioForSeconds(int n)
{
    // first start recording
    SDL_PauseAudioDevice(deviceID, false);

    // store audio samples 
    int sample_size =  obtained.freq * n;
    int time = 0;
    while(time < n*100)
    {
        int16_t temp[1000];

        int bytes_out = SDL_DequeueAudio(deviceID, temp, sizeof(temp));
        int samples_out = bytes_out / sizeof(int16_t);
        // store audio samples into our vector
        this->audio_data.insert(audio_data.end(), temp, temp+samples_out);

        SDL_Delay(10);// add a 10ms delay
        time++;
        
    }

    // stop recording
    SDL_PauseAudioDevice(deviceID, true);

    return true;

}
bool AudioDevice::GetAudioData(std::vector<int16_t>& Audio_data)
{
    Audio_data = this->audio_data;

    return true;

}
bool AudioDevice::CloseAudioDevice()
{
    // close the device
    SDL_CloseAudioDevice(deviceID);

    return true;
}

bool AudioDevice::CreateWavFile(const char* wav_name)
{
    // first fill the wav header with details
        
    wav_header header;

    // RIFF info
    memcpy(header.riff, "RIFF", 4);// it's a RIFF file
    memcpy(header.wave, "WAVE", 4);// it's a wav file
    int audio_size = audio_data.size()*2;// audio size in bytes: 1 sample = 2 bytes
    int header_size = sizeof(wav_header);// 44 bytes
    header.file_size = audio_size + header_size - 8;

    // Format info
    memcpy(header.fmt, "fmt ", 4);
    header.fmt_size = 16;
    header.audio_format    = 1;// PCM
    header.num_channels    = 1;
    header.sample_rate     = obtained.freq;
    header.byte_rate       = obtained.freq*2;
    header.block_align     = 2;
    header.bits_per_sample = 16;
    
    // audio info
    memcpy(header.data, "data", 4);
    header.data_size = audio_size;

    // write the header and the audio data into the wav file
     // open file for write/output
    std::ofstream file;

    file.open(wav_name, std::ios::binary);

    if(!file.is_open()){
        std::cerr << "File could not be opened";
        return false;
    }

    // write wav header: write() takes const char* as 1st argument
    file.write(reinterpret_cast<const char*>(&header), sizeof(header));

    // write audio data
    file.write(reinterpret_cast<const char*>(audio_data.data()), audio_data.size()*sizeof(int16_t));

    if(!file){
        std::cerr<<"Filed to write file"<<std::endl;
        return false;
    }

    // close file
    file.close();

    return true;
}