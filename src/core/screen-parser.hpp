#pragma once
#include <Windows.h>
#include <winuser.h>
#include <opencv2/opencv.hpp>

namespace valups
{
    namespace parser
    {
        BOOL CALLBACK enumWindowsProc(HWND hwnd, LPARAM lParam);

        BOOL getWindowRect(const char* windowSearchParams, RECT& outputWindowRect);

        BOOL captureScreenMat(RECT targetRect, cv::Mat& outputMat);

        BITMAPINFOHEADER createBitmapHeader(int width, int height);
    }
}
