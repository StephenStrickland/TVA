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
using System.Drawing.Drawing2D;
using System.Runtime.InteropServices;



namespace WindowsFormsApplicationTVA
{
    public partial class Form1 : Form
    {

        //This allows us to have a console for debugging. If you need to see something call Console.WriteLine();
        public class Win32
        {
            [DllImport("kernel32.dll")]
            public static extern Boolean AllocConsole();
            [DllImport("kernel32.dll")]
            public static extern Boolean FreeConsole();
        }
        

        public Form1()
        {
            CSV = new StringBuilder();
            CarMaster = new List<Car>();
            //start our console
            Win32.AllocConsole();
            // mRect.Parent.Parent = pictureBox1;
            InitializeComponent();
            updateStatus("Welcome, to begin please open a video.");
            this.DoubleBuffered = true;
            Through = new Car();
            Turn = new Car();
            //setup our motion detector and processor
            simpleBg = new SimpleBackgroundModelingDetector();
            simpleBg.SuppressNoise = true;
            simpleBg.MillisecondsPerBackgroundUpdate = 0;
            simpleBg.FramesPerBackgroundUpdate = 10;
            simpleBg.DifferenceThreshold = 10;
            simpleBg.KeepObjectsEdges = true;
            blob = new BlobCountingObjectsProcessing(15, 15);
            blob.HighlightMotionRegions = true;
           // blob.HighlightColor = Color.Yellow;

            //motion area highlighter, FOR DEBUGGING  - allows us to see the actual difference of pixels.
            motArea = new MotionAreaHighlighting(Color.Magenta);

            //our main detector. We give it the background modeler and the blob counter.
            DET = new MotionDetector(simpleBg, blob);

            //scales our video to the size of the picturebox instead of display the original size of video, has no effect on the actual motino detection.
            pictureBox1.SizeMode = PictureBoxSizeMode.Zoom;
            //original ROI
            mRect = new Rectangle();


            //We have two pictureboxes the first we use to display the video, Then on pbRectangle we draw our polygon / ROI
            this.SetStyle(ControlStyles.SupportsTransparentBackColor, true);
            this.pictureBox1.Controls.Add(this.pbRectangle);
            this.pictureBox1.BorderStyle = BorderStyle.FixedSingle;
            this.pbRectangle.BackColor = Color.Transparent;

            this.pbRectangle.Location = new Point(0, 0);
            this.pbRectangle.Size = pictureBox1.Size;


            //this section is for debugging. it draws a standard polygon.
            var lst = new  List<Point>();
            lst.Add(new Point(179, 207));
            lst.Add(new Point(235,232));
            lst.Add(new Point(304, 216));
            lst.Add(new Point(244, 193));

            Polygons.Add(lst);
            //give our poly the vertices, then set them.
            poly.Corners = lst;
            poly.setPoints(pictureBox1);
            

        }

        #region System Variables and Objects
        //our master list of cars for the entire video
        public virtual List<Car> CarMaster { get; set;  }
        //we add to this stringbuilder so that when we finish processing we can output to a csv file, incase the user wants to process the info better.
        public StringBuilder CSV;


        private void initListView()
        {
            listView1.Columns.Add("Object", 20, HorizontalAlignment.Left);
        }
        //motion handlers(detectors, processors)
        MotionAreaHighlighting motArea;
        SimpleBackgroundModelingDetector simpleBg;
        BlobCountingObjectsProcessing blob;
        MotionDetector DET;
        //original use of ROI
        Rectangle mRect = new Rectangle(), ThreshRect = new Rectangle(), FinishRect = new Rectangle();
        bool draw = false;
        //old way of asigning cars, only allows one in the ROI at a time
        public Car Turn = new Car();
        public Car Through = new Car();
        public Car CurrentCar = new Car();
        //aSync handles the 
        AsyncVideoSource aSyncSource;
        public int HThresh = 0, WThresh = 0, frames_Count = 0;
        public double FRAMERATE = 0, TOTALFRAMES = 0;
        //we set to true if the video is running
        public bool isRunning = false;
        //our polygon class
        public Polygon poly = new Polygon();

        #endregion

        #region UI Events, sections

