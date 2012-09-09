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
        ACInputW _acInput;
        ACInputDeviceW _acInputDevice;

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
            _acRenderDevice.Init(Handle, true, true);
            _acRenderDevice.SetActiveViewport(Handle);

            _acInput = new ACInputW(hInstance);
            _acInput.CreateDevice("ACDIEngine.dll");
            _acInputDevice = _acInput.GetDevice();
        }
    }
}
