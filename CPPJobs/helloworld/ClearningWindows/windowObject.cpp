#ifndef UNICODE
#define UNICODE
#endif 

#include <shobjidl.h>
#include <windows.h>
#include <d2d1.h>
#include <math.h>
#include <assert.h>
//#include <atlbase.h>

#include "learningwins.h"
//#include "lScene.h"

/*class Scene : public GraphicsScene
{
    CCompPtr<ID2D1SolidColorBrush> m_pFill;
    CComPtr<ID2D1SolidColorBrush> m_pStroke;

    D2D1_ELLIPSE m_ellipse;
    D2D_POINT_2F m_Ticks[24];

    HRESULT CreateDeviceIndependentResources() { return S_OK;}
    void    DiscardDeviceIndependentResources() { }
    HRESULT CreateDeviceDependentResources();
    void    DiscardDeviceDependentResources();
    void    CalculateLayout();
    void    RenderScene();

    void DrawClockHand(float fHandLength, float fAngle, float fStrokeWidth);

};

HRESULT Scene::CreateDeviceDependentResources(){}*/

template <class T> void SafeRelease(T **ppT)
{
    if(*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

class MainWindow : public BaseWindow<MainWindow>
{
    ID2D1Factory            *pFactory;
    ID2D1HwndRenderTarget   *pRenderTarget;
    ID2D1SolidColorBrush    *pBrush;
    D2D1_ELLIPSE            ellipse;

    void        CalculateLayout();
    HRESULT     CreateGraphicsResources();
    void        DiscardGraphicsResources();
    void        OnPaint();
    void        Resize();

public:
    MainWindow() : pFactory(NULL), pRenderTarget(NULL), pBrush(NULL)
    {

    }

    PCWSTR  ClassName() { return L"How 2 Circle"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

void MainWindow::CalculateLayout()
{
    if(pRenderTarget != NULL)
    {
        D2D1_SIZE_F size = pRenderTarget->GetSize();
        const float x = size.width / 2;
        const float y = size.height / 5;
        const float radius = std::min(x,y);
        ellipse = D2D1::Ellipse(D2D1::Point2F(x,y), radius, radius*1.33f);

    }
}

HRESULT MainWindow::CreateGraphicsResources()
{
    HRESULT hr = S_OK;
    if (pRenderTarget == NULL)
    {
        RECT rc;
        GetClientRect(m_hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

        hr = pFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(m_hwnd, size),
            &pRenderTarget);

        if (SUCCEEDED(hr))
        {
            const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0);
            hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);

            if (SUCCEEDED(hr))
            {
                CalculateLayout();
            }
        }
    }
    return hr;
}

void MainWindow::DiscardGraphicsResources()
{
    SafeRelease(&pRenderTarget);
    SafeRelease(&pBrush);
}

void MainWindow::OnPaint()
{
    HRESULT hr = CreateGraphicsResources();
    if (SUCCEEDED(hr))
    {
        PAINTSTRUCT ps;
        BeginPaint(m_hwnd, &ps);
     
        pRenderTarget->BeginDraw();

        pRenderTarget->Clear( D2D1::ColorF(D2D1::ColorF::SkyBlue) );
        pRenderTarget->FillEllipse(ellipse, pBrush);

        hr = pRenderTarget->EndDraw();
        if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
        {
            DiscardGraphicsResources();
        }
        EndPaint(m_hwnd, &ps);
    }
}

void MainWindow::Resize()
{
    if (pRenderTarget != NULL)
    {
        RECT rc;
        GetClientRect(m_hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

        pRenderTarget->Resize(size);
        CalculateLayout();
        InvalidateRect(m_hwnd, NULL, FALSE);
    }
}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd)
{
    MainWindow win;
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | 
        COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileOpenDialog *pFileOpen;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, 
                IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem *pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    // Display the file name to the user.
                    if (SUCCEEDED(hr))
                    {
                        MessageBoxW(NULL, pszFilePath, L"File Path", MB_OK);
                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }

    if(!win.Create( L"How 2 Circle", WS_OVERLAPPEDWINDOW))
    {
        return 0;
    }

    ShowWindow(win.Window(), nShowCmd);

    MSG msg = { };
    while(GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
        case WM_CREATE:
            if(FAILED(D2D1CreateFactory(
                D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
            {
                return -1;
            }
            return 0;
        /*case WM_LBUTTONDOWN:
            {
                wchar_t nomeArquivo[MAX_PATH];
                HINSTANCE hInstance = GetModuleHandle(NULL);

                GetModuleFileName(hInstance, nomeArquivo, MAX_PATH);
                MessageBox(MainWindow::m_hwnd, nomeArquivo, L"Esse programa eh: ", MB_OK | MB_ICONINFORMATION);
            }
            return 0;*/
        case WM_CLOSE:
        {
            if (MessageBox(MainWindow::m_hwnd, L"Really quit?", L"Close program", MB_YESNO) == IDYES)
            {
                DestroyWindow(MainWindow::m_hwnd);
            }
            // Else: User canceled. Do nothing.
            return 0;
        }
        case WM_DESTROY:
            DiscardGraphicsResources();
            SafeRelease(&pFactory);
            PostQuitMessage(0);
            return 0;

        case WM_PAINT:

            OnPaint();
            return 0;
        case WM_SIZE:

            Resize();
            return 0;
        default:
            return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
        }
        return TRUE;
    }
