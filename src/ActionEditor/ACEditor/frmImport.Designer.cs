namespace ACEditor
{
    partial class frmImport
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.label2 = new System.Windows.Forms.Label();
            this.btnFindFolder = new System.Windows.Forms.Button();
            this.txtDestination = new System.Windows.Forms.TextBox();
            this.btnImport = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.btnFindFile = new System.Windows.Forms.Button();
            this.txtFilePath = new System.Windows.Forms.TextBox();
            this.lblImportProgress = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(20, 50);
            this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(79, 17);
            this.label2.TabIndex = 13;
            this.label2.Text = "Destination";
            // 
            // btnFindFolder
            // 
            this.btnFindFolder.Location = new System.Drawing.Point(669, 44);
            this.btnFindFolder.Margin = new System.Windows.Forms.Padding(4);
            this.btnFindFolder.Name = "btnFindFolder";
            this.btnFindFolder.Size = new System.Drawing.Size(100, 28);
            this.btnFindFolder.TabIndex = 12;
            this.btnFindFolder.Text = "Find";
            this.btnFindFolder.UseVisualStyleBackColor = true;
            this.btnFindFolder.Click += new System.EventHandler(this.btnFindFolder_Click);
            // 
            // txtDestination
            // 
            this.txtDestination.Location = new System.Drawing.Point(108, 47);
            this.txtDestination.Margin = new System.Windows.Forms.Padding(4);
            this.txtDestination.Name = "txtDestination";
            this.txtDestination.Size = new System.Drawing.Size(552, 22);
            this.txtDestination.TabIndex = 11;
            this.txtDestination.Text = "C:\\GitHub\\ActionEngine\\src\\Engine\\Demo\\Contents\\Models";
            // 
            // btnImport
            // 
            this.btnImport.Location = new System.Drawing.Point(609, 97);
            this.btnImport.Margin = new System.Windows.Forms.Padding(4);
            this.btnImport.Name = "btnImport";
            this.btnImport.Size = new System.Drawing.Size(160, 28);
            this.btnImport.TabIndex = 10;
            this.btnImport.Text = "Import";
            this.btnImport.UseVisualStyleBackColor = true;
            this.btnImport.Click += new System.EventHandler(this.btnImport_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(20, 18);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(30, 17);
            this.label1.TabIndex = 9;
            this.label1.Text = "File";
            // 
            // btnFindFile
            // 
            this.btnFindFile.Location = new System.Drawing.Point(669, 12);
            this.btnFindFile.Margin = new System.Windows.Forms.Padding(4);
            this.btnFindFile.Name = "btnFindFile";
            this.btnFindFile.Size = new System.Drawing.Size(100, 28);
            this.btnFindFile.TabIndex = 8;
            this.btnFindFile.Text = "Find";
            this.btnFindFile.UseVisualStyleBackColor = true;
            this.btnFindFile.Click += new System.EventHandler(this.btnFindFile_Click);
            // 
            // txtFilePath
            // 
            this.txtFilePath.Location = new System.Drawing.Point(108, 15);
            this.txtFilePath.Margin = new System.Windows.Forms.Padding(4);
            this.txtFilePath.Name = "txtFilePath";
            this.txtFilePath.Size = new System.Drawing.Size(552, 22);
            this.txtFilePath.TabIndex = 7;
            // 
            // lblImportProgress
            // 
            this.lblImportProgress.AutoSize = true;
            this.lblImportProgress.Location = new System.Drawing.Point(20, 110);
            this.lblImportProgress.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblImportProgress.Name = "lblImportProgress";
            this.lblImportProgress.Size = new System.Drawing.Size(0, 17);
            this.lblImportProgress.TabIndex = 14;
            // 
            // frmImport
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.Gray;
            this.ClientSize = new System.Drawing.Size(785, 138);
            this.Controls.Add(this.lblImportProgress);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.btnFindFolder);
            this.Controls.Add(this.txtDestination);
            this.Controls.Add(this.btnImport);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.btnFindFile);
            this.Controls.Add(this.txtFilePath);
            this.Margin = new System.Windows.Forms.Padding(4);
            this.MaximizeBox = false;
            this.Name = "frmImport";
            this.Opacity = 0.9D;
            this.Text = "Import";
            this.Load += new System.EventHandler(this.frmImport_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button btnFindFolder;
        private System.Windows.Forms.TextBox txtDestination;
        private System.Windows.Forms.Button btnImport;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnFindFile;
        private System.Windows.Forms.TextBox txtFilePath;
        private System.Windows.Forms.Label lblImportProgress;
    }
}