#include "screen-parser.hpp"

namespace valups
{
    namespace parser
    {
        BOOL CALLBACK enumWindowsProc(HWND hwnd, LPARAM lParam) {
            std::vector<std::string>* titles = reinterpret_cast<std::vector<std::string>*>(lParam);

            char title[256];
            if (GetWindowText(hwnd, title, sizeof(title))) {
                titles->push_back(title);
            }

            return true;
        }

        BOOL getWindowRect(const char* windowSearchParams, RECT& outputWindowRect)
        {
            std::vector<std::string> windowTitles;
            if (!EnumWindows(enumWindowsProc, reinterpret_cast<LPARAM>(&windowTitles))) {
                fprintf(stderr, "-- Failed to enumerate windows! \n");
                return false;
            }

            for (const auto& title : windowTitles) {
                if (title.find(windowSearchParams) != std::string::npos) {
                    HWND hwndVal = FindWindow(nullptr, static_cast<LPCSTR>(title.c_str()));
                    RECT rect{};
                    GetWindowRect(hwndVal, &rect);
                    outputWindowRect = rect;

                    printf("Val pos: %d \n", rect.left);
                    return true;
                }
            }    

            return false;
        }

        BITMAPINFOHEADER createBitmapHeader(int width, int height)
        {
            BITMAPINFOHEADER bi{};

            // create a bitmap
            bi.biSize = sizeof(BITMAPINFOHEADER);
            bi.biWidth = width;
            bi.biHeight = -height;
            bi.biPlanes = 1;
            bi.biBitCount = 32;
            bi.biCompression = BI_RGB;
            bi.biSizeImage = 0;
            bi.biXPelsPerMeter = 0;
            bi.biYPelsPerMeter = 0;
            bi.biClrUsed = 0;
            bi.biClrImportant = 0;

            return bi;
        }

        BOOL captureScreenMat(RECT targetRect, cv::Mat& outputMat)
        {
            cv::Mat src;

            // get handles to a device context (DC)
            HDC hwindowDC = GetDC(GetDesktopWindow());
            HDC hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
            SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

            // define scale, height and width
            int width = targetRect.right - targetRect.left;
            int height = targetRect.bottom - targetRect.top;

            // create mat object
            src.create(height, width, CV_8UC4);

            // create a bitmap
            HBITMAP hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
            BITMAPINFOHEADER bi = createBitmapHeader(width, height);

            // use the previously created device context with the bitmap
            SelectObject(hwindowCompatibleDC, hbwindow);

            // copy from the window device context to the bitmap device context
            StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, targetRect.left, targetRect.top, width, height, SRCCOPY);
            GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

            // avoid memory leak
            DeleteObject(hbwindow);
            DeleteDC(hwindowCompatibleDC);
            ReleaseDC(GetDesktopWindow(), hwindowDC);

            outputMat = src;
            return true;
        }
    }
}