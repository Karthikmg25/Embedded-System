#ifndef WAV_H
#define WAV_H


// define the wav header here:

#pragma pack(push, 1) // push new alignment

typedef struct{
    
		// RIFF information
		char riff[4];            // tells the program that it's a RIFF format file
        uint32_t file_size;      // file size after this field (eg 160036 if total size is 160044)
        char wave[4];            // RIFF file contains wave audio

        // Format Information //
        char fmt[4];              // beginning of format information
        uint32_t fmt_size;        // size of format information after this field
        uint16_t audio_format;    // type of audio encoding (eg: PCM)
        uint16_t num_channels;    // number of audio channels
        uint32_t sample_rate;     // number of samples per second
        uint32_t byte_rate;       // number of bytes per second
        uint16_t block_align;     // number of bytes in one sample frame (eg 2 for single channel 16 bit)
        uint16_t bits_per_sample; // number of bits in one sample

        // audio data //
        char data[4];             // beginning of audio data
        uint32_t data_size;       // size of audio data (size of vector in this case)

}wav_header;

#pragma pack(pop) // restore default alignment
#endif