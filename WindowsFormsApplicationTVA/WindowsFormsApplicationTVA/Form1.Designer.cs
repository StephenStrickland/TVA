﻿namespace WindowsFormsApplicationTVA
{
    partial class Form1
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            System.Windows.Forms.ListViewItem listViewItem1 = new System.Windows.Forms.ListViewItem(new string[] {
            "Test",
            "another test"}, -1);
            System.Windows.Forms.ColumnHeader ElapsedFrames;
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.toolStripButton1 = new System.Windows.Forms.ToolStripButton();
            this.toolStripButton2 = new System.Windows.Forms.ToolStripDropDownButton();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.streamToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripButton3 = new System.Windows.Forms.ToolStripButton();
            this.label1 = new System.Windows.Forms.Label();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.label4 = new System.Windows.Forms.Label();
            this.button2 = new System.Windows.Forms.Button();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.listView1 = new System.Windows.Forms.ListView();
            this.Cars = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.Valid = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.CenterPoint = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.pbRectangle = new System.Windows.Forms.PictureBox();
            this.Completed = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            ElapsedFrames = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.toolStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.statusStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbRectangle)).BeginInit();
            this.SuspendLayout();
            // 
            // toolStrip1
            // 
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripButton1,
            this.toolStripButton2,
            this.toolStripButton3});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
<<<<<<< HEAD
            this.toolStrip1.Size = new System.Drawing.Size(1140, 25);
=======
            this.toolStrip1.Size = new System.Drawing.Size(1029, 25);
>>>>>>> 8f4ebcdda7de4828c1af67f5a695a2eb11dd363a
            this.toolStrip1.TabIndex = 0;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // toolStripButton1
            // 
            this.toolStripButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.toolStripButton1.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton1.Image")));
            this.toolStripButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton1.Name = "toolStripButton1";
            this.toolStripButton1.Size = new System.Drawing.Size(39, 22);
            this.toolStripButton1.Text = "Save";
            this.toolStripButton1.Click += new System.EventHandler(this.toolStripButton1_Click);
            // 
            // toolStripButton2
            // 
            this.toolStripButton2.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.toolStripButton2.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.streamToolStripMenuItem});
            this.toolStripButton2.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton2.Image")));
            this.toolStripButton2.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton2.Name = "toolStripButton2";
            this.toolStripButton2.Size = new System.Drawing.Size(50, 22);
            this.toolStripButton2.Text = "Load";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(117, 22);
            this.fileToolStripMenuItem.Text = "File";
            this.fileToolStripMenuItem.Click += new System.EventHandler(this.fileToolStripMenuItem_Click);
            // 
            // streamToolStripMenuItem
            // 
            this.streamToolStripMenuItem.Name = "streamToolStripMenuItem";
            this.streamToolStripMenuItem.Size = new System.Drawing.Size(117, 22);
            this.streamToolStripMenuItem.Text = "Stream";
            this.streamToolStripMenuItem.Click += new System.EventHandler(this.streamToolStripMenuItem_Click);
            // 
            // toolStripButton3
            // 
            this.toolStripButton3.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.toolStripButton3.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton3.Image")));
            this.toolStripButton3.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton3.Name = "toolStripButton3";
            this.toolStripButton3.Size = new System.Drawing.Size(51, 22);
            this.toolStripButton3.Text = "Viewer";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 27);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(35, 12);
            this.label1.TabIndex = 1;
            this.label1.Text = "Video";
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackColor = System.Drawing.Color.CadetBlue;
<<<<<<< HEAD
            this.pictureBox1.Location = new System.Drawing.Point(16, 41);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(681, 473);
=======
            this.pictureBox1.Location = new System.Drawing.Point(12, 45);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(558, 331);
>>>>>>> 8f4ebcdda7de4828c1af67f5a695a2eb11dd363a
            this.pictureBox1.TabIndex = 14;
            this.pictureBox1.TabStop = false;
            this.pictureBox1.MouseDown += new System.Windows.Forms.MouseEventHandler(this.pictureBox1_MouseDown);
<<<<<<< HEAD
            this.pictureBox1.MouseMove += new System.Windows.Forms.MouseEventHandler(this.pictureBox1_MouseMove);
            this.pictureBox1.MouseUp += new System.Windows.Forms.MouseEventHandler(this.pictureBox1_MouseUp);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(13, 516);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(71, 12);
            this.label2.TabIndex = 3;
            this.label2.Text = "Preferences";
=======
>>>>>>> 8f4ebcdda7de4828c1af67f5a695a2eb11dd363a
            // 
            // comboBox1
            // 
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Items.AddRange(new object[] {
            "Box",
            "Line"});
<<<<<<< HEAD
            this.comboBox1.Location = new System.Drawing.Point(12, 555);
=======
            this.comboBox1.Location = new System.Drawing.Point(12, 407);
>>>>>>> 8f4ebcdda7de4828c1af67f5a695a2eb11dd363a
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(80, 20);
            this.comboBox1.TabIndex = 6;
            // 
<<<<<<< HEAD
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(100, 554);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 21);
            this.button1.TabIndex = 7;
            this.button1.Text = "Draw";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(9, 540);
