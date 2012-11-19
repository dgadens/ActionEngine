using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using EngineWrapper;
using EngineMathCLI;

namespace DemoCLI
{
    public partial class Form1 : Form
    {
        IntPtr hWnd;

        ACRendererW _renderer;
        ACRenderDeviceW _renderDevice;
        ACInputW _input;
        ACInputDeviceW _inputDevice;
        ACMouseW _mouse;
        ACKeyboardW _keyboard;
        ACNetworkW _network;
        ACNetworkDeviceW _networkDevice;
        ACAudioW _audio;
        ACAudioDeviceW _audioDevice;
        ACContentManagerW _contentManager;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            ACConfigurationsW.Initialize();

            var hInstance = Marshal.GetHINSTANCE(typeof(Form).Module);
            hWnd = panel1.Handle;

            _renderer = new ACRendererW(hInstance);
            _renderer.CreateDevice("ACD3DEngine.dll");
            _renderDevice = _renderer.GetDevice();
            _renderDevice.Init(hWnd, true, true);
            _renderDevice.SetActiveViewport(hWnd);

            //_input = new ACInputW(hInstance);
            //_input.CreateDevice("ACDIEngine.dll");
            //_inputDevice = _input.GetDevice();

            //_input.CreateMouse();
            //_mouse = _input.GetMouse();

            //_input.CreateKeyboard();
            //_keyboard = _input.GetKeyboard();

            //_network = new ACNetworkW(hInstance);
            //_network.CreateDevice("ACWSEngine.dll");
            //_networkDevice = _network.GetDevice();

            _audio = new ACAudioW(hInstance);
            _audio.CreateDevice("ACOpenALEngine.dll");
            _audioDevice = _audio.GetDevice();
            _audioDevice.Init(hWnd, true);
            _audioDevice.SetListener(Vector3.Zero, Vector3.Zero, Vector3.Zero, Vector3.Zero);

            _contentManager = new ACContentManagerW(_renderDevice, _audioDevice);

            _renderDevice.GoFullscreen(0);
            _renderDevice.SetClearColor(new Vector4(0, 0, 0, 1));
            _renderDevice.SetWireColor(new Vector3(1, 1, 0));
            //_renderDevice.SetRasterizeState(ACRASTERIZESTATEW::ACRS_SolidCullCCW);
            //_renderDevice.SetShadeMode(ACSHADEMODEW::ACSM_LineList);
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {


        }

        private void pnlVP_Paint(object sender, PaintEventArgs e)
        {
        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {
            _renderDevice.SetActiveRenderingViewport(hWnd);
            _renderDevice.SetClearColor(new Vector4(0, 0, 0, 1));
            _renderDevice.BeginRendering();
            _renderDevice.EndRendering();
        }

        private void panel1_Validated(object sender, EventArgs e)
        {
          
        }
    }
}
