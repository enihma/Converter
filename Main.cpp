#include "App.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int nCmdShow)
{
    App app{};

    return app.Run(hInst, nCmdShow);
}