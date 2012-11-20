//classe com os wrappers vindos da dll
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Reflection;
using System.Threading;
using EngineMathCLI;

namespace ACFramework
{
    public class ACGraphics
    {
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

               /* IntPtr hresult = __InitializeGraphicsDevice(hInst, graphicsAPI, mainHWND, saveLog);

                if (hresult.ToInt32() != 0)
                    throw new Exception("Error on graphics device initializer.");

                MainInitialize = true;
                hWndList.Add(mainHWND);*/
            }
        }

        public void AddViewport(IntPtr hWnd)
        {
            hWndList.Add(hWnd);
           // __AddViewport(hWnd);
        }

        public void DropViewport(IntPtr hWnd)
        {
           /* __DropViewport(hWnd);
            hWndList.Remove(hWnd);
            if (hWndList.Count <= 0)
            {
                __Release();
            }*/
        }

        public void DropAll()
        {
            /*for (int i = hWndList.Count - 1; i >= 0; i--)
                __DropViewport(hWndList[i]);
            hWndList.Clear();*/
        }

        public void ActiveViewport(IntPtr hWnd)
        {
            //__ActiveViewport(hWnd);
        }

        public void SetWindowSize(IntPtr hWnd, int width, int height)
        {
            //__SetWindowSize(hWnd, width, height);
        }

        public void SetClearColor(Vector3 value)
        {
            //__SetClearColorW(value.X, value.Y, value.Z);
        }

        //renderiza
        public void Render()
        {
           /* __Update();
            __Render();*/
        }
    }
}
