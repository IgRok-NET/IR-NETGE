#include "pch.h"

#include "org.igrok-net.game.natives.h"

IRNETGE::Natives::IGNNativeWindow::IGNNativeWindow(Panel^ panel)
{
	HWND panelHandle = (HWND)panel->Handle.ToPointer();
	this->nativeWindow = new UnmanagedNativeWindow(panelHandle);
}

void IRNETGE::Natives::IGNNativeWindow::NativeDraw()
{
	this->nativeWindow->Draw();
}

IRNETGE::Natives::IGNNativeWindow::~IGNNativeWindow()
{
	delete this->nativeWindow;
}
