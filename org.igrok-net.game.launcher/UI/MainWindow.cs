using IRNETGE.Natives;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace IRNETGE.Launcher.UI
{
    public partial class MainWindow : Form
    {
        private Timer renderTimer;
        private GLPanel glPanel;
        private int counter;

        public MainWindow()
        {
            InitializeComponent();

            this.glPanel = new GLPanel
            {
                Dock = DockStyle.Fill
            };

            this.Controls.Add(glPanel);
            this.counter = 0;

            renderTimer = new Timer
            {
                Interval = 16
            };
            renderTimer.Tick += (s, e) =>
            {
                this.glPanel.Invalidate();
                this.counter++;
                if(this.counter == 120)
                {
                    this.counter = 0;
                    GC.Collect();
                }
            };
            renderTimer.Start();
        }

        private void MainWindow_FormClosing(object sender, FormClosingEventArgs e)
        {
        }
    }
}