        //save btn
        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            SaveFileDialog sv = new SaveFileDialog();
            sv.Filter = "txt files (*.csv)|*.csv|All files (*.*)|*.*";
            sv.FilterIndex = 2;
            sv.RestoreDirectory = true;
            sv.ShowDialog();
            //Form f = new Form();
            //f.ShowDialog(this);
            //sv.FileOk += save_CSV();
            if (sv.ShowDialog() == DialogResult.OK)
            {
                ///save and genereate the csv file to that location
            }
        }
        //load a video, but does not start it
        private void loadVideo(string fileName)
        {
            Console.WriteLine("Video loaded");
            FileVideoSource vidSrc = new FileVideoSource(fileName);
           
            
            //set first frame
            Capture cp = new Capture(fileName);
            cp.Wait();

            Image<Bgr, Byte> img = cp.QueryFrame();
            Bitmap temp = new Bitmap((Bitmap)img.Bitmap.Clone());
            FRAMERATE = cp.GetCaptureProperty(CAP_PROP.CV_CAP_PROP_FPS);
            TOTALFRAMES = cp.GetCaptureProperty(CAP_PROP.CV_CAP_PROP_FRAME_COUNT);
            pictureBox1.Image = temp;
            cp.Dispose();
            img.Dispose();
            setVideoProperties();
           
            aSyncSource = new AsyncVideoSource(vidSrc);
           
            aSyncSource.SkipFramesIfBusy = false;
            aSyncSource.NewFrame += new NewFrameEventHandler(video_newFrame);
            aSyncSource.PlayingFinished += new PlayingFinishedEventHandler(video_close);
          
            updateStatus("Video Loaded");

        }
        //load btn
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

        //For Future Development, using a webcam. just pull in th emjpeg stream and you are practically set.
        private void streamToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //MessageBox.Show("hellow world", "title");

