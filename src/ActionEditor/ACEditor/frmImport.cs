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
        DataTable animationsData;

        public frmImport()
        {
            InitializeComponent();

            animationsData = new DataTable();
            animationsData.Columns.Add("Name", typeof(string));
            animationsData.Columns.Add("Start", typeof(uint));
            animationsData.Columns.Add("End", typeof(uint));
        }

        private void btnFindFile_Click(object sender, EventArgs e)
        {
            OpenFileDialog of = new OpenFileDialog();
            of.Filter = "Model Files(*.DAE)|*.DAE|All files (*.*)|*.*";
            if (of.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                txtFilePath.Text = of.FileName;
                animationsData.Clear();
            }
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

            try
            {
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

                //adiciona as animacoes q estao na interface
                AddAnimations(ref model);

                //se fez a conversao entao ele grava o arquivo amt
                if (model.HasValue)
                {
                    string fileName = Path.GetFileNameWithoutExtension(txtFilePath.Text);
                    fileName += ".amt";
                    AMTExport export = new AMTExport();
                    export.Export(model.Value, txtDestination.Text + "\\" + fileName, _import);

                    MessageBox.Show("Arquivo gerado com sucesso.");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        void AddAnimations(ref AMT_MODEL? amtModel)
        {
            List<AMT_ANIMATION> animations = new List<AMT_ANIMATION>();
            for (int i = 0; i < animationsData.Rows.Count; i++)
			{
                var animation = new AMT_ANIMATION()
                { 
			        StartFrame = (uint)animationsData.Rows[i]["Start"],
                    EndFrame = (uint)animationsData.Rows[i]["End"],
                    Name = animationsData.Rows[i]["Name"].ToString()
                };
                animations.Add(animation);
			}

            AMT_MODEL im = amtModel.Value;
            im.Animations = animations;
            im.Head.NumAnimations = (uint)animations.Count;
            amtModel = im;
        }

        void _import_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            lblImportProgress.Text = e.PropertyName;
            lblImportProgress.Refresh();
        }

        private void frmImport_Load(object sender, EventArgs e)
        {

        }

        private void btnAddAnimation_Click(object sender, EventArgs e)
        {
            DataRow row = animationsData.NewRow();
            row["Name"] = txtAnimationName.Text;
            row["Start"] = uint.Parse(txtInitFrame.Text);
            row["End"] = uint.Parse(txtFinalFrame.Text);
            animationsData.Rows.Add(row);

            dgvAnimations.DataSource = animationsData;
        }
    }
}
