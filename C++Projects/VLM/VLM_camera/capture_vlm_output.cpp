#include <cstdio>
#include <iostream>
#include <string>

int main()
{
    const char *command =
        "cmd /c \""
        "C:\\Users\\hp\\Desktop\\VLM\\llama.cpp-current\\build\\bin\\llama-mtmd-cli.exe "
        "-m \"F:\\AI Models\\VLM\\Qwen2.5-VL-3B-Instruct-Q4_K_M.gguf\" "
        "--mmproj \"F:\\AI Models\\VLM\\mmproj-Qwen2.5-VL-3B-Instruct-f16.gguf\" "
        "--image \"C:\\Users\\hp\\Desktop\\VLM\\VLM_camera\\capture.jpg\" "
        "-p \"Describe what you see in this image.\""
        "\"";

        // run the cli program and provide a pipe to read it's output.
    FILE *pipe = _popen(command, "r");

    if (pipe == nullptr)
    {
        std::cout << "Failed to start VLM\n";
        return 1;
    }

    //declare a buffer to store one line of output
    char buffer[256];
    //String to store the filtered VLM response
    std::string output;

    //read each line and store as output.
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
    {
        //store single line
        std::string line = buffer;

        //ignore log lines from llama.cpp
        //store it as output only if is not an I log line or W log line.
        if(line.find(" I ") == std::string::npos &&
           line.find(" W ") == std::string::npos)
           {
            output += buffer;
           }
        
    }
    //close the pipe after reading
    _pclose(pipe);

    //print the output.
    std::cout << "\n----- VLM DESCRIPTION -----\n";
    std::cout << output;

    return 0;
}