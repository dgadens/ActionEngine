using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using ACFramework;

namespace ACEditor
{
    public partial class MDIEditor : Form
    {
        frmModelManager _frmModelManager;
        frmImport _frmImport;
        frmImportFont _frmImportFont;

        public MDIEditor()
        {
            InitializeComponent();
        }

        private void ShowNewForm(object sender, EventArgs e)
        {

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

        private void SaveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog saveFileDialog = new SaveFileDialog();
            saveFileDialog.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.Personal);
            saveFileDialog.Filter = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*";
            if (saveFileDialog.ShowDialog(this) == DialogResult.OK)
            {
                string FileName = saveFileDialog.FileName;
            }
        }

        private void ExitToolsStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void ToolBarToolStripMenuItem_Click(object sender, EventArgs e)
        {
            toolStrip.Visible = toolBarToolStripMenuItem.Checked;
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

        private void MDIEditor_Load(object sender, EventArgs e)
        {
            VpMain.InitializeVP();
            VpMain.ClearColor = new Vector3(0, 0, 0);
            VpMain.Draw();
        }

        private void mnuImportModel_Click(object sender, EventArgs e)
        {
            if (_frmImport == null || _frmImport.IsDisposed)
                _frmImport = new frmImport();

            _frmImport.Show();
        }

        private void tsbAddModel_Click(object sender, EventArgs e)
        {
            if (_frmModelManager == null || _frmModelManager.IsDisposed)
                _frmModelManager = new frmModelManager();

            _frmModelManager.Show();
            _frmModelManager.BringToFront();
        }

        private void MDIEditor_FormClosing(object sender, FormClosingEventArgs e)
        {
            ACFramework.ACGraphics.Instance.DropAll();
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

        private void mnuImportFont_Click(object sender, EventArgs e)
        {
            if (_frmImportFont == null || _frmImportFont.IsDisposed)
                _frmImportFont = new frmImportFont();

            _frmImportFont.Show();
        }
    }
}
