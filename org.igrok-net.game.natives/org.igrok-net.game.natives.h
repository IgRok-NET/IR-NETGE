#pragma once

using namespace System;
using namespace System::Diagnostics;
using namespace System::Threading;

namespace IRNETGE {
	namespace Natives {
		public ref class IGNNativeWindow
		{
		private:
			HWND windowHandle;
			HINSTANCE hInstance;
			HICON appIcon;

			HDC hdc;
			HGLRC hglrc;

			static bool isClosing;

			int targetFps;

			void DisableVSync();
			void InitOpenGL();
			void CleanupOpenGL();

		public:
			IGNNativeWindow();
			~IGNNativeWindow();
			!IGNNativeWindow();

			void Run();
			void SetIcon(IntPtr iconPointer);
			void SetFpsTarget(int target);

			static LRESULT ProcessMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		};
	}
}
