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
        [DllImport(ACGlobals.dllPath, EntryPoint = "InitWrapper", CallingConvention = CallingConvention.Cdecl)]
        static extern IntPtr InitW(IntPtr hInst,
                                   [MarshalAs(UnmanagedType.LPStr)] string chAPI,
                                   IntPtr hWnd,
                                   [MarshalAs(UnmanagedType.Bool)] bool saveLog);

        [DllImport(ACGlobals.dllPath, EntryPoint = "AddViewportWrapper", CallingConvention = CallingConvention.Cdecl)]
        static extern IntPtr AddViewportW(IntPtr hWnd);
        [DllImport(ACGlobals.dllPath, EntryPoint = "DropViewportWrapper", CallingConvention = CallingConvention.Cdecl)]
        static extern void DropViewportW(IntPtr hWnd);
        [DllImport(ACGlobals.dllPath, EntryPoint = "ActiveViewportWrapper", CallingConvention = CallingConvention.Cdecl)]
        static extern void ActiveViewportW(IntPtr hWnd);
        [DllImport(ACGlobals.dllPath, EntryPoint = "LockWrapper", CallingConvention = CallingConvention.Cdecl)]
        static extern void LockW();
        [DllImport(ACGlobals.dllPath, EntryPoint = "UnlockWrapper", CallingConvention = CallingConvention.Cdecl)]
        static extern void UnlockW();

        //usar o mesmo nome do methodo dentro da dll pra nao precisar do entrypoint
        //[return: MarshalAs(UnmanagedType.Bool)]

        [DllImport(ACGlobals.dllPath, EntryPoint = "AddModelWrapper", CallingConvention = CallingConvention.Cdecl)]
        static extern void AddModelW([MarshalAs(UnmanagedType.LPStr)] string name);
        [DllImport(ACGlobals.dllPath, EntryPoint = "ShowNormalsWrapper", CallingConvention = CallingConvention.Cdecl)]
        static extern void ShowNormalsW([MarshalAs(UnmanagedType.LPStr)] string name);
        [DllImport(ACGlobals.dllPath, EntryPoint = "ReleaseWrapper", CallingConvention = CallingConvention.Cdecl)]
        static extern void ReleaseW();
        [DllImport(ACGlobals.dllPath, EntryPoint = "SetWindowSizeWrapper", CallingConvention = CallingConvention.Cdecl)]
        static extern void SetWindowSizeW(IntPtr hWnd,
                                          [MarshalAs(UnmanagedType.I4)] int width,
                                          [MarshalAs(UnmanagedType.I4)] int height);

        [DllImport(ACGlobals.dllPath, EntryPoint = "UpdateWrapper", CallingConvention = CallingConvention.Cdecl)]
        static extern void UpdateW();
        [DllImport(ACGlobals.dllPath, EntryPoint = "RenderWrapper", CallingConvention = CallingConvention.Cdecl)]
        static extern void RenderW();
        [DllImport(ACGlobals.dllPath, EntryPoint = "SetClearColorWrapper", CallingConvention = CallingConvention.Cdecl)]
        static extern void SetClearColorW([MarshalAs(UnmanagedType.R4)] float r,
                                          [MarshalAs(UnmanagedType.R4)] float g,
                                          [MarshalAs(UnmanagedType.R4)] float b);
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
                InitW(hInst, type.ToString(), mainHWND, saveLog);
                MainInitialize = true;
                hWndList.Add(mainHWND);
            }
        }

        public void AddViewport(IntPtr hWnd)
        {
            hWndList.Add(hWnd);
            AddViewportW(hWnd);
        }

        public void DropViewport(IntPtr hWnd)
        {
            DropViewportW(hWnd);
            hWndList.Remove(hWnd);
            if (hWndList.Count <= 0)
            {
                ReleaseW();
            }
        }

        public void DropAll()
        {
            for (int i = hWndList.Count - 1; i >= 0; i--)
            {
                DropViewport(hWndList[i]);
            }
            hWndList.Clear();
        }

        public void ActiveViewport(IntPtr hWnd)
        {
            ActiveViewportW(hWnd);
        }

        public void AddModel(string name)
        {
            AddModelW(name);
        }

        public void ShowNormals(string name)
        {
            ShowNormalsW(name);
        }

        public void SetWindowSize(IntPtr hWnd, int width, int height)
        {
            SetWindowSizeW(hWnd, width, height);
        }

        public void SetClearColor(Vector3 value)
        {
            SetClearColorW(value.X, value.Y, value.Z);
        }

        //renderiza
        public void Draw()
        {
            UpdateW();
            RenderW();
        }
    }
}
