using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Emgu.CV;
using Emgu.Util;
using Emgu.CV.CvEnum;
using Emgu.CV.UI;
using Emgu.CV.Structure;
using Emgu.CV.VideoSurveillance;
using System.Diagnostics;
using AForge.Video.DirectShow;
using AForge.Video;


namespace WindowsFormsApplicationTVA
{
    public partial class Form1 : Form
    {
        public Form1()
        {
           // mRect.Parent.Parent = pictureBox1;
            InitializeComponent();
            this.DoubleBuffered = true;
            
        }
        Rectangle mRect;
        bool draw = false;

        //bool draw = false;
        //int s = 3;
        //Color color = Color.Red;
        //private void pictureBox1_MouseDown(object sender, MouseEventArgs e)
        //{
        //    draw = true;
        //    Graphics g = Graphics.FromImage(pictureBox1.Image);
        //    Pen pen1 = new Pen(color, 4);
        //    g.DrawRectangle(pen1, e.X, e.Y, 2, 2);
        //    g.Save();
        //    pictureBox1.Image = pictureBox1.Image;
        //}
        //private void pictureBox1_MouseUp(object sender, MouseEventArgs e)
        //{
        //    draw = false;
        //}

        //private void pictureBox1_MouseMove(object sender, MouseEventArgs e)
        //{
        //    if (draw)
        //    {
        //        Graphics g = Graphics.FromImage(pictureBox1.Image);
        //        SolidBrush brush = new SolidBrush(color);
        //        g.FillRectangle(brush, e.X, e.Y, s, s);
        //        g.Save();
        //        pictureBox1.Image = mainimage;
        //    }
        //}

        //drawing cs = new drawing() { ev = 2, 34 };


     

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            Form f = new Form();
            f.ShowDialog(this);
        }

        private void openFileDialog1_FileOk(object sender, CancelEventArgs e)
        {

        }

        private void toolStripButton2_Click(object sender, EventArgs e)
        {
            //    openFileDialog1.Title = "Load Video";
            //    openFileDialog1.Filter = "AVI| *.avi";
            //    if (openFileDialog1.ShowDialog()==DialogResult.OK)
            //    {

            //        //string fileName = openFileDialog1.FileName;
            //       string fileName = System.IO.Path.GetDirectoryName(openFileDialog1.FileName);
            //        PlayVideoFile(fileName);
            //    }
            //}
        }

        


        private void PlayVideoFile(string fileName)
        {
            string test = "hello world";
            Capture cap = null;
            bool captureInProgress = false;
            int CameratDevice = 0;
            int framWidth;
            int frameHeight;
            int frameCount;
            Capture _Capture;
            VideoWriter VW;
            
         

           cap =  new Capture(fileName);

            Image<Bgr, byte> img = cap.QueryFrame();

            ImageViewer vi = new ImageViewer();
            vi.Image = img;
            vi.ShowDialog();

            Stopwatch SW;


            //DisplayImage(_Capture.RetrieveBgrFrame().ToBitmap());
 
            //        //Show time stamp
            //        double time_index = _Capture.GetCaptureProperty(Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_POS_MSEC);
            //        UpdateTextBox("Time: " + TimeSpan.FromMilliseconds(time_index).ToString(), Time_Label);
 
            //        //show frame number
            //        double framenumber = _Capture.GetCaptureProperty(Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_POS_FRAMES);
            //        UpdateTextBox("Frame: " + framenumber.ToString(), Frame_lbl);
 
            //        //update trackbar
            //        UpdateVideo_CNTRL(framenumber);
 
            //        /*Note: We can increase or decrease this delay to fastforward of slow down the display rate
            //         if we want a re-wind function we would have to use _Capture.SetCaptureProperty(Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_POS_FRAMES, FrameNumber*);
            //        //and call the process frame to update the picturebox ProcessFrame(null, null);. This is more complicated.*/
 
            //        //Wait to display correct framerate
            //        Thread.Sleep((int)(1000.0 / FrameRate)); //This may result in fast playback if the codec does not tell the truth
 
            //        //Lets check to see if we have reached the end of the video
            //        //If we have lets stop the capture and video as in pause button was pressed
            //        //and reset the video back to start
            //        if (framenumber == TotalFrames)
            //        {
            //            //pause button update
            //            play_pause_BTN_MouseUp(null, null);

            //            framenumber = 0;
            //            UpdateVideo_CNTRL(framenumber);
            //            _Capture.SetCaptureProperty(Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_POS_FRAMES, framenumber);
            //            //call the process frame to update the picturebox
            //            ProcessFrame(null, null);
            //        }
        }

