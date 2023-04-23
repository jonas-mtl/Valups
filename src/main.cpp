#include "core/screen-parser.hpp"


int main()
{
    // Capture Valorant screenshot
    cv::Mat valSrc{};
    RECT valRect{};

    if (!valups::parser::getWindowRect("Notepad", valRect))
    {
        fprintf(stderr, "-- Failed to find process! \n");
    }

    if (valups::parser::captureScreenMat(valRect, valSrc))
    {
        cv::imwrite("Screenshot.png", valSrc);
    }

    std::string test;
    std::cin >> test;

    return 0;
}