#define SDL_MAIN_HANDLED

#include <iostream>
#include <string>

/* This program runs the TTS model using the piper.exe program */

int main()
{
	std::cout <<"\n\n Prgram started...\n\n";

	// Enter input string
	//////////////////////
	std::cout <<"Enter the text you want to convert to audio file\n\n";

	std::string text;
	std::getline(std::cin,text);

	// run piper program using enetered text to run the TTS model

	std::string command = "echo \"" +text+"\" | piper.exe -m model.onnx -c config.json -f output.wav";

	system(command.c_str());

	return 0;
}


