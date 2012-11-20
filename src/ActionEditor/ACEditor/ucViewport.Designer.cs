namespace ACEditor
{
    partial class ucViewport
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.pnlVPContainer = new System.Windows.Forms.Panel();
            this.lblInformation = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // pnlVPContainer
            // 
            this.pnlVPContainer.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.pnlVPContainer.BackColor = System.Drawing.Color.Black;
            this.pnlVPContainer.Location = new System.Drawing.Point(4, 4);
            this.pnlVPContainer.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.pnlVPContainer.Name = "pnlVPContainer";
            this.pnlVPContainer.Size = new System.Drawing.Size(192, 177);
            this.pnlVPContainer.TabIndex = 0;
            this.pnlVPContainer.MouseDown += new System.Windows.Forms.MouseEventHandler(this.pnlVPContainer_MouseDown);
            // 
            // lblInformation
            // 
            this.lblInformation.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lblInformation.BackColor = System.Drawing.Color.DimGray;
            this.lblInformation.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblInformation.ForeColor = System.Drawing.Color.White;
            this.lblInformation.Location = new System.Drawing.Point(4, 182);
            this.lblInformation.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblInformation.Name = "lblInformation";
            this.lblInformation.Size = new System.Drawing.Size(192, 24);
            this.lblInformation.TabIndex = 1;
            this.lblInformation.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.lblInformation.MouseDown += new System.Windows.Forms.MouseEventHandler(this.lblInformation_MouseDown);
            // 
            // ucViewport
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.lblInformation);
            this.Controls.Add(this.pnlVPContainer);
            this.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.Name = "ucViewport";
            this.Size = new System.Drawing.Size(200, 209);
            this.Load += new System.EventHandler(this.ucViewport_Load);
            this.Enter += new System.EventHandler(this.ucViewport_Enter);
            this.Resize += new System.EventHandler(this.ucViewport_Resize);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel pnlVPContainer;
        private System.Windows.Forms.Label lblInformation;
    }
}
