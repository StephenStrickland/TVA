﻿using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WindowsFormsApplicationTVA
{
    public class Car
    {

        //centiseconds
        public int start { get; set; }
        public int end { get; set; }
        public int diff { get; set; }
        public int average { get; set; }
        public bool tracked { get; set; }
        public bool accelerated { get; set; }
        public string timestamp { get; set; }
       // public Guid parent { get; set; }
      //  public Rectangle currentRect { get; set; }
        public int elapsedFrames { get; set; }
        public bool done { get; set; }
        public Point position { get; set; }
        public bool isValidData { get; set;}
        public bool isThrough { get; set; }

        public bool set { get; set;}
        public int id { get; set; }

        public virtual List<Rectangle> History { get; set; }

        //returns the time
        public int getTime(int fps)
        {
            int result = 0;

            result = (elapsedFrames/fps);
             
            return result;
            
        }
        //if you are giving a car its new postion/rect call this
        public Rectangle currentRect
        {
            
            set 
        {
            History.Insert(0, value);
        }
        }

        public Car()
        {
            History = new List<Rectangle>();
        }

        //compares the first and the last position of the car, if it is negative then it moved down, else moved up(turn)
        public void setType()
        {
            if(History.LastOrDefault().Location.Y - History[0].Location.Y < 0)
            {
                isThrough = true;
            }
        }

        public String toString() 
        {
            StringBuilder sb = new StringBuilder();
            sb.Append(this.id);
            sb.Append("| Frames:  " + this.elapsedFrames);
            sb.Append(" | Time: " + this.isValidData);
            if (History != null && History.Count > 0)
                sb.Append("   Location: " + History[0].ToString());


            return sb.ToString();
        }

      

        //public void calcDiff()
        //{
        //    diff = end - start;
        //}

        //public void calcAverage()
        //{
        //    average = ((end - start)/2);
        //}

    }
}
