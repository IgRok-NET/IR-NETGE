#include "pch.h"
#include "UnmanagedNativeWindow.h"

IRNETGE::Natives::UnmanagedNativeWindow::UnmanagedNativeWindow(HWND glPanel)
{
	this->panelHandle = glPanel;
	this->pfd = { sizeof(PIXELFORMATDESCRIPTOR), 1 };
	this->pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	this->pfd.iPixelType = PFD_TYPE_RGBA;
	this->pfd.cColorBits = 32;
	this->pfd.cDepthBits = 24;
	this->pfd.iLayerType = PFD_MAIN_PLANE;

	this->hdc = GetDC(this->panelHandle);
	this->pf = ChoosePixelFormat(this->hdc, &this->pfd);
	SetPixelFormat(this->hdc, this->pf, &this->pfd);

	this->hglrc = wglCreateContext(this->hdc);
	wglMakeCurrent(this->hdc, this->hglrc);
}

void IRNETGE::Natives::UnmanagedNativeWindow::Draw()
{
	SwapBuffers(this->hdc);
}

IRNETGE::Natives::UnmanagedNativeWindow::~UnmanagedNativeWindow()
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(this->hglrc);
	ReleaseDC(this->panelHandle, this->hdc);
}
