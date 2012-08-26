using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using EngineWrapper;

namespace DemoCLI
{
    public partial class Form1 : Form
    {
        ACRendererW _acRenderer;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            _acRenderer = new ACRendererW(IntPtr.Zero);
        }
    }
}
