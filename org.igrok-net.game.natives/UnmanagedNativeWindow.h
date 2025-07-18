#pragma once

namespace IRNETGE {
	namespace Natives {

		public class UnmanagedNativeWindow
		{

		private:
			HWND panelHandle;
			PIXELFORMATDESCRIPTOR pfd;
			HDC hdc;
			int pf;
			HGLRC hglrc;
		public:
			UnmanagedNativeWindow(HWND glPanel);
			void Draw();
			~UnmanagedNativeWindow();
		};

	}
}