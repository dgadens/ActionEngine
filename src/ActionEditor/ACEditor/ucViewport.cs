using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using ACFramework;
using System.Runtime.InteropServices;
using System.Threading;

namespace ACEditor
{
    public partial class ucViewport : UserControl
    {
        //segura a unica vp ativa so vai ter uma sempre
        public static ucViewport CurrentViewport;

        bool Initialized = false;
        IntPtr hWnd;

        private bool _active;
        public bool Active 
        {
            get { return _active; }
            set
            {
                _active = value;
                if (_active)
                {
                    CurrentViewport = this;
                    //ACGraphics.Instance.ActiveViewport(hWnd);
                }
            }
        }

        private Vector3 _clearColor;
        public Vector3 ClearColor
        {
            set
            {
                _clearColor = value;
                //ACGraphics.Instance.SetClearColor(_clearColor);
            }

            get
            {
                return _clearColor;
            }
        }

        private string _information;
        public string Information
        {
            set
            {
                _information = value;
                lblInformation.Text = _information;
            }

            get
            {
                return _information;
            }
        }


        public ucViewport()
        {
            InitializeComponent();
        }

        private void ucViewport_Load(object sender, EventArgs e)
        {
            this.BackColor = Color.DarkGray;
            pnlVPContainer.MouseClick += new MouseEventHandler(pnlVPContainer_MouseClick);
        }

        void pnlVPContainer_MouseClick(object sender, MouseEventArgs e)
        {
            if (!this.Focus())
                this.Focus();
        }

        protected override void OnCreateControl()
        {
            base.OnCreateControl();
            this.ParentForm.FormClosing += new FormClosingEventHandler(ParentForm_FormClosing);
        }

        protected override void OnGotFocus(EventArgs e)
        {
            base.OnGotFocus(e);
            this.BackColor = Color.White;
            Active = true;
        }

        protected override void OnLostFocus(EventArgs e)
        {
            base.OnLostFocus(e);
            this.BackColor = Color.DarkGray;
        }

        protected override void OnMouseDown(MouseEventArgs e)
        {
            base.OnMouseDown(e);
        }

        protected override void OnMouseClick(MouseEventArgs e)
        {
            base.OnMouseClick(e);
            this.Focus();
        }

        void ParentForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            //ACGraphics.Instance.DropViewport(hWnd);
        }

        public void InitializeVP()
        {
           /* IntPtr hInst = Marshal.GetHINSTANCE(typeof(ucViewport).Module);
            hWnd = pnlVPContainer.Handle;

            if (!ACGraphics.MainInitialize)
            {
                ACGraphics.Instance.Init(hInst, hWnd, APIType.Direct3D10, true);
                CurrentViewport = this;
            }
            else
                ACGraphics.Instance.AddViewport(hWnd);
            */
            Initialized = true;
        }

        public void AddModel(string name)
        {
            //ACGraphics.Instance.AddModel(name);
        }

        public void ShowNormals(string name)
        {
            //ACGraphics.Instance.ShowNormals(name);
        }

        private void ucViewport_Resize(object sender, EventArgs e)
        {
            //if (Initialized)
              //  ACGraphics.Instance.SetWindowSize(hWnd, pnlVPContainer.Width, pnlVPContainer.Height);
        }

        private void ucViewport_Enter(object sender, EventArgs e)
        {
           // ACGraphics.Instance.ActiveViewport(hWnd);
        }

        private void pnlVPContainer_MouseDown(object sender, MouseEventArgs e)
        {
            //chama o evento do uc
            OnMouseDown(e);
        }

        private void lblInformation_MouseDown(object sender, MouseEventArgs e)
        {
            //chama o evento do uc
            OnMouseDown(e);
        }

        public void Draw()
        {
            //ACGraphics.Instance.Render();
        }
    }
}
