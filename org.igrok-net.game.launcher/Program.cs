using IRNETGE.Engine;
using IRNETGE.Natives;
using System;
using System.Drawing;

namespace IRNETGE.Launcher
{
    internal class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine($"IgRok-NET Game Engine v{Defaults.ENGINE_VERSION}");
            using (Icon appIcon = new Icon("icon.ico")) // load .ico file
            {
                IntPtr hIcon = appIcon.Handle;

                var window = new IGNNativeWindow();
                window.SetIcon(hIcon);   // pass HICON to native window
                window.SetFpsTarget(120);
                window.Run();
                window.Dispose();
            }
        }
    }
}