            // String url =  Microsoft.VisualBasic.Interaction.InputBox("Please enter the url for the video", "Enter URL");
            String url = ShowDialog("Please enter URL", "URL Input");

        }
        //scale for translating the blobs rectangles to our polygon's scale.

        //since the video is a different resolution then the picturebox. 
        //The blob detector returns rectangles at the scale of the video resolution not the picturebox so we need to translate them to our picbox
        double scaleX = 0;
        double scaleY = 0;

        //analyze button
        private void button2_Click(object sender, EventArgs e)
        {
            try
            {
                if (!isRunning && !aSyncSource.Source.Equals(null) && poly.Corners.Count >3)
                {
                   // Point p = pictureBox1.PointToClient(new Point(pictureBox1.Image.Size.Width, pictureBox1.Image.Size.Height));
                    //find the scale
                    scaleX = pictureBox1.Image.Size.Width / (1.0 * pictureBox1.Width);
                    scaleY = pictureBox1.Image.Size.Height / (1.0 * pictureBox1.Height);

                    isRunning = true;
                   //start video
                    aSyncSource.Start();
                    updateStatus("Analyzing");
                }
                else if (poly.Corners.Count <= 3)
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

        static object objLock = new object();

        private void video_newFrame(object sender, NewFrameEventArgs evenArgs)
        {

            lock (objLock)
            {
                Bitmap bi = (Bitmap)evenArgs.Frame.Clone();
                double motion = 0.0;

                // currentFrame = bi;
                if ((motion = DET.ProcessFrame(bi)) < 1)
                {
                    if (blob.ObjectsCount > 0)
                    {
                        Tracker(blob.ObjectRectangles);
                    }

                }

                frames_Count++;
                Console.WriteLine("Frame Count: " + frames_Count);
                pictureBox1.Image = bi;
            }
            
        }
        private void video_close(object sender, ReasonToFinishPlaying evenArgs)
        {
            
            updateStatus("Video Finished");
            aSyncSource.SignalToStop();
            aSyncSource.WaitForStop();
            aSyncSource = null;

        }
        #endregion
        //check for the elapsed time of the cars. If both have been tracked and the elapsed time is greater than 5 sec then it will create a new car for the oldest car(based off of start frame)
        #region Tracking
        /* === New Way ===
         * for our list of rects, 
         * 
         * Validate, see if they are touching/contained by poly
         * 
         * create a temporay list of cars from CarMaster that are NOT DONE
         * 
         * then cycle throught the list of rectangles, testing each car if that rect intersects with the last location.
         * 
         *  if it intersects then add arr to the cars history, ++ elapsed frames, and break out of that Rectangle.
         *  
         * for rectangles that are not set to a car, we add them to th enew list if it is valid data(not another lane coming in from the sides.)
         * 
         * Then modify our current list of cars that are NOT DONE to see if they are in bounds (Check in Bounds)
         * 
         * Then check for New Cars - see notes for this funciton
         * 
         * add our new cars to our Car Master
         * 
         * Then add new cars to listview
         * 
         */

        public void Tracker(Rectangle[] rects)
        {
            Console.WriteLine("******* NUmber of Rects: " + rects.Length.ToString());
            List<Rectangle> MasterList = rects.ToList();
            MasterList = ValidateRects(MasterList);
            if (MasterList.Count > 0)
            {
                List<Car> NewCars = new List<Car>();
                List<Car> CarListTemp = new List<Car>();;

                if (CarMaster.Count > 0)
                    CarListTemp = CarMaster.Where ( x=> !x.done).ToList();
                

                foreach (var r in MasterList)
                {
                   foreach ( var c in CarListTemp )
                   {
                       if (c.History[0].IntersectsWith(r))
                       {
                           c.History.Insert(0, r);
                           c.elapsedFrames++;
                           //goto next rect
                           break;

                       }
                   }
                    if(isValidCar(r))
                   NewCars.Add(new Car { currentRect = r, start = frames_Count, done = false, elapsedFrames = 1, id = CarMaster.Count });


                }

                CheckInBounds(CarListTemp);
                CheckNewCars(NewCars);

                CarMaster.AddRange(NewCars);

                //This is very messy, FFD find a new way to do this, this was just a quick and dirty way of adding cars to the top of the listview.
                foreach ( var cc in NewCars)
                AddListView(cc);
                checkFinished();
                // update listview
                //add listview NewCars

            }
            Console.WriteLine(Environment.NewLine + "\r\n End of Tracker" );
           
        }
        //adds a car to CSV
        public void addCSV(Car c)
        {
            CSV.Append(c.id +  ",");
            CSV.Append(c.elapsedFrames + ",");
            CSV.Append(c.getTime(30) + Environment.NewLine);
            

        }

        //simple math to see if a point is left of two point(p, s)
        bool isPointLeftOfRay(Point target, Point p, Point s)
        {
            double raySx = s.X;
            double raySy = s.Y;
            double rayEx = p.X;
            double rayEy = p.Y;
            double x = target.X;
            double y = target.X;


            return (y - raySy) * (rayEx - raySx)
            > (x - raySx) * (rayEy - raySy);
        }

        //attempt to test to make sure that the cars center is not coming in from the sides
        //making sure that new cars are coming in from the top and the bottom
        private bool isValidCar(Rectangle r)
        {
            Point right = new Point(r.X + r.Width, r.Y);
            Point left = r.Location;
            Point target = getCenter(r);
            bool result = false;
            if(target.X <= poly.Bottom.X || target.X <= poly.Left.X)
                result = (isPointLeftOfRay(target, poly.Left, poly.Bottom) && !isPointLeftOfRay(target, poly.Top, poly.Right));
            else if (target.X >= poly.Right.X || target.X >= poly.Top.X)
            {
                result = (!isPointLeftOfRay(target, poly.Left, poly.Bottom) && isPointLeftOfRay(target, poly.Top, poly.Right));
            }

            return result;
        }

          //checks if car is Through or turn
        private void CheckNewCars(List<Car> NewCars)
        {
           //this should not be the way of doing things.
            /*
             * First check to see of any lines of the rect are touching the lines of poly
             * 
             * If so set car tracked to "true"
             * 
             * This means that in order for a car to be add to our master list, it need to be touching a line of the polygon
             * 
             * This prevents new rectangles that pop up in the ROI with out a car from being added to our CarMaster
             * 
             * Then when you addRange to car master, change it to 
             * 
             * foreach car in Newcars
             * 
             *  if(car.tracked)
             *  CarMaster.Add(car)
             *  
             * it iterates through our nes cars and only adds the cars that are valid.
             */
            foreach(var c in NewCars)
            {
                c.isThrough = isTop(c.History[0]);

            }

        }

        //modifies list to see if car is in poly, if not then set to done.
        private void CheckInBounds(List<Car> CarListTemp)
        {
            //modifies list to see of thcar is inbounds
            /*
             * it choses the midpoint on the front of the car, the point is decided by if the car is through or turn.
             * 
             * if turn the it chooses the midpoint of the top of the rect, through - the bottom
             * 
             * if that point is no longer contained within the polygon then it sets the car to DONE
             */

            foreach(var c in CarListTemp)
            {
                var tempRect = c.History[0];
                Point left = tempRect.Location;
                Point top = new Point(tempRect.X + tempRect.Width, tempRect.Y);
                Point right = new Point(tempRect.X + tempRect.Width, tempRect.Y + tempRect.Height);
                Point bottom = new Point(tempRect.X, tempRect.Height + tempRect.Y);
               switch(c.isThrough)
               {
                   case true:

                       if(!poly.ContainsPoint(poly.midPoint(bottom, right)))
                       {
                           c.done = true;
                       }

                       break;

                   case false:

                       if (!poly.ContainsPoint(poly.midPoint(left, top)))
                       {
                           c.done = true;
                       }

                       break;
               }
              
            }


        }

        //updates listview, 
        //FFD - checks elapsed, adds analysis 
        private void checkFinished()
        {
            Thread th = new Thread(new ThreadStart(updateListView));
            th.Start();



        }

        //shade for alternating listview items
        Color shaded = Color.FromArgb(149, 165, 166);

        private void AddListView(Car cc)
        {

           //create a listview item
            ListViewItem it = new ListViewItem(listView1.Items.Count.ToString());
            //give it a tag
            it.Tag = cc;
            //add the subitems
            it.SubItems.Add(cc.elapsedFrames.ToString());
            //FFD this should be c.history[0].gettime(Frames Per Second)
            it.SubItems.Add(cc.tracked.ToString());
            //Then show if Through or Turn
            it.SubItems.Add(cc.isValidData.ToString());
            //not really needed
            it.SubItems.Add(cc.done.ToString());

            if(listView1.Items.Count % 2 == 1)
            {
                it.BackColor = shaded;
                it.UseItemStyleForSubItems = true;
            }

           

            listView1.BeginInvoke(new MethodInvoker(() => listView1.Items.Insert(0,it)));
            //format and add this car to the list with an ID. 1,2,3,4,...
        }

        //checks if rectangle is in the top of the poly, this is a bad way of doing this.
        private bool isTop(Rectangle r)
        {
            //true for TOP / Through
           //split the poly into half(including the slope, not just a horizontal split) and see if our rect is inside
            Point[] rects = new Point[] {poly.midPoint(poly.Left, poly.Bottom), poly.Left, poly.Top, poly.midPoint(poly.Top, poly.Right)  };
            return poly.partialPolygonContainsRect(rects, r);
        }

        public List<Rectangle> ValidateRects(List<Rectangle> temp)
        {

            // these are points that are areas off to the sides of the polygon - areas that would contain traffic coming from the other side.
            Point[] leftExclude = new Point[] {
                poly.Left,
                poly.Bottom,
                new Point(poly.Left.X - 200, poly.Left.Y),
                new Point(poly.Bottom.X - 200, poly.Bottom.Y)
            
            };

            Point[] rightExclude = new Point[] {
                poly.Left,
                poly.Bottom,
                new Point(poly.Top.X - 200, poly.Top.Y),
                new Point(poly.Right.X - 200, poly.Right.Y)
            
            };
            List<Rectangle> ValidatedList = new List<Rectangle>();
            foreach (Rectangle rr in temp)
            {
               // var r = pictureBox1.RectangleToClient(rr);
                var r = rr;
                r.X = (int)(rr.X / scaleX);
                r.Y = (int)(rr.Y / scaleY);

                r.Width = (int)(rr.Width / scaleX);
                r.Height = (int)(rr.Height / scaleY);
                // && (!poly.partialPolygonContainsPoint(leftExclude, getCenter(r))) && (!poly.partialPolygonContainsPoint(rightExclude, getCenter(r)))
                if (poly.ContainsRect(r) ) //|| r.IntersectsWith(CurrentCar.currentRect)))
                {
                    ValidatedList.Add(r);
                }
                else
                {
                    int zzzz = 0;
                }
            }
            Console.WriteLine("Validate rects: " + ValidatedList.Count.ToString());
             return ValidatedList;
        }


            /* === OLD WAY OF DOING THINGS ===
             * 
             * Group Rectangles
             * validate that they are in the ROI
             * set New Rectangles - if new car then set as new Turn/Through
             * set new positions - set currentRect, x, y, elapsed++
             * Check if done
             *  Assign car
             *  Analyze and create new cars
             * 
             */
        public Rectangle[] groupRectangles(Rectangle[] rects)
        {
            Rectangle[] filtered = null;
            List<Rectangle> RectList = new List<Rectangle>();
            List<Rectangle> dupes = new List<Rectangle>();
            RectList = rects.ToList();
            List<Rectangle> list = new List<Rectangle>();
            double heightThreshold = HThresh * .1;
            double widthThreshold = WThresh * .1;
            RectList = new List<Rectangle>(RectList.Distinct<Rectangle>());
            if (RectList.Count > 1)
            {
                foreach(Rectangle rect in RectList)
                {
                    for (int i = 0; i < RectList.Count; i++)
                    {
                        if (!RectList[i].Equals(rect) && !RectList[i].Contains(rect) && !list.Contains(rect) )
                        {

                            // && !RectList[i].IntersectsWith(RectList[j])
                            list.Add(rect);                            //rects[i] = new Rectangle   
                        }
                        else if (RectList[i].IntersectsWith(rect) && !RectList[i].Equals(rect))
                        {
                            double iArea = RectList[i].Width * RectList[i].Height;
                            double jArea = rect.Width * rect.Height;
                            if(iArea >= jArea)
                            {
                                list.Add(RectList[i]);
                                dupes.Add(rect);
                            }
                            else
                            {
                                list.Add(rect);
                                dupes.Add(RectList[i]);
                            }

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
        public bool isNextPositionAndUpdate(Rectangle r)
        {
            //Point left = r.Location;
            //Point top = new Point(r.X+r.Width, r.Y);
            //Point right = new Point(r.X+r.Width, r.Y + r.Height);
            //Point bottom = new Point(r.X, r.Height);
            
            //bool result = false;
            //Point p;
            //if(CurrentCar.isThrough)
            //{
            //    p = poly.midPoint(bottom, right);
            //    if (CurrentCar.History[0].IntersectsWith(r) && !CurrentCar.done && poly.ContainsPoint(p))
            //    {

            //        result = true;
            //        CurrentCar.currentRect = r;
            //        CurrentCar.elapsedFrames++;

            //        //updateListView();

            //    }
            //}
            //else
            //{
            //    p = poly.midPoint(left, top);
            //    if (CurrentCar.currentRect.IntersectsWith(r) && !CurrentCar.done && poly.ContainsPoint(p))
            //    {

            //        result = true;
            //        CurrentCar.currentRect = r;
            //        CurrentCar.elapsedFrames++;

            //        //updateListView();

            //    }
            //}

            //if (CurrentCar.currentRect.IntersectsWith(r) && !CurrentCar.done && poly.ContainsPoint(p))
            //{
               
            //    result = true;
            //    CurrentCar.currentRect = r;
            //    CurrentCar.elapsedFrames++;
                
            //    //updateListView();

            //}
            //else if (CurrentCar.currentRect.IntersectsWith(r) && !CurrentCar.done && !poly.ContainsPoint(p))
            //{

            //    CurrentCar.done = true;
            //    CurrentCar.elapsedFrames++;
            //    assignCar();
            //    result = false;
            //    //finished
            //}
            //else if (CurrentCar.done)
            //{
                
            //}
            //if(result)
            //{
            //    Console.WriteLine(r.ToString() + "new position");
            //}
            //else
            //{
            //    Console.Write("Rect did not match");
            //}
            
            return true;
        }
        //need flag for if
        private bool newRectangles(List<Rectangle> r)
        {
            return false;

            ////IList<Rectangle> keepers = new List<Rectangle>();
            //bool result = false;
            //if (r != null)
            //{
            //   // r = r.OrderByDescending(xx => xx.Height * xx.Width).ToList();
            //}
            //List<Rectangle> keepers = new List<Rectangle>();
            //bool CurIsThrough = CurrentCar.isThrough;

            //foreach (Rectangle rect in r)
            //{
            //    if (!rect.IntersectsWith(Rectangle.Inflate(CurrentCar.currentRect, 20, 20)) && !CurrentCar.currentRect.Contains(rect) && !keepers.Contains(rect))
            //    {
            //        bool isTop = isToporBottom(rect);

            //        if (isTop && CurrentCar.isThrough)
            //        {
            //            if (listView1.Items.Count > 1)
            //            {
            //                CurrentCar.isValidData = false;
            //                //Thread th = new Thread(new ThreadStart(updateListView));
            //                //th.Start();
            //                //assignCar();
            //            }

            //        }
            //        else if (!isTop)
            //        {

            //        }
            //        CurrentCar = new Car()
            //        {
                     
            //            currentRect = rect,
            //            position = getCenter(rect),
            //            done = false,
            //            elapsedFrames = 1,
            //            isThrough = false,
            //            tracked = false
            //        };
            //        switch (isTop)
            //        {
            //            //Turn | Bottom
            //            case false:
            //                CurrentCar.isThrough = false;
            //                break;

            //            //Through | Top
            //            case true:
            //                CurrentCar.isThrough = true;
            //                break;
            //        }
            //        keepers.Add(rect);
            //        AddListView(CurrentCar);
            //        result = true;
            //        Console.WriteLine("new Car    " + rect.ToString());
            //    }
            //}


            //return result;
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
        public void assignCar()
        {
            
            if (CurrentCar.isThrough)
            {
                Through = CurrentCar;
            }
            else
            {
                Turn = CurrentCar;
            }
            Console.WriteLine(Environment.NewLine + "---- Assigned New Car ---- " + Environment.NewLine);
            updateListView();
            //AnalyzeCars();
            CurrentCar = new Car();
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

            var carTemp = new List<Car>();
            carTemp.AddRange(CarMaster) ;

            foreach (var c in carTemp)
            {

             //   var vv = listView1.Items.IndexOfKey(c.id.ToString());

                var vv = listView1.Items.Count - c.id -1;
                if (vv >= 0)
                {
                    ListViewItem i = listView1.Items[vv];

                    //foreach (ListViewItem i in listView1.Items)
                    //{
                    if (i.Tag == c)
                    {
                        i.SubItems[1].Text = c.elapsedFrames.ToString();
                        i.SubItems[2].Text = c.done.ToString();
                        i.SubItems[3].Text = c.isValidData.ToString();
                        i.SubItems[4].Text = c.done.ToString();
                    }
                    //}

                }

            }
            
          
            listView1.EndUpdate();
            ///listView1.Refresh();

        }
        #endregion

        #region Utilites | Dialogs
        //not needed anymore, just set image height once set to the picbox
        private void setVideoProperties()
        {
            HThresh = pictureBox1.Image.Height;
            WThresh = pictureBox1.Image.Height;

        }

        //returns a point that is th ecenter of a rectangle.
        public Point getCenter(Rectangle rect)
        {
            return new Point(rect.Left + rect.Width / 2, rect.Top + rect.Height / 2);
        }
        //just a show dialogs, incase a dialog is needed.
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
        //set values for the old rectangles, Threshold retangle, ...
        public void setRectangleProperties()
        {
            double defH = -1 * (.10 * mRect.Height);
            double defW = -1 * (.10 * mRect.Width);
            ThreshRect = Rectangle.Inflate(mRect, (int)defW, (int)defH);
            FinishRect = Rectangle.Inflate(mRect, (-1 * (mRect.Width - 5)), (-1 * (mRect.Width - 5)));
        }
        #endregion

        //OLD WAY OF DRAWING THE RECT == NOT NEEDED ANYMORE
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
            if (!isRunning)
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
            if (mDown == true && !isRunning)
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
           // loadVideo(@"C:\Users\sstrickland\Desktop\Tracking Video\Barrett_BarrettLakes2014-06-12_15-48-221.avi");


        }


        //++++++++++++++++++++++++++++++++++++++++++++++++++   C U T T  O F F  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        //NOTE Due to the ridiculous due date for the first version and due to the attempt at finishing on time, this code is copied and modified from: 
        /*http://blog.csharphelper.com/2010/06/12/let-the-user-draw-polygons-and-move-them-in-c.aspx*/

        #region Drawing Polygon

        // The "size" of an object for mouse over purposes.
        private const int object_radius = 5;

        // We're over an object if the distance squared
        // between the mouse and the object is less than this.
        private const int over_dist_squared = object_radius * object_radius;

        // Each polygon is represented by a List<Point>.
        private List<List<Point>> Polygons = new List<List<Point>>();

        // Points for the new polygon.
        private List<Point> NewPolygon = null;

        // The current mouse position while drawing a new polygon.
        private Point NewPoint;

        // The polygon and index of the corner we are moving.
        private List<Point> MovingPolygon = null;
        private int MovingPoint = -1;
        private int OffsetX, OffsetY;

        // Start or continue drawing a new polygon,
        // or start moving a corner or polygon.
        private void pbRectangle_MouseDown(object sender, MouseEventArgs e)
        {
            // See what we're over.
            Point mouse_pt = e.Location;
            List<Point> hit_polygon;
            int hit_point;

            if (NewPolygon != null)
            {
                // We are already drawing a polygon.
                // If it's the right mouse button, finish this polygon.
                if (e.Button == MouseButtons.Right)
                {
                    // Finish this polygon.
                    if (NewPolygon.Count > 2)
                    {// === After polygon is complete we set the corners of the polygon
                        Polygons.Add(NewPolygon);
                        poly.Corners = Polygons.FirstOrDefault();
                        poly.setPoints(this.pictureBox1);
                    }
                    NewPolygon = null;


                    // We no longer are drawing.
                    pbRectangle.MouseMove += pbRectangle_MouseMove_NotDrawing;
                    pbRectangle.MouseMove -= pbRectangle_MouseMove_Drawing;
                }
                else
                {
                    // Add a point to this polygon.
                    if (NewPolygon[NewPolygon.Count - 1] != e.Location)
                    {
                        NewPolygon.Add(e.Location);
                    }
                }
            }
            else if (MouseIsOverCornerPoint(mouse_pt, out hit_polygon, out hit_point))
            {
                // Start dragging this corner.
                pbRectangle.MouseMove -= pbRectangle_MouseMove_NotDrawing;
                pbRectangle.MouseMove += pbRectangle_MouseMove_MovingCorner;
                pbRectangle.MouseUp += pbRectangle_MouseUp_MovingCorner;

                // Remember the polygon and point number.
                MovingPolygon = hit_polygon;
                MovingPoint = hit_point;

                // Remember the offset from the mouse to the point.
                OffsetX = hit_polygon[hit_point].X - e.X;
                OffsetY = hit_polygon[hit_point].Y - e.Y;
                //update polygon
                poly.Corners = Polygons.FirstOrDefault();
                poly.setPoints(this.pictureBox1);
            }
            else if (MouseIsOverPolygon(mouse_pt, out hit_polygon))
            {
                // Start moving this polygon.
                pbRectangle.MouseMove -= pbRectangle_MouseMove_NotDrawing;
                pbRectangle.MouseMove += pbRectangle_MouseMove_MovingPolygon;
                pbRectangle.MouseUp += pbRectangle_MouseUp_MovingPolygon;

                // Remember the polygon.
                MovingPolygon = hit_polygon;

                // Remember the offset from the mouse to the segment's first point.
                OffsetX = hit_polygon[0].X - e.X;
                OffsetY = hit_polygon[0].Y - e.Y;
                //update polygon
                poly.Corners = Polygons.FirstOrDefault();
                poly.setPoints(this.pictureBox1);
            }
            else
            {
                // Start a new polygon.
                NewPolygon = new List<Point>();
                NewPoint = e.Location;
                NewPolygon.Add(e.Location);

                // Get ready to work on the new polygon.
                pbRectangle.MouseMove -= pbRectangle_MouseMove_NotDrawing;
                pbRectangle.MouseMove += pbRectangle_MouseMove_Drawing;
            }

            
            // Redraw.
            pbRectangle.Invalidate();
        }

        // Move the next point in the new polygon.
        private void pbRectangle_MouseMove_Drawing(object sender, MouseEventArgs e)
        {
            NewPoint = e.Location;
            pbRectangle.Invalidate();
        }

        // Move the selected corner.
        private void pbRectangle_MouseMove_MovingCorner(object sender, MouseEventArgs e)
        {
            // Move the point.
            MovingPolygon[MovingPoint] = new Point(e.X + OffsetX, e.Y + OffsetY);

            // Redraw.
            pbRectangle.Invalidate();
        }

        // Finish moving the selected corner.
        private void pbRectangle_MouseUp_MovingCorner(object sender, MouseEventArgs e)
        {
            pbRectangle.MouseMove += pbRectangle_MouseMove_NotDrawing;
            pbRectangle.MouseMove -= pbRectangle_MouseMove_MovingCorner;
            pbRectangle.MouseUp -= pbRectangle_MouseUp_MovingCorner;
        }

        // Move the selected polygon.
        private void pbRectangle_MouseMove_MovingPolygon(object sender, MouseEventArgs e)
        {
            // See how far the first point will move.
            int new_x1 = e.X + OffsetX;
            int new_y1 = e.Y + OffsetY;

            int dx = new_x1 - MovingPolygon[0].X;
            int dy = new_y1 - MovingPolygon[0].Y;

            if (dx == 0 && dy == 0) return;

            // Move the polygon.
            for (int i = 0; i < MovingPolygon.Count; i++)
            {
                MovingPolygon[i] = new Point(
                    MovingPolygon[i].X + dx,
                    MovingPolygon[i].Y + dy);
            }

            // Redraw.
            pbRectangle.Invalidate();
        }

        // Finish moving the selected polygon.
        private void pbRectangle_MouseUp_MovingPolygon(object sender, MouseEventArgs e)
        {
            pbRectangle.MouseMove += pbRectangle_MouseMove_NotDrawing;
            pbRectangle.MouseMove -= pbRectangle_MouseMove_MovingPolygon;
            pbRectangle.MouseUp -= pbRectangle_MouseUp_MovingPolygon;
        }

        // See if we're over a polygon or corner point.
        private void pbRectangle_MouseMove_NotDrawing(object sender, MouseEventArgs e)
        {

        }

        // Redraw old polygons in blue. Draw the new polygon in green.
        // Draw the final segment dashed.
        private void pbRectangle_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;
            //e.Graphics.Clear(pbRectangle.BackColor);
            SolidBrush semiT = new SolidBrush(Color.FromArgb(60, 0, 0, 0));
            // Draw the old polygons.
            foreach (List<Point> polygon in Polygons)
            {
                // Draw the polygon.
                e.Graphics.FillPolygon(semiT, polygon.ToArray());
                e.Graphics.DrawPolygon(Pens.Blue, polygon.ToArray());

                // Draw the corners.
                foreach (Point corner in polygon)
                {
                    Rectangle rect = new Rectangle(
                        corner.X - object_radius, corner.Y - object_radius,
                        2 * object_radius + 1, 2 * object_radius + 1);
                    e.Graphics.FillEllipse(Brushes.White, rect);
                    e.Graphics.DrawEllipse(Pens.Fuchsia, rect);
                }
            }

            // Draw the new polygon.
            if (NewPolygon != null)
            {
                // Draw the new polygon.
                if (NewPolygon.Count > 1)
                {
                    e.Graphics.DrawLines(Pens.DarkTurquoise, NewPolygon.ToArray());
                }

                // Draw the newest edge.
                if (NewPolygon.Count > 0)
                {
                    using (Pen dashed_pen = new Pen(Color.DarkTurquoise))
                    {
                        dashed_pen.DashPattern = new float[] { 3, 3 };
                        e.Graphics.DrawLine(dashed_pen,
                            NewPolygon[NewPolygon.Count - 1],
                            NewPoint);
                    }
                }
            }
        }

        // See if the mouse is over a corner point.
        private bool MouseIsOverCornerPoint(Point mouse_pt, out List<Point> hit_polygon, out int hit_pt)
        {
            // See if we're over a corner point.
            foreach (List<Point> polygon in Polygons)
            {
                // See if we're over one of the polygon's corner points.
                for (int i = 0; i < polygon.Count; i++)
                {
                    // See if we're over this point.
                    if (FindDistanceToPointSquared(polygon[i], mouse_pt) < over_dist_squared)
                    {
                        // We're over this point.
                        hit_polygon = polygon;
                        hit_pt = i;
                        return true;
                    }
                }
            }

            hit_polygon = null;
            hit_pt = -1;
            return false;
        }

        // See if the mouse is over a polygon's body.
        private bool MouseIsOverPolygon(Point mouse_pt, out List<Point> hit_polygon)
        {
            // Examine each polygon.
            // Examine them in reverse order to check the ones on top first.
            for (int i = Polygons.Count - 1; i >= 0; i--)
            {
                // Make a GraphicsPath representing the polygon.
                GraphicsPath path = new GraphicsPath();
                path.AddPolygon(Polygons[i].ToArray());

                // See if the point is inside the GraphicsPath.
                if (path.IsVisible(mouse_pt))
                {
                    hit_polygon = Polygons[i];
                    return true;
                }
            }

            hit_polygon = null;
            return false;
        }

        // Calculate the distance squared between two points.
        private int FindDistanceToPointSquared(Point pt1, Point pt2)
        {
            int dx = pt1.X - pt2.X;
            int dy = pt1.Y - pt2.Y;
            return dx * dx + dy * dy;
        }
        #endregion


  
        private void listView1_DrawItem(object sender, DrawListViewItemEventArgs e)
    {

    }


    }
}
