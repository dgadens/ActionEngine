using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using ACFramework.FileStructs;
using System.IO;
using System.Threading;
using ACFramework.FileStruct;

namespace ACEditor
{
    public partial class frmImport : Form
    {
        public frmImport()
        {
            InitializeComponent();
        }

        private void btnFindFile_Click(object sender, EventArgs e)
        {
            OpenFileDialog of = new OpenFileDialog();
            of.Filter = "Model Files(*.DAE;*.MS3D)|*.DAE;*.MS3D|All files (*.*)|*.*";
            if (of.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                txtFilePath.Text = of.FileName;
        }

        private void btnFindFolder_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog fd = new FolderBrowserDialog();
            if (fd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                txtDestination.Text = fd.SelectedPath;
        }

        private void btnImport_Click(object sender, EventArgs e)
        {
            Import _import = new Import();
            _import.PropertyChanged += new PropertyChangedEventHandler(_import_PropertyChanged);

            //try
            //{
                AMT_MODEL? model = null;
                IConverter converter = null;

                string fileType = Path.GetExtension(txtFilePath.Text).ToUpper();

                switch (fileType)
                {
                    case ".DAE":
                        converter = new COLLADAConverter();
                        model = converter.Convert(txtFilePath.Text, _import);
                        break;
                }

                //se fez a conversao entao ele grava o arquivo amt
                if (model.HasValue)
                {
                    string fileName = Path.GetFileNameWithoutExtension(txtFilePath.Text);
                    fileName += ".amt";
                    AMTExport export = new AMTExport();
                    export.Export(model.Value, txtDestination.Text + "\\" + fileName, _import);

                    MessageBox.Show("Arquivo gerado com sucesso.");
                }
            //}
            //catch (Exception ex)
            //{
            //    MessageBox.Show(ex.Message);
            //}
        }

        void _import_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            lblImportProgress.Text = e.PropertyName;
            lblImportProgress.Refresh();
        }

        private void frmImport_Load(object sender, EventArgs e)
        {

        }
    }
}
