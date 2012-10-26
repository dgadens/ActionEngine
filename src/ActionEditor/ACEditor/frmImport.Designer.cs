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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label3 = new System.Windows.Forms.Label();
            this.txtInitFrame = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.txtFinalFrame = new System.Windows.Forms.TextBox();
            this.btnAddAnimation = new System.Windows.Forms.Button();
            this.dgvAnimations = new System.Windows.Forms.DataGridView();
            this.label5 = new System.Windows.Forms.Label();
            this.txtAnimationName = new System.Windows.Forms.TextBox();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgvAnimations)).BeginInit();
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
            this.btnImport.Location = new System.Drawing.Point(23, 445);
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
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.txtAnimationName);
            this.groupBox1.Controls.Add(this.dgvAnimations);
            this.groupBox1.Controls.Add(this.btnAddAnimation);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.txtFinalFrame);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.txtInitFrame);
            this.groupBox1.Location = new System.Drawing.Point(23, 92);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(746, 334);
            this.groupBox1.TabIndex = 17;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Animations";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(296, 43);
            this.label3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(84, 17);
            this.label3.TabIndex = 11;
            this.label3.Text = "Initial Frame";
            // 
            // txtInitFrame
            // 
            this.txtInitFrame.Location = new System.Drawing.Point(384, 40);
            this.txtInitFrame.Margin = new System.Windows.Forms.Padding(4);
            this.txtInitFrame.Name = "txtInitFrame";
            this.txtInitFrame.Size = new System.Drawing.Size(64, 22);
            this.txtInitFrame.TabIndex = 10;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(455, 43);
            this.label4.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(82, 17);
            this.label4.TabIndex = 13;
            this.label4.Text = "Final Frame";
            // 
            // txtFinalFrame
            // 
            this.txtFinalFrame.Location = new System.Drawing.Point(545, 40);
            this.txtFinalFrame.Margin = new System.Windows.Forms.Padding(4);
            this.txtFinalFrame.Name = "txtFinalFrame";
            this.txtFinalFrame.Size = new System.Drawing.Size(64, 22);
            this.txtFinalFrame.TabIndex = 12;
            // 
            // btnAddAnimation
            // 
            this.btnAddAnimation.Location = new System.Drawing.Point(629, 37);
            this.btnAddAnimation.Margin = new System.Windows.Forms.Padding(4);
            this.btnAddAnimation.Name = "btnAddAnimation";
            this.btnAddAnimation.Size = new System.Drawing.Size(100, 28);
            this.btnAddAnimation.TabIndex = 18;
            this.btnAddAnimation.Text = "Add";
            this.btnAddAnimation.UseVisualStyleBackColor = true;
            this.btnAddAnimation.Click += new System.EventHandler(this.btnAddAnimation_Click);
            // 
            // dgvAnimations
            // 
            this.dgvAnimations.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgvAnimations.Location = new System.Drawing.Point(19, 81);
            this.dgvAnimations.Name = "dgvAnimations";
            this.dgvAnimations.RowTemplate.Height = 24;
            this.dgvAnimations.Size = new System.Drawing.Size(710, 236);
            this.dgvAnimations.TabIndex = 19;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(20, 43);
            this.label5.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(45, 17);
            this.label5.TabIndex = 21;
            this.label5.Text = "Name";
            // 
            // txtAnimationName
            // 
            this.txtAnimationName.Location = new System.Drawing.Point(73, 40);
            this.txtAnimationName.Margin = new System.Windows.Forms.Padding(4);
            this.txtAnimationName.Name = "txtAnimationName";
            this.txtAnimationName.Size = new System.Drawing.Size(215, 22);
            this.txtAnimationName.TabIndex = 20;
            // 
            // frmImport
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.Gray;
            this.ClientSize = new System.Drawing.Size(785, 486);
            this.Controls.Add(this.groupBox1);
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
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgvAnimations)).EndInit();
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
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox txtAnimationName;
        private System.Windows.Forms.DataGridView dgvAnimations;
        private System.Windows.Forms.Button btnAddAnimation;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox txtFinalFrame;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox txtInitFrame;
    }
}