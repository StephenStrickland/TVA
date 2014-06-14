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
using AForge.Imaging;
using AForge.Imaging.Filters;
using AForge.Vision.Motion;
using AForge.Vision;
using AForge.Video.VFW;
using AForge.Video.DirectShow;
using AForge.Video.FFMPEG;



namespace WindowsFormsApplicationTVA
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            // mRect.Parent.Parent = pictureBox1;
            InitializeComponent();
            updateStatus("Welcome, to begin please open a video.");
            this.DoubleBuffered = true;
            Through = new Car();
            Turn = new Car();
            blb.FilterBlobs = true;
            blb.MinHeight = 5;
            blb.MinWidth = 5;
            blb.ObjectsOrder = ObjectsOrder.Size;
            simpleBg = new SimpleBackgroundModelingDetector(true, true);
            simpleBg.FramesPerBackgroundUpdate = 5;
            simpleBg.DifferenceThreshold = 10;

            blob = new BlobCountingObjectsProcessing(8, 8);
            motArea = new MotionAreaHighlighting(Color.Red);

            DET = new MotionDetector(simpleBg, blob);
            pictureBox1.SizeMode = PictureBoxSizeMode.Zoom;
            mRect = new Rectangle();
           

            //initListView();



        }
       

        private void initListView()
        {
            listView1.Columns.Add("Object", 20, HorizontalAlignment.Left);
        }
        MotionAreaHighlighting motArea;
        SimpleBackgroundModelingDetector simpleBg;
        BlobCountingObjectsProcessing blob;
        MotionDetector DET;
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
        Car Turn = new Car(), Through = new Car();
        AsyncVideoSource aSyncSource;
        //Image CurrentFrame;
        public event EventHandler Alarm;
        IFilter thresholdFilter = new Threshold(15);
        Difference differencefilter = new Difference();
        //Image PreviousFrame;
        //Image Bg;
        private IVideoSource vidSrouce = null;
        private IMotionDetector motionDet = null;
        private Bitmap lastFrame = null;
        private Bitmap currentFrame = null;
        private Bitmap backgroundFrame = null;
        public event EventHandler NewFrame;

        BlobCounter blb = new BlobCounter();



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



            cap = new Capture(fileName);

            Image<Bgr, byte> img = cap.QueryFrame();

            ImageViewer vi = new ImageViewer();
            vi.Image = img;
            vi.ShowDialog();

            Stopwatch SW;


        }

        private void fileToolStripMenuItem_Click(object sender, EventArgs e)
        {
                updateStatus("Loading Video");
            openFileDialog1.Title = "Load Video";
            openFileDialog1.Filter = "AVI| *.avi";
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                aSyncSource = null; ;
                //string fileName = openFileDialog1.FileName;
                string fileName = openFileDialog1.InitialDirectory + openFileDialog1.FileName;
                // FileVideoSource source = new FileVideoSource(fileName);
                // source.NewFrame += new NewFrameEventHandler(video_newFrame);
                // source.Start();
                // PlayVideoFile(fileName);
                FileVideoSource vidSrc = new FileVideoSource(fileName);
                AVIFileVideoSource aviSrc = new AVIFileVideoSource(fileName);

                VideoFileReader r = new VideoFileReader();
                r.Open(fileName);
                
                Bitmap img = r.ReadVideoFrame();
                pictureBox1.Image = img;
                aSyncSource = new AsyncVideoSource(vidSrc);
                aSyncSource.NewFrame += new NewFrameEventHandler(video_newFrame);
                aSyncSource.PlayingFinished += new PlayingFinishedEventHandler(video_close);
                setVideoProperties();
                updateStatus("Video Loaded");
                
                
               // aSyncSource.Start();



                // IVideoSource src = new FileVideoSource(fileName);
                //Detector dt = new Detector(src);
            }
            else
            {
                updateStatus("");
            }

        }

        private void setVideoProperties()
        {

        }

        //check for the elapsed time of the cars. If both have been tracked and the elapsed time is greater than 5 sec then it will create a new car for the oldest car(based off of start frame)
        public bool checkElapsed()
        {
            bool stillValid = false;
            int fps = 0;

            //if both have been at least started tracking
            if (Through.tracked && Turn.tracked)
            {
                int elapsedTime = Through.getTime(fps) - Turn.getTime(fps);
                if (Math.Abs(elapsedTime) > 5)
                {
                    stillValid = false;
                    if (Turn.start > Through.start)
                    {
                        Through = new Car();
                    }
                    else
                    {
                        Turn = new Car();
                    }
                }
                else
                    stillValid = true;

            }
            else
                stillValid = true;

            return stillValid;
        }

        public Rectangle[] groupRectangles(Rectangle[] rects)
        {
            Rectangle[] filtered = null;
            List<Rectangle> list = new List<Rectangle>();
            double heightThreshold = pictureBox1.Image.Height * .1;
            double widthThreshold = pictureBox1.Image.Width * .1;

            foreach (Rectangle r in rects)
            {
                for (int i = 0; i < rects.Length; i++)
                {
                    if (!rects[i].Equals(r))
                    {
                        if ((rects[i].IntersectsWith(r)) || ((getCenter(r).Y) - getCenter(rects[i]).Y <= heightThreshold) && (getCenter(r).X - getCenter(rects[i]).X <= widthThreshold))
                        {
                            rects[i] = new Rectangle();
                            list.Add(r);
                        }
                    }
                }
            }
            filtered = list.ToArray();
            return filtered;
        }

        public void Tracker(Rectangle[] rects)
        {
            /* Group Rectangles
             * Find New Rectangles - if new car then set as new Turn/Through
             * set new potitions - set currentRect, x, y, elapsed++
             * Check if done
             *  Then Analyze and create new cars
             * 
             */
            bool isThroughCurrent = currentCarThrough();
            Rectangle[] r = groupRectangles(rects);
            newRectangles(r);
            if (Turn.done && Through.done)
            {
                checkElapsed();
                //find new position of CurrentCar
                foreach (Rectangle rectangles in rects)
                {
                    isNextPosition(rectangles, isThroughCurrent);

                }
            }
            //find current car, if true then Through is current

         


            //if objects are not null
            //determine origin

            //else find next rect position for current object

            //update list
            var rectArr = blob.ObjectRectangles;
            if (null != rectArr)
            {

                updateListView();
                // listView1.Items.Add(it);
            }
        }

        private void newRectangles(System.Drawing.Rectangle[] r)
        {
         //   throw new NotImplementedException();
        }

        public bool currentCarThrough()
        {
            bool result = false;
            if (Through.tracked && !Through.done)
                result = true;

            return result;
        }

        public bool isNextPosition(Rectangle r, bool isTurn)
        {
            int threshold = 5;
            bool result = false;
            if (isTurn && Turn.currentRect.IntersectsWith(r))
            {
                result = true;
                Turn.currentRect = r;
                Turn.elapsedFrames++;
                if (Turn.currentRect.Left - r.Left <= threshold)
                {
                    Turn.elapsedFrames++;
                    Turn.done = true;
                }

            }
            else if (!isTurn && Through.currentRect.IntersectsWith(r))
            {
                result = true;
                Through.currentRect = r;
                Through.elapsedFrames++;
                if (Through.currentRect.Bottom - r.Bottom <= threshold)
                {
                    Through.elapsedFrames++;
                    Through.done = true;
                }
            }

            return result;
        }

        private void video_newFrame(object sender, NewFrameEventArgs evenArgs)
        {
            Bitmap bi = (Bitmap)evenArgs.Frame.Clone();
            
            
            // currentFrame = bi;
            if (DET.ProcessFrame(bi) > .02)
            {
                if (blob.ObjectsCount > 1)
                {
                    Tracker(blob.ObjectRectangles);
                }
                
            }
            pictureBox1.Image = bi;
            reDrawRect();

            bi.Dispose();
            

           


          //  pictureBox1.Image = bi;
            // pictureBox1.Refresh();
        }

        private void reDrawRect()
        {
             Pen drawPen = new Pen(Color.Red, 2);
           

        }

        private void video_close(object sender, ReasonToFinishPlaying evenArgs)
        {
            aSyncSource.SignalToStop();
            aSyncSource.WaitForStop();
            aSyncSource = null;

        }

        private void updateListView()
        {
            string car = "";
            if (currentCarThrough()) 
            {car = "Turn";}
            else { car = "Through"; }
            var it = new ListViewItem(new[] { car, aSyncSource.FramesProcessed.ToString(), "true", Through.X.ToString() });

            listView1.BeginInvoke(new MethodInvoker(() => listView1.Items.Add(it)));
        }

        private void streamToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //MessageBox.Show("hellow world", "title");

            // String url =  Microsoft.VisualBasic.Interaction.InputBox("Please enter the url for the video", "Enter URL");
            String url = ShowDialog("Please enter URL", "URL Input");

        }
        public string ShowDialog(string text, string caption)
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

        public void updateStatus(string txt)
        {
            toolStripStatusLabel1.Text = txt;
            statusStrip1.Refresh();
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


        private void button2_Click(object sender, EventArgs e)
        {
            try
            {
                if (!aSyncSource.IsRunning && !aSyncSource.Source.Equals(null) && !mRect.Location.IsEmpty)
                {
                    aSyncSource.Start();
                    updateStatus("Analyzing");
                }
                else if(mRect.Location.IsEmpty)
                {
                    MessageBox.Show("Please draw a Region of Interest", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                }
               
            }
            catch (NullReferenceException ex)
            {
                MessageBox.Show("Please load a video", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                
            }

        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {

        }
        public Point getCenter(Rectangle rect)
        {
            return new Point(rect.Left + rect.Width / 2, rect.Top + rect.Height / 2);
        }










    }
}
