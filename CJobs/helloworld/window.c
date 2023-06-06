#include <windows.h>

const char nomeClasse[] = "Como criar uma classe";

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_LBUTTONDOWN:
            char nomeArquivo[MAX_PATH];
            HINSTANCE hInstance = GetModuleHandle(NULL);

            GetModuleFileName(hInstance, nomeArquivo, MAX_PATH);
            MessageBox(hwnd, nomeArquivo, "Esse programa eh: ", MB_OK | MB_ICONINFORMATION);
        break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX classeWindow;
    HWND hwnd;
    MSG msg;

    classeWindow.cbSize        = sizeof(WNDCLASSEX);
    classeWindow.style         = 0;
    classeWindow.lpfnWndProc   = WndProc;
    classeWindow.cbClsExtra    = 0;
    classeWindow.cbWndExtra    = 0;
    classeWindow.hInstance     = hInstance;
    classeWindow.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    classeWindow.hCursor       = LoadCursor(NULL, IDC_ARROW);
    classeWindow.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    classeWindow.lpszMenuName  = NULL;
    classeWindow.lpszClassName = nomeClasse;
    classeWindow.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&classeWindow))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,
        nomeClasse, 
        "HOW 2 WINDOW", 
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
        NULL, NULL, hInstance, NULL
    );

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }else
    {
        MessageBox(NULL, "Window Creation Success!", "Nice!", MB_ICONINFORMATION | MB_OK);
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
    
    

}