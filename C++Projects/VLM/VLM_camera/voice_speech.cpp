
/* Program to record audio, run the whisper program and read the output*/

#include<iostream>
#include<stdio.h>
#include<string>

int main()
{
    // first run new_record_wav.exe to take audio input 
    // this generates .wav audio file
   // std::cout<<"start recording...\n\n";
    system("new_record_voice.exe");
    //std::cout<<"recording stopped\n\n";

    // Command to run whisper.cpp 
    const char *command = "C:\\Users\\hp\\Desktop\\VLM\\whisper.cpp\\build\\bin\\whisper-cli.exe "
    "-m C:\\Users\\hp\\Desktop\\VLM\\whisper.cpp\\models\\ggml-base.en.bin "
    "-f C:\\Users\\hp\\Desktop\\VLM\\VLM_camera\\samples\\audio_test.wav";
    
    // Open pipe and read output from whisper-cli 
    FILE *pipe = _popen(command, "r");
    
    if (pipe == nullptr)
    {
         std::cout << "Failed to start Whisper\n";
          return 1;
    }
    
    // Store Whisper output
    std::string output; 
     
    // Buffer for reading each line
    char buffer[256]; 

     // Read output line by line 
     while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
    {
        std::string line = buffer;
         
        // remove the time-stamp from output
        // actual text starts after [0.00 - 0...11]

        // find position of ]
        size_t end = line.find(']');

        if (end != std::string::npos)// if there is ']' in the line, 
        {                            // assign the portion of line from the character after ] as line.
            line = line.substr(end + 1);
        }
        output += line;
    }

    // Close pipe
    _pclose(pipe);
    
    // Display everything received from Whisper
    //std::cout << "\n----- WHISPER OUTPUT -----\n";
    std::cout << output;


    // create a new file and store the output
    FILE *fp = fopen("output/test.txt", "w");

    // write output to new file
    fputs(output.c_str(), fp);//fputs need const char* as argument, what c_str() gives
    
    //close the file
    fclose(fp);
    
    return 0;
}