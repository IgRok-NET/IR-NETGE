#include "pch.h"

#include "org.igrok-net.game.natives.h"

static LRESULT CALLBACK NativeWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return IRNETGE::Natives::IGNNativeWindow::ProcessMessage(hwnd, msg, wParam, lParam);
}

void IRNETGE::Natives::IGNNativeWindow::DisableVSync()
{
    const char* vendor = (const char*)glGetString(GL_VENDOR);
    const char* renderer = (const char*)glGetString(GL_RENDERER);
    const char* version = (const char*)glGetString(GL_VERSION);
    System::Diagnostics::Debug::WriteLine(gcnew System::String(vendor));
    System::Diagnostics::Debug::WriteLine(gcnew System::String(renderer));
    System::Diagnostics::Debug::WriteLine(gcnew System::String(version));

    // Declare the function pointer type (can live here or at file scope)
    typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC)(int interval);

    // Get pointer to the function
    PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT =
        (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");

    System::Diagnostics::Debug::WriteLine(
        "wglSwapIntervalEXT address: " +
        System::IntPtr((void*)wglSwapIntervalEXT).ToString("X")
    );

    if (wglSwapIntervalEXT)
    {
        wglSwapIntervalEXT(0); // 0 = disable V-Sync
    }
    else
    {
        System::Diagnostics::Debug::WriteLine("wglSwapIntervalEXT not available");
    }
}

void IRNETGE::Natives::IGNNativeWindow::InitOpenGL()
{
    this->hdc = GetDC(this->windowHandle);

    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pf = ChoosePixelFormat(this->hdc, &pfd);
    SetPixelFormat(this->hdc, pf, &pfd);

    this->hglrc = wglCreateContext(this->hdc);
    wglMakeCurrent(this->hdc, this->hglrc);

    this->DisableVSync();

    // Set clear color once
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // white background
}

void IRNETGE::Natives::IGNNativeWindow::CleanupOpenGL()
{
    if (this->hglrc)
    {
        ChangeDisplaySettings(nullptr, 0); // restore desktop mode
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(this->hglrc);
        this->hglrc = NULL;
    }
    if (this->hdc && this->windowHandle)
    {
        ReleaseDC(this->windowHandle, this->hdc);
        this->hdc = NULL;
    }
}

IRNETGE::Natives::IGNNativeWindow::IGNNativeWindow()
{
    this->hInstance = GetModuleHandle(NULL);
    this->isClosing = false;
    this->targetFps = 60;
}

IRNETGE::Natives::IGNNativeWindow::~IGNNativeWindow()
{
    this->!IGNNativeWindow();
}

IRNETGE::Natives::IGNNativeWindow::!IGNNativeWindow()
{
    CleanupOpenGL(); // one-time teardown
}

void IRNETGE::Natives::IGNNativeWindow::Run()
{
    // Register class
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = NativeWndProc;
    wc.hInstance = this->hInstance;
    wc.lpszClassName = L"IRNETGENativeWindow";
    wc.hIcon = this->appIcon;

    RegisterClass(&wc);

    // Get primary monitor size
    RECT rc;
    HMONITOR hmon = MonitorFromPoint(POINT{ 0,0 }, MONITOR_DEFAULTTOPRIMARY);
    MONITORINFO mi = { sizeof(mi) };
    GetMonitorInfo(hmon, &mi);
    rc = mi.rcMonitor;

    DEVMODE dm;
    ZeroMemory(&dm, sizeof(dm));
    dm.dmSize = sizeof(dm);
    dm.dmPelsWidth = rc.right - rc.left;
    dm.dmPelsHeight = rc.bottom - rc.top;
    dm.dmBitsPerPel = 32;          // usually 32bpp
    dm.dmDisplayFrequency = 75;    // or read from EnumDisplaySettings if you want the exact refresh
    dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_DISPLAYFREQUENCY;

    if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
        MessageBox(nullptr, L"Failed to set exclusive fullscreen!", L"Error", MB_OK);
    }

    // Create window class as usual (register WNDCLASS with your WndProc)

    // Then create window:
    this->windowHandle = CreateWindowEx(
        0,                             // no extended styles (or add WS_EX_TOPMOST if you want)
        wc.lpszClassName,
        L"Fullscreen Window",
        WS_POPUP | WS_VISIBLE,         // WS_POPUP = no border/title bar
        rc.left, rc.top,               // position top-left of monitor
        rc.right - rc.left,            // width of monitor
        rc.bottom - rc.top,            // height of monitor
        NULL, NULL, this->hInstance, NULL);

    if (!this->windowHandle)
    {
        // handle error
    }

    // Show and update
    ShowWindow(this->windowHandle, SW_SHOW);
    UpdateWindow(this->windowHandle);

    InitOpenGL();

    double frameDuration = 1000.0 / targetFps; // ms per frame
    Stopwatch^ sw = Stopwatch::StartNew();
    MSG msg;
    double lastFrameTime = 0.0;

    while (!isClosing) {
        // Handle all Windows messages as fast as possible
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                isClosing = true;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        double now = sw->Elapsed.TotalMilliseconds;

        if (now - lastFrameTime >= frameDuration) {
            // Render only if enough time has passed
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            SwapBuffers(hdc);

            lastFrameTime = now;
        }
    }
}

void IRNETGE::Natives::IGNNativeWindow::SetIcon(IntPtr iconPointer)
{
    this->appIcon = (HICON)iconPointer.ToPointer();
    if (this->windowHandle != NULL)
    {
        // Update window icon immediately if window already exists
        SendMessage(this->windowHandle, WM_SETICON, ICON_BIG, (LPARAM)this->appIcon);
        SendMessage(this->windowHandle, WM_SETICON, ICON_SMALL, (LPARAM)this->appIcon);
    }
}

void IRNETGE::Natives::IGNNativeWindow::SetFpsTarget(int target)
{
    this->targetFps = target;
}

LRESULT IRNETGE::Natives::IGNNativeWindow::ProcessMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
        {
            IGNNativeWindow::isClosing = true;
            PostQuitMessage(0);
            return 0;
        }
        break;
    case WM_SETCURSOR:
        SetCursor(LoadCursor(NULL, IDC_ARROW)); // normal arrow cursor
        return TRUE;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
