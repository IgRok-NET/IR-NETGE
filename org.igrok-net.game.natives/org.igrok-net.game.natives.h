#pragma once

#include "UnmanagedNativeWindow.h"

using namespace System;
using namespace System::Windows::Forms;

namespace IRNETGE {
	namespace Natives {
		public ref class IGNNativeWindow
		{
		private:
			UnmanagedNativeWindow* nativeWindow;
		public:
			IGNNativeWindow(Panel^ panel);
			void NativeDraw();
			~IGNNativeWindow();
		};
	}
}
