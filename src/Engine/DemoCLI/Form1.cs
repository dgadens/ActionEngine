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
        ACRendererW _renderer;
        ACRenderDeviceW _renderDevice;
        ACInputW _input;
        ACInputDeviceW _inputDevice;
        ACMouseW _mouse;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            ACConfigurationsW.Initialize();

            var hInstance = Marshal.GetHINSTANCE(typeof(Form1).Module);

            _renderer = new ACRendererW(hInstance);
            _renderer.CreateDevice("ACD3DEngine.dll");
            _renderDevice = _renderer.GetDevice();
            _renderDevice.Init(Handle, true, true);
            _renderDevice.SetActiveViewport(Handle);

            _input = new ACInputW(hInstance);
            _input.CreateDevice("ACDIEngine.dll");
            _inputDevice = _input.GetDevice();

            _input.CreateMouse();
            _mouse = _input.GetMouse();
        }
    }
}