=======
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(9, 391);
>>>>>>> 8f4ebcdda7de4828c1af67f5a695a2eb11dd363a
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(71, 12);
            this.label4.TabIndex = 8;
            this.label4.Text = "Test Method";
            // 
            // button2
            // 
<<<<<<< HEAD
            this.button2.Location = new System.Drawing.Point(622, 555);
=======
            this.button2.Location = new System.Drawing.Point(108, 405);
>>>>>>> 8f4ebcdda7de4828c1af67f5a695a2eb11dd363a
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 21);
            this.button2.TabIndex = 9;
            this.button2.Text = "Analyze";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
<<<<<<< HEAD
            this.openFileDialog1.FileOk += new System.ComponentModel.CancelEventHandler(this.openFileDialog1_FileOk);
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(195, 554);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(120, 21);
            this.button3.TabIndex = 13;
            this.button3.Text = "Place Lane Watchers";
            this.button3.UseVisualStyleBackColor = true;
            // 
            // pictureBox2
            // 
            this.pictureBox2.BackColor = System.Drawing.Color.Beige;
            this.pictureBox2.Location = new System.Drawing.Point(757, 41);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(389, 191);
            this.pictureBox2.TabIndex = 15;
            this.pictureBox2.TabStop = false;
            this.pictureBox2.Click += new System.EventHandler(this.pictureBox2_Click);
=======
>>>>>>> 8f4ebcdda7de4828c1af67f5a695a2eb11dd363a
            // 
            // listView1
            // 
            this.listView1.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.Cars,
            ElapsedFrames,
            this.Valid,
            this.CenterPoint,
            this.Completed});
            this.listView1.GridLines = true;
            this.listView1.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.listView1.HideSelection = false;
<<<<<<< HEAD
            this.listView1.Location = new System.Drawing.Point(826, 317);
            this.listView1.Name = "listView1";
            this.listView1.Size = new System.Drawing.Size(320, 197);
=======
            this.listView1.Items.AddRange(new System.Windows.Forms.ListViewItem[] {
            listViewItem1});
            this.listView1.Location = new System.Drawing.Point(612, 45);
            this.listView1.Name = "listView1";
            this.listView1.Size = new System.Drawing.Size(405, 331);
>>>>>>> 8f4ebcdda7de4828c1af67f5a695a2eb11dd363a
            this.listView1.TabIndex = 16;
            this.listView1.UseCompatibleStateImageBehavior = false;
            this.listView1.View = System.Windows.Forms.View.Details;
            // 
            // Cars
            // 
            this.Cars.Text = "ID";
            this.Cars.Width = 53;
            // 
            // ElapsedFrames
            // 
            ElapsedFrames.Text = "Frames";
            ElapsedFrames.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            ElapsedFrames.Width = 99;
            // 
            // Valid
            // 
            this.Valid.Text = "Time";
            this.Valid.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // CenterPoint
            // 
            this.CenterPoint.Text = "Is Valid";
            this.CenterPoint.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.CenterPoint.Width = 80;
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1});
            this.statusStrip1.Location = new System.Drawing.Point(0, 446);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(1029, 22);
            this.statusStrip1.TabIndex = 17;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(0, 17);
            // 
            // pbRectangle
            // 
            this.pbRectangle.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pbRectangle.Location = new System.Drawing.Point(136, 112);
            this.pbRectangle.Name = "pbRectangle";
            this.pbRectangle.Size = new System.Drawing.Size(558, 331);
            this.pbRectangle.TabIndex = 18;
            this.pbRectangle.TabStop = false;
            this.pbRectangle.MouseDown += new System.Windows.Forms.MouseEventHandler(this.pictureBox1_MouseDown);
            this.pbRectangle.MouseMove += new System.Windows.Forms.MouseEventHandler(this.pictureBox1_MouseMove);
            this.pbRectangle.MouseUp += new System.Windows.Forms.MouseEventHandler(this.pictureBox1_MouseUp);
            // 
            // Completed
            // 
            this.Completed.Text = "Completed";
            this.Completed.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.Completed.Width = 99;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
<<<<<<< HEAD
            this.ClientSize = new System.Drawing.Size(1140, 587);
=======
            this.ClientSize = new System.Drawing.Size(1029, 468);
            this.Controls.Add(this.pbRectangle);
            this.Controls.Add(this.statusStrip1);
>>>>>>> 8f4ebcdda7de4828c1af67f5a695a2eb11dd363a
            this.Controls.Add(this.listView1);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.comboBox1);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.toolStrip1);
            this.Name = "Form1";
            this.Text = "Traffic Video Analysis";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbRectangle)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton toolStripButton1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.ToolStripButton toolStripButton3;
        private System.Windows.Forms.ToolStripDropDownButton toolStripButton2;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem streamToolStripMenuItem;
        private System.Windows.Forms.ListView listView1;
        private System.Windows.Forms.ColumnHeader Cars;
        private System.Windows.Forms.ColumnHeader Valid;
        private System.Windows.Forms.ColumnHeader CenterPoint;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.PictureBox pbRectangle;
        private System.Windows.Forms.ColumnHeader Completed;


    }
}

