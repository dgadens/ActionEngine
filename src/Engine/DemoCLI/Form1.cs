using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using EngineWrapper;
using System.Runtime.InteropServices;

namespace DemoCLI
{
    public partial class Form1 : Form
    {
        ACRendererW _acRenderer;
        ACRenderDeviceW _acRenderDevice;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            ACConfigurationsW.Initialize();

            var hInstance = Marshal.GetHINSTANCE(typeof(Form1).Module);
            _acRenderer = new ACRendererW(hInstance);
            _acRenderer.CreateDevice("ACD3DEngine.dll");
            _acRenderDevice = _acRenderer.GetDevice();
        }
    }
}
