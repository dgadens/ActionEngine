using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using EngineMathCLI;

namespace ACEditor
{
    public partial class MDIMain : Form
    {
        frmModelManager _frmModelManager;
        frmImport _frmImport;
        frmImportFont _frmImportFont;

        private int childFormNumber = 0;

        public MDIMain()
        {
            InitializeComponent();
        }

        private void MDIMain_Load(object sender, EventArgs e)
        {
            VpMain.InitializeVP();
            VpMain.ClearColor = new Vector3(0, 0, 0);
            VpMain.Draw();
        }

        private void OpenFile(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.Personal);
            openFileDialog.Filter = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*";
            if (openFileDialog.ShowDialog(this) == DialogResult.OK)
            {
                string FileName = openFileDialog.FileName;
            }
        }

        private void mnuSaveAs_Click(object sender, EventArgs e)
        {
            SaveFileDialog saveFileDialog = new SaveFileDialog();
            saveFileDialog.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.Personal);
            saveFileDialog.Filter = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*";
            if (saveFileDialog.ShowDialog(this) == DialogResult.OK)
            {
                string FileName = saveFileDialog.FileName;
            }
        }

        private void mnuExit_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void mnuNew_Click(object sender, EventArgs e)
        {

        }

        private void mnuCut_Click(object sender, EventArgs e)
        {
        }

        private void mnuCopy_Click(object sender, EventArgs e)
        {
        }

        private void mnuPaste_Click(object sender, EventArgs e)
        {
        }

        private void StatusBarToolStripMenuItem_Click(object sender, EventArgs e)
        {
            statusStrip.Visible = statusBarToolStripMenuItem.Checked;
        }

        private void CascadeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            LayoutMdi(MdiLayout.Cascade);
        }

        private void TileVerticalToolStripMenuItem_Click(object sender, EventArgs e)
        {
            LayoutMdi(MdiLayout.TileVertical);
        }

        private void TileHorizontalToolStripMenuItem_Click(object sender, EventArgs e)
        {
            LayoutMdi(MdiLayout.TileHorizontal);
        }

        private void ArrangeIconsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            LayoutMdi(MdiLayout.ArrangeIcons);
        }

        private void CloseAllToolStripMenuItem_Click(object sender, EventArgs e)
        {
            foreach (Form childForm in MdiChildren)
            {
                childForm.Close();
            }
        }

        private void mnuImportModels_Click(object sender, EventArgs e)
        {
            if (_frmImport == null || _frmImport.IsDisposed)
                _frmImport = new frmImport();

            _frmImport.Show();
        }

        private void mnuImportFonts_Click(object sender, EventArgs e)
        {
            if (_frmImportFont == null || _frmImportFont.IsDisposed)
                _frmImportFont = new frmImportFont();

            _frmImportFont.Show();
        }

        private void mnuModels_Click(object sender, EventArgs e)
        {
            if (_frmModelManager == null || _frmModelManager.IsDisposed)
                _frmModelManager = new frmModelManager();

            _frmModelManager.Show();
        }


        private void VpMain_DragDrop(object sender, DragEventArgs e)
        {
            VpMain.Active = true;
            VpMain.AddModel(e.Data.GetData(DataFormats.Text).ToString());
        }

        private void VpMain_DragEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.Text))
                e.Effect = DragDropEffects.Copy;
            else
                e.Effect = DragDropEffects.None;
        }
    }
}
