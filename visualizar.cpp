#include <opencv2/opencv.hpp>
#include <Windows.h>

using namespace std;
using namespace cv;

int main(int argc, char* argv[]) {
    FreeConsole();
    Mat input = imread(argv[1]);
    namedWindow("PDI-2023", WINDOW_AUTOSIZE);
    imshow("PDI-2023", input);
    while (getWindowProperty("PDI-2023", WND_PROP_VISIBLE)) {
        char c = waitKeyEx(50);
        if (c == ' ') break;
    }
    destroyWindow("PDI-2023");
    return 0;
}
