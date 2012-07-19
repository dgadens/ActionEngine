using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using ACFramework.Font;
using System.IO;

namespace ACEditor
{
    public partial class frmImportFont : Form
    {
        public frmImportFont()
        {
            InitializeComponent();
        }

        private void btnFindFile_Click(object sender, EventArgs e)
        {
            OpenFileDialog of = new OpenFileDialog();
            of.Filter = "Font Files(*.FNT)|*.FNT|All files (*.*)|*.*";
            if (of.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                txtFilePath.Text = of.FileName;
        }

        private void btnFindFolder_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog fd = new FolderBrowserDialog();
            if (fd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                txtDestination.Text = fd.SelectedPath;
        }

        private void frmImportFont_Load(object sender, EventArgs e)
        {

        }

        private void btnImport_Click(object sender, EventArgs e)
        {
            if (!File.Exists(txtFilePath.Text))
            {
                MessageBox.Show("Source Path not found.");
                return;
            }

            if (!Directory.Exists(txtDestination.Text))
            {
                MessageBox.Show("Destination Path not found.");
                return;
            }

            ConvertBMFont convert = new ConvertBMFont();
            string fontName = Path.GetFileNameWithoutExtension(txtFilePath.Text);
            string ret = convert.Convert(txtFilePath.Text, txtDestination.Text + "\\" + fontName + ".aft");

            MessageBox.Show(ret);
        }
    }
}