        private void fileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            openFileDialog1.Title = "Load Video";
            openFileDialog1.Filter = "AVI| *.avi";
            if (openFileDialog1.ShowDialog()==DialogResult.OK)
            {

                //string fileName = openFileDialog1.FileName;
               string fileName = openFileDialog1.InitialDirectory + openFileDialog1.FileName;
               FileVideoSource source = new FileVideoSource(fileName);
               source.NewFrame += new NewFrameEventHandler(video_newFrame);
               source.Start();
               // PlayVideoFile(fileName);
            }
        }

        private void video_newFrame(object sender, NewFrameEventArgs evenArgs)
        {
            Bitmap bi = evenArgs.Frame;
         //   pictureBox1.Image = bi;
           // pictureBox1.Refresh();
        }

        private void streamToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //MessageBox.Show("hellow world", "title");

          // String url =  Microsoft.VisualBasic.Interaction.InputBox("Please enter the url for the video", "Enter URL");
            String url = ShowDialog("Please enter URL", "URL Input");

        }
        public  string ShowDialog(string text, string caption)
        {
            Form prompt = new Form();
            prompt.Width = 475;
            prompt.Height = 150;
            prompt.Text = caption;
            Label textLabel = new Label() { Left = 25, Top = 20, Text = text };

            TextBox inputBox = new TextBox() { Left = 25, Top = 50, Width = 400 };
            Button confirmation = new Button() { Text = "OK", Left = 325, Width = 100, Top = 80 };
            confirmation.Click += (sender, e) => { prompt.Close(); };
            prompt.Controls.Add(confirmation);
            prompt.Controls.Add(textLabel);
            prompt.Controls.Add(inputBox);
            prompt.ShowDialog();
            return inputBox.Text;
        }

        #region rect

     

        private void pictureBox1_Paint(object sender, PaintEventArgs e)
        {
            //using (Pen pen = new Pen(Color.Red, 2))
            //{
            //    e.Graphics.DrawRectangle(pen, mRect);
            //    e.Dispose();
            //}
            //Invalidate();
          

        }

        private void panAndZoomPictureBox1_Click(object sender, EventArgs e)
        {

        }

        System.Drawing.Graphics picboxGraphics;
        bool mDown = false;
        int mouseX;
        int mouseY;

        private void pictureBox1_MouseDown(object sender, MouseEventArgs e)
        {
            mDown = true;
            mouseX = e.X;
            mouseY = e.Y;
            mRect = new Rectangle(e.X, e.Y, 0, 0);
            this.Invalidate();
        }

        private void pictureBox1_MouseMove(object sender, MouseEventArgs e)
        {
            if (mDown == true)
            {
                this.Refresh();
                Pen drawPen = new Pen(Color.Red, 2);
                int width = e.X - mouseX, height = e.Y - mouseY;
                //Rectangle mRect;
               mRect = new Rectangle(mRect.Left, mRect.Top, Math.Min(e.X - mRect.Left, pictureBox1.ClientRectangle.Width - mRect.Left), Math.Min(e.Y - mRect.Top, pictureBox1.ClientRectangle.Height - mRect.Top));
                // mRect = new Rectangle(mouseX, mouseY, width * Math.Sign(width), height * Math.Sign(height));
                using (var graphics = (sender as Control).CreateGraphics())
                    graphics.DrawRectangle(drawPen, mRect);
                Invalidate();
                
            }
        }

        private void pictureBox1_MouseUp(object sender, MouseEventArgs e)
        {
            mDown = false;
        }

#endregion

        private void Form1_Load(object sender, EventArgs e)
        {

        }



        


      

      

        //private void Form1_MouseMove(object sender, MouseEventArgs e)
        //{
        //    if (e.Button == MouseButtons.Left)
        //    {
        //        // draws the rectangle as the mouse moves
        //        mRect = new Rectangle(mRect.Left, mRect.Top, e.X - mRect.Left, e.Y - mRect.Top);
        //    }
        //    this.Invalidate();
        //}

        //private void Form1_Paint(object sender, PaintEventArgs e)
        //{
        //    using (Pen pen = new Pen(Color.Red, 2))
        //    {
        //        e.Graphics.DrawRectangle(pen, mRect);
        //    }
        //}

        //private void Form1_MouseDown(object sender, MouseEventArgs e)
        //{
        //    mRect = new Rectangle(e.X, e.Y, 0, 0);
        //    this.Invalidate();
        //}
        
    }
}
