using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Threading;
using ACFramework;
using System.IO;

namespace ACEditor
{
    public partial class frmModelManager : Form
    {
        List<ucViewport> _vps = new List<ucViewport>();

        public frmModelManager()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            string basePath = Environment.CurrentDirectory + @"\..\..\..\..\ShadowWorld\Contents\Models";
            string[] files = Directory.GetFiles(basePath);
            for (int i = 0; i < files.Length; i++)
            {
                string fileName = Path.GetFileName(files[i]);
                if (Path.GetExtension(files[i]) == ".amt")
                {
                    ucViewport vp = new ucViewport();
                    vp.Width = 200;
                    vp.Height = 200;
                    vp.InitializeVP();
                    vp.Active = true;
                    vp.AddModel(fileName);
                    vp.Information = fileName;
                    vp.MouseDown += new MouseEventHandler(vp_MouseDown);
                    _vps.Add(vp);
                }
            }

            flpVpsContainer.Controls.AddRange(_vps.ToArray());
        }

        void vp_MouseDown(object sender, MouseEventArgs e)
        {
            ucViewport vp = sender as ucViewport;
            vp.DoDragDrop(vp.Information, DragDropEffects.Copy);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            ucViewport.CurrentViewport.AddModel("dude.amt");
        }

        private void btnShowNormals_Click(object sender, EventArgs e)
        {
            ucViewport.CurrentViewport.ShowNormals("dude.amt");
        }
    }
}
