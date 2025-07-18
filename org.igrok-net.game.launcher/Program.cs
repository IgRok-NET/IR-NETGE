using IRNETGE.Engine;
using IRNETGE.Launcher.UI;
using System;
using System.Windows.Forms;

namespace IRNETGE.Launcher
{
    internal class Program
    {
        private static readonly MainWindow ui = new MainWindow();
        [STAThread]
        static void Main(string[] args)
        {
            Console.WriteLine($"IgRok-NET Game Engine v{Defaults.ENGINE_VERSION}");

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            Application.Run(new MainWindow());
        }
    }
}
