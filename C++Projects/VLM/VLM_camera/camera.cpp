#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{
     std::cout << "Program started\n";

    cv::VideoCapture camera(0);

    if (!camera.isOpened())
    {
        std::cout << "Failed to open camera\n";
        return 1;
    }

    std::cout << "Camera opened successfully\n";

    cv::Mat frame;// declare a frame to store image

    while (true)
    {
        //capture frame
        camera >> frame;

        if (frame.empty())
        {
            std::cout << "Failed to capture frame\n";
            break;
        }
        //display each frame
        cv::imshow("Camera", frame);

        //check key-board input
        int key = cv::waitKey(1);

        //close program when 'q' pressed
        if (key== 'q')
        {
            break;
        }

        //capture image when 'c' pressed
        if (key == 'c')
        {
             if (cv::imwrite("capture.jpg", frame))
        {
            std::cout << "Image captured successfully\n";
        }
        else
        {
            std::cout << "Failed to save image\n";
        }
        }
    }

    camera.release();
    cv::destroyAllWindows();

    return 0;
}