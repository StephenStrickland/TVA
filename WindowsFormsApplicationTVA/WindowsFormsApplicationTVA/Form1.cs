﻿using System;
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
using System.Threading;



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
            //blb.FilterBlobs = true;
            //blb.MinHeight = 5;
            //blb.MinWidth = 5;
            //blb.ObjectsOrder = ObjectsOrder.Size;
            simpleBg = new SimpleBackgroundModelingDetector();
            simpleBg.SuppressNoise = true;
            //simpleBg.FramesPerBackgroundUpdate = 2;
            simpleBg.MillisecondsPerBackgroundUpdate = 50;
            simpleBg.DifferenceThreshold = 28;
            simpleBg.KeepObjectsEdges = true;
            blob = new BlobCountingObjectsProcessing(28, 28);
            
            motArea = new MotionAreaHighlighting(Color.Magenta);
            

            //CustomFrameDifferenceDetector custBG = new CustomFrameDifferenceDetector();
            //custBG.KeepObjectsEdges = true;
            //custBG.DifferenceThreshold = 20;

            DET = new MotionDetector(simpleBg, blob);
            pictureBox1.SizeMode = PictureBoxSizeMode.Zoom;
            mRect = new Rectangle();


            //  this.pbRectangle.BackColor = Color.Transparent;

            //this.pbRectangle.Image = transparentBitmap;

            this.SetStyle(ControlStyles.SupportsTransparentBackColor, true);
            this.pictureBox1.Controls.Add(this.pbRectangle);
            this.pictureBox1.BorderStyle = BorderStyle.FixedSingle;
            this.pbRectangle.BackColor = Color.Transparent;

            this.pbRectangle.Location = new Point(0, 0);
            this.pbRectangle.Size = pictureBox1.Size;



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
        Rectangle mRect = new Rectangle(), ThreshRect = new Rectangle(), FinishRect = new Rectangle();
        bool draw = false;
        public Car Turn = new Car();
        public Car Through = new Car();
        public Car CurrentCar = new Car();
        AsyncVideoSource aSyncSource;
        public int HThresh = 0, WThresh = 0, frames_Count = 0;
        public double FRAMERATE = 0;


        #region UI Events, sections

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            Form f = new Form();
            f.ShowDialog(this);
        }
        private void loadVideo(string fileName)
        {

            FileVideoSource vidSrc = new FileVideoSource(fileName);
            //set first frame
            Capture cp = new Capture(fileName);
            cp.Wait();

            Image<Bgr, Byte> img = cp.QueryFrame();
            Bitmap temp = new Bitmap((Bitmap)img.Bitmap.Clone());
            FRAMERATE = cp.GetCaptureProperty(CAP_PROP.CV_CAP_PROP_FPS);

            // temp =   

            pictureBox1.Image = temp;
            cp.Dispose();
            img.Dispose();
            setVideoProperties();
            // reDrawRect();
            aSyncSource = new AsyncVideoSource(vidSrc);
            aSyncSource.NewFrame += new NewFrameEventHandler(video_newFrame);
            aSyncSource.PlayingFinished += new PlayingFinishedEventHandler(video_close);
            //setVideoProperties();
            updateStatus("Video Loaded");
            //aSyncSource.Start();




        }

        private void fileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            updateStatus("Loading Video");
            openFileDialog1.Title = "Load Video";
            openFileDialog1.Filter = "AVI| *.avi";
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                aSyncSource = null;
                string fileName = openFileDialog1.InitialDirectory + openFileDialog1.FileName;
                loadVideo(fileName);
            }
            else
            {
                updateStatus("");
            }

        }
        //open URl, for future development | load a MJPEG stream
        private void streamToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //MessageBox.Show("hellow world", "title");

            // String url =  Microsoft.VisualBasic.Interaction.InputBox("Please enter the url for the video", "Enter URL");
            String url = ShowDialog("Please enter URL", "URL Input");

        }
        //analyze button
        private void button2_Click(object sender, EventArgs e)
        {
            try
            {
                if (!aSyncSource.IsRunning && !aSyncSource.Source.Equals(null) && !mRect.Location.IsEmpty)
                {
                    //Rectangle[] r = new Rectangle[1];
                    //r[0] = mRect;
                    //DET.MotionZones = r;
                    setRectangleProperties();
                    aSyncSource.Start();
                    updateStatus("Analyzing");
                }
                else if (mRect.Location.IsEmpty)
                {
                    MessageBox.Show("Please draw a Region of Interest", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                }

            }
            catch (NullReferenceException ex)
            {
                MessageBox.Show("Please load a video", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

            }

        }
        #endregion



        #region Video processing events

        private void video_newFrame(object sender, NewFrameEventArgs evenArgs)
        {

            Bitmap bi = (Bitmap)evenArgs.Frame.Clone();


            // currentFrame = bi;
            if (DET.ProcessFrame(bi) > .002)
            {
                if (blob.ObjectsCount > 0)
                {
                    Tracker(blob.ObjectRectangles);
                }

            }
            frames_Count = aSyncSource.FramesReceived;
            pictureBox1.Image = bi;
            
        }
        private void video_close(object sender, ReasonToFinishPlaying evenArgs)
        {
            aSyncSource.SignalToStop();
            aSyncSource.WaitForStop();
            aSyncSource = null;
            updateStatus("Video Finished");

        }
        #endregion
        //check for the elapsed time of the cars. If both have been tracked and the elapsed time is greater than 5 sec then it will create a new car for the oldest car(based off of start frame)
        #region Tracking




        public void Tracker(Rectangle[] rects)
        {
            /* Group Rectangles
             * validate that they are in the ROI
             * set New Rectangles - if new car then set as new Turn/Through
             * set new potitions - set currentRect, x, y, elapsed++
             * Check if done
             *  Then Analyze and create new cars
             * 
             */
            List<Rectangle> MasterList = rects.ToList();
            MasterList = groupRectangles(rects).ToList();
            MasterList = ValidateRects(MasterList);
            bool newCars = newRectangles(MasterList);
            if (!newCars)
            {

                //find new position of CurrentCar
                foreach (Rectangle rectangles in rects)
                {
                    isNextPosition(rectangles);

                }
            }
            //find current car, if true then Through is current
            isCurrentCarDone();
        }


        //need flag for if
        private bool newRectangles(List<Rectangle> r)
        {
            IList<Rectangle> keepers = new List<Rectangle>();
            bool result = false;
            if (r != null)
            {
                r = r.OrderByDescending(xx => xx.Height * xx.Width).ToList();
            }

            bool CurIsThrough = CurrentCar.isThrough;

            foreach (Rectangle rect in r)
            {
                if (!ThreshRect.Contains(rect) && mRect.IntersectsWith(rect) && !rect.IntersectsWith(CurrentCar.currentRect) && !CurrentCar.currentRect.Contains(rect) && !keepers.Contains(rect))
                {
                    CurrentCar = new Car()
                    {
                        currentRect = rect,
                        position = getCenter(rect),
                        done = false,
                        elapsedFrames = 1,
                        isThrough = false,
                        tracked = true
                    };
                    switch (isToporBottom(rect))
                    {
                        //Turn | Bottom
                        case true:
                            CurrentCar.isThrough = false;
                            break;

                        //Through | Top
                        case false:
                            CurrentCar.isThrough = true;
                            break;
                    }
                    keepers.Add(rect);
                    AddListView(CurrentCar);
                    result = true;
                }
            }


            return result;
        }

        private void AddListView(Car cc)
        {
            ListViewItem it = new ListViewItem(listView1.Items.Count.ToString());
            it.SubItems.Add(cc.elapsedFrames.ToString());
            it.SubItems.Add(cc.tracked.ToString());
            it.SubItems.Add(cc.isValidData.ToString());
            it.SubItems.Add(cc.done.ToString());



            listView1.BeginInvoke(new MethodInvoker(() => listView1.Items.Add(it)));
            //format and add this car to the list with an ID. 1,2,3,4,...
        }
        private bool isToporBottom(Rectangle r)
        {
            //true for Turn/Bottom
            bool result = true;

            Point p = getCenter(r);
            Point m = getCenter(mRect);
            if(p.Y >= m.Y)
            {
                result = false;
            }

            return result;
        }

        public void isCurrentCarDone()
        {
            if (CurrentCar.done)
            {

                checkElapsed();
                if (CurrentCar.isThrough)
                {
                    Through = CurrentCar;

                }
                else
                {
                    Turn = CurrentCar;
                }
                updateListView();
                CurrentCar = new Car();
            }

            //if (Through.tracked && !Through.done)
            //    result = true;

            //return result;
        }
        public List<Rectangle> ValidateRects(List<Rectangle> temp)
        {
            List<Rectangle> ValidatedList = new List<Rectangle>();
            foreach (Rectangle r in temp)
            {
                if (mRect.Contains(r) || mRect.IntersectsWith(r))
                {
                    ValidatedList.Add(r);
                }
            }

            return ValidatedList;
        }
        public bool isFinishing(Car cc)
        {
            bool finished = false;

            if (cc.currentRect.IntersectsWith(mRect) || cc.currentRect.IntersectsWith(FinishRect))
            {
                finished = true;
            }
            return finished;
        }
        public void assignCar()
        {
            CurrentCar.done = true;
            if (CurrentCar.isThrough)
            {
                Through = CurrentCar;
            }
            else
            {
                Turn = CurrentCar;
            }
            CurrentCar = new Car();
        }

        public bool isNextPosition(Rectangle r)
        {
            int threshold = 5;
            bool result = false;
            if (CurrentCar.currentRect.IntersectsWith(r) && !CurrentCar.done)
            {
                result = true;
                Turn.currentRect = r;
                Turn.elapsedFrames++;
                if (isFinishing(CurrentCar))
                {
                    CurrentCar.elapsedFrames++;
                    assignCar();
                }

                Thread th = new Thread(new ThreadStart(updateListView));
                th.Start();
                //updateListView();

            }
            //if (!CurrentCar.isThrough && Curr.currentRect.IntersectsWith(r))
            //{
            //    result = true;
            //    Turn.currentRect = r;
            //    Turn.elapsedFrames++;
            //    if (Turn.currentRect.Left - r.Left <= threshold)
            //    {
            //        Turn.elapsedFrames++;
            //        Turn.done = true;
            //    }

            //}
            //else if (CurrentCar.isThrough && Through.currentRect.IntersectsWith(r))
            //{
            //    result = true;
            //    Through.currentRect = r;
            //    Through.elapsedFrames++;
            //    if (Through.currentRect.Bottom - r.Bottom <= threshold)
            //    {
            //        Through.elapsedFrames++;
            //        Through.done = true;
            //    }
            //}

            return result;
        }
        public bool checkElapsed()
        {
            //if valid then set isValidData
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
            List<Rectangle> RectList = new List<Rectangle>();
            RectList = rects.ToList();
            List<Rectangle> list = new List<Rectangle>();
            double heightThreshold = HThresh * .1;
            double widthThreshold = WThresh * .1;
            RectList = new List<Rectangle>(RectList.Distinct<Rectangle>());
            if (RectList.Count > 1)
            {
                for (int j = 0; j < RectList.Count; j++)
                {
                    for (int i = 0; i < RectList.Count; i++)
                    {
                        if (!RectList[i].Equals(j) && !RectList[i].IntersectsWith(RectList[j]) && !RectList[i].Contains(RectList[j]) && !list.Contains(RectList[j]))
                        {
                            list.Add(RectList[j]);                            //rects[i] = new Rectangle   
                        }
                    }
                }
            }
            else
            {
                list.Add(RectList[0]);
            }
            filtered = list.ToArray();
            return filtered;
        }
        #endregion


        #region Updating UI Rect | ListViews
        public void updateStatus(string txt)
        {
            toolStripStatusLabel1.Text = txt;
            statusStrip1.Refresh();
        }



        private void updateListView()
        {

            if(InvokeRequired)
            {
                MethodInvoker method = new MethodInvoker(updateListView);
                Invoke(method);
                return;
            }
            listView1.BeginUpdate();
            listView1.Items[listView1.Items.Count-1].SubItems[1].Text = CurrentCar.elapsedFrames.ToString();
            listView1.Items[listView1.Items.Count-1].SubItems[2].Text = CurrentCar.done.ToString();
            listView1.Items[listView1.Items.Count-1].SubItems[3].Text = CurrentCar.isValidData.ToString();
            listView1.Items[listView1.Items.Count-1].SubItems[3].Text = CurrentCar.done.ToString();
            listView1.EndUpdate();
            listView1.Refresh();

            //ListViewItem cc = new ListViewItem(listView1.Items.Count.ToString());
            //cc.SubItems.Add("SubItem1a");
            //cc.SubItems.Add("SubItem1b");
            //cc.SubItems.Add("SubItem1c");


            //string car = "";

            //car = "Turn";
            // car = "Through"; 
            //var it = new ListViewItem(new[] { car, aSyncSource.FramesProcessed.ToString(), "true", Through.position.X.ToString() });

            // listView1.BeginInvoke(new MethodInvoker(() => listView1.Items.Add(it)));
        }
        #endregion

        #region Utilites | Dialogs
        private void setVideoProperties()
        {
            HThresh = pictureBox1.Image.Height;
            WThresh = pictureBox1.Image.Height;

        }
        public Point getCenter(Rectangle rect)
        {
            return new Point(rect.Left + rect.Width / 2, rect.Top + rect.Height / 2);
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

        public void setRectangleProperties()
        {
            double defH = -1 * (.25 * mRect.Height);
            double defW = -1 * (.25 * mRect.Width);
            ThreshRect = Rectangle.Inflate(mRect, (int)defW, (int)defH);
            FinishRect = Rectangle.Inflate(mRect, (-1 * (mRect.Width - 5)), (-1 * (mRect.Width - 5)));
        }
        #endregion


        #region PictureBox Events | Drawing Rectangle



        private void pictureBox1_Paint(object sender, PaintEventArgs e)
        {
            // reDrawRect();


        }

        bool mDown = false;
        int mouseX;
        int mouseY;

        private void pictureBox1_MouseDown(object sender, MouseEventArgs e)
        {
            if (!aSyncSource.IsRunning)
            {
                mDown = true;
                mouseX = e.X;
                mouseY = e.Y;
                mRect = new Rectangle(e.X, e.Y, 0, 0);
                this.Invalidate();
            }
        }

        static object lockit = new object();

        private void pictureBox1_MouseMove(object sender, MouseEventArgs e)
        {
            if (mDown == true && !aSyncSource.IsRunning)
            {
                lock (lockit)
                {
                    this.Refresh();
                    //     pbRectangle.Invalidate();
                    Pen drawPen = new Pen(Color.GreenYellow, 2);
                    int width = e.X - mouseX, height = e.Y - mouseY;
                    //Rectangle mRect;
                    mRect = new Rectangle(mRect.Left, mRect.Top, Math.Min(e.X - mRect.Left, pbRectangle.ClientRectangle.Width - mRect.Left), Math.Min(e.Y - mRect.Top, pbRectangle.ClientRectangle.Height - mRect.Top));
                    // mRect = new Rectangle(mouseX, mouseY, width * Math.Sign(width), height * Math.Sign(height));
                    using (var graphics = (sender as Control).CreateGraphics())
                        graphics.DrawRectangle(drawPen, mRect);
                }
                //   Invalidate();

            }
        }

        private void pictureBox1_MouseUp(object sender, MouseEventArgs e)
        {
            mDown = false;
        }

        #endregion


        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {

        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // var transparentBitmap = new Bitmap(pbRectangle.Width, pbRectangle.Height);
            // transparentBitmap.MakeTransparent();



        }


    }
}
