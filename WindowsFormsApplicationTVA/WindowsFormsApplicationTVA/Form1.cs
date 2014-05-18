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


namespace WindowsFormsApplicationTVA
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

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
                string fileName = openFileDialog1.FileName;
                PlayVideoFile(fileName);
            }
        }
        

        private void PlayVideoFile(string fileName)
        {
            string winName = "PlayVideoWin";
            CvInvoke.cvNamedWindow(winName);
            IntPtr capture = CvInvoke.cvCreateFileCapture(fileName);
            IntPtr iplImage;
            int fps = 0;//frames per second
            while (true)
            {
                iplImage = CvInvoke.cvQueryFrame(capture);
                //exit the loop after the end of play
                if (iplImage.Equals(IntPtr.Zero)) break;
                CvInvoke.cvShowImage(winName, iplImage);
                if (fps == 0)
                {
                    fps = (int)CvInvoke.cvGetCaptureProperty(capture, CAP_PROP.CV_CAP_PROP_FPS);
                }
                int c = CvInvoke.cvWaitKey(fps);
                // press "Esc" to exit the program
                if (c == 27) break;
            }
            //release the resource
            CvInvoke.cvReleaseCapture(ref capture);
            CvInvoke.cvDestroyWindow(winName);
        }
    }
}
