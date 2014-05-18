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


namespace WindowsFormsApplicationTVA
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            this.DoubleBuffered = true;
        }
        Rectangle mRect;


        //Initiate rectangle with mouse down event
        protected override void OnMouseDown(MouseEventArgs e)
        {
            mRect = new Rectangle(e.X, e.Y, 0, 0);
            this.Invalidate();
        }

        //check if mouse is down and being draged, then draw rectangle
        protected override void OnMouseMove(MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                mRect = new Rectangle(mRect.Left, mRect.Top, e.X - mRect.Left, e.Y - mRect.Top);
                this.Invalidate();
            }
        }

        //draw the rectangle on paint event
        protected override void OnPaint(PaintEventArgs e)
        {
            //Draw a rectangle with 2pixel wide line
            using (Pen pen = new Pen(Color.Red, 2))
            {
                e.Graphics.DrawRectangle(pen, mRect);
            }

        }

        //drawing cs = new drawing() { ev = 2, 34 };


        private void textBox2_TextChanged(object sender, EventArgs e)
        {

        }

        private void splitContainer2_Panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void label5_Click(object sender, EventArgs e)
        {

        }

        private void label4_Click(object sender, EventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

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
            openFileDialog1.Title = "Load Video";
            if (openFileDialog1.ShowDialog()==DialogResult.OK)
            {

                //string fileName = openFileDialog1.FileName;
               string fileName = System.IO.Path.GetDirectoryName(openFileDialog1.FileName);
                PlayVideoFile(fileName);
            }
        }
        

        private void PlayVideoFile(string fileName)
        {
            string test = "hello world";
            Capture cap = new Capture(fileName);
            Image<Bgr, byte> img = cap.QueryFrame();

            ImageViewer vi = new ImageViewer();
            vi.Image = img;
            vi.ShowDialog();
            //string winName = "PlayVideoWin";
            //CvInvoke.cvNamedWindow(winName);
            //IntPtr capture = CvInvoke.cvCreateFileCapture(fileName);
            //IntPtr iplImage;
            //int fps = 0;//frames per second
            //while (true)
            //{
            //    iplImage = CvInvoke.cvQueryFrame(capture);
            //    //exit the loop after the end of play
            //    if (iplImage.Equals(IntPtr.Zero)) break;
            //    CvInvoke.cvShowImage(winName, iplImage);
            //    if (fps == 0)
            //    {
            //        fps = (int)CvInvoke.cvGetCaptureProperty(capture, CAP_PROP.CV_CAP_PROP_FPS);
            //    }
            //    int c = CvInvoke.cvWaitKey(fps);
            //    // press "Esc" to exit the program
            //    if (c == 27) break;
            //}
            ////release the resource
            //CvInvoke.cvReleaseCapture(ref capture);
            //CvInvoke.cvDestroyWindow(winName);
        }
    }
}
