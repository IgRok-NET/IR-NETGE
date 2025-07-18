using IRNETGE.Natives;
using System.Windows.Forms;

namespace IRNETGE.Launcher.UI
{
    public class GLPanel : Panel
    {
        private readonly IGNNativeWindow nw;


        public GLPanel()
        {
            this.SetStyle(ControlStyles.UserPaint |
                          ControlStyles.AllPaintingInWmPaint |
                          ControlStyles.Opaque, true);
            this.UpdateStyles();

            this.nw = new IGNNativeWindow(this);
        }

        protected override void OnPaintBackground(PaintEventArgs e)
        {
            // Do nothing – prevents background erasing and GDI+ DC allocation
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            // Don't use e.Graphics
            this.nw.NativeDraw();
        }
        protected override void Dispose(bool disposing)
        {
            base.Dispose(disposing);
            this.nw.Dispose();
        }
    }
}
