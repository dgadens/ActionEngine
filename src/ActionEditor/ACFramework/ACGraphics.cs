//classe com os wrappers vindos da dll
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Reflection;
using System.Threading;

namespace ACFramework
{
    public class ACGraphics
    {
        #region Metodos externos

        [DllImport(ACGlobals.dllPath, EntryPoint = "InitializeGraphicsDevice", CallingConvention = CallingConvention.Cdecl)]
        static extern IntPtr __InitializeGraphicsDevice(IntPtr hInst,
                                   [MarshalAs(UnmanagedType.LPStr)] string chAPI,
                                   IntPtr hWnd,
                                   [MarshalAs(UnmanagedType.Bool)] bool saveLog);

        [DllImport(ACGlobals.dllPath, EntryPoint = "AddViewport", CallingConvention = CallingConvention.Cdecl)]
        static extern IntPtr __AddViewport(IntPtr hWnd);

        [DllImport(ACGlobals.dllPath, EntryPoint = "DropViewport", CallingConvention = CallingConvention.Cdecl)]
        static extern void __DropViewport(IntPtr hWnd);

        [DllImport(ACGlobals.dllPath, EntryPoint = "SetWindowSize", CallingConvention = CallingConvention.Cdecl)]
        static extern void __SetWindowSize(IntPtr hWnd,
                                          [MarshalAs(UnmanagedType.I4)] int width,
                                          [MarshalAs(UnmanagedType.I4)] int height);
        
        [DllImport(ACGlobals.dllPath, EntryPoint = "ActiveViewport", CallingConvention = CallingConvention.Cdecl)]
        static extern void __ActiveViewport(IntPtr hWnd);

        [DllImport(ACGlobals.dllPath, EntryPoint = "SetClearColor", CallingConvention = CallingConvention.Cdecl)]
        static extern void __SetClearColorW([MarshalAs(UnmanagedType.R4)] float r,
                                          [MarshalAs(UnmanagedType.R4)] float g,
                                          [MarshalAs(UnmanagedType.R4)] float b);

        [DllImport(ACGlobals.dllPath, EntryPoint = "Lock", CallingConvention = CallingConvention.Cdecl)]
        static extern void __Lock();

        [DllImport(ACGlobals.dllPath, EntryPoint = "Unlock", CallingConvention = CallingConvention.Cdecl)]
        static extern void __Unlock();

        [DllImport(ACGlobals.dllPath, EntryPoint = "Update", CallingConvention = CallingConvention.Cdecl)]
        static extern void __Update();

        [DllImport(ACGlobals.dllPath, EntryPoint = "Render", CallingConvention = CallingConvention.Cdecl)]
        static extern void __Render();

        [DllImport(ACGlobals.dllPath, EntryPoint = "Release", CallingConvention = CallingConvention.Cdecl)]
        static extern void __Release();

        #endregion

        public static bool MainInitialize = false;
        public static List<IntPtr> hWndList = new List<IntPtr>();

        private static ACGraphics _instance;
        public static ACGraphics Instance
        {
            get 
            {
                if (_instance == null)
                    _instance = new ACGraphics();

                return _instance;
            }
        }

        public void Init(IntPtr hInst, IntPtr mainHWND, APIType type, bool saveLog)
        {
            if (!MainInitialize)
            {
                string graphicsAPI = null;
                switch (type)
                {
                    case APIType.DirectX9:   graphicsAPI = "ACD3DEngine.dll";  break;
                    case APIType.Direct3D10: graphicsAPI = "ACD3DEngine.dll"; break;
                    case APIType.Direct3D11: graphicsAPI = "ACD3DEngine.dll"; break;
                    case APIType.OpenGl2:    graphicsAPI = "OpenGl2Engine.dll"; break;
                    case APIType.OpenGl3:    graphicsAPI = "OpenGl3Engine.dll"; break;
                    case APIType.OpenGl4:    graphicsAPI = "OpenGl4Engine.dll"; break;
                    default: throw new Exception("Graphics API not supported."); 
                }

                IntPtr hresult = __InitializeGraphicsDevice(hInst, graphicsAPI, mainHWND, saveLog);

                if (hresult.ToInt32() != 0)
                    throw new Exception("Error on graphics device initializer.");

                MainInitialize = true;
                hWndList.Add(mainHWND);
            }
        }

        public void AddViewport(IntPtr hWnd)
        {
            hWndList.Add(hWnd);
            __AddViewport(hWnd);
        }

        public void DropViewport(IntPtr hWnd)
        {
            __DropViewport(hWnd);
            hWndList.Remove(hWnd);
            if (hWndList.Count <= 0)
            {
                __Release();
            }
        }

        public void DropAll()
        {
            for (int i = hWndList.Count - 1; i >= 0; i--)
                __DropViewport(hWndList[i]);
            hWndList.Clear();
        }

        public void ActiveViewport(IntPtr hWnd)
        {
            __ActiveViewport(hWnd);
        }

        public void SetWindowSize(IntPtr hWnd, int width, int height)
        {
            __SetWindowSize(hWnd, width, height);
        }

        public void SetClearColor(Vector3 value)
        {
            __SetClearColorW(value.X, value.Y, value.Z);
        }

        //renderiza
        public void Render()
        {
            __Update();
            __Render();
        }
    }
}
