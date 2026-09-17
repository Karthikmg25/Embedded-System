#include <cstdlib>
int main()
{
    system(
        "cmd /c \""
        "C:\\Users\\hp\\Desktop\\VLM\\llama.cpp-current\\build\\bin\\llama-mtmd-cli.exe "
        "-m \"F:\\AI Models\\VLM\\Qwen2.5-VL-3B-Instruct-Q4_K_M.gguf\" "
        "--mmproj \"F:\\AI Models\\VLM\\mmproj-Qwen2.5-VL-3B-Instruct-f16.gguf\" "
        "--image \"C:\\Users\\hp\\Desktop\\VLM\\VLM_camera\\capture.jpg\" "
        "-p \"Describe what you see in this image.\""
        "\""
    );

    return 0;
}