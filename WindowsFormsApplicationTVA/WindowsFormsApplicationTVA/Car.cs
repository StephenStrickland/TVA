using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WindowsFormsApplicationTVA
{
    class Car
    {

        //centiseconds
        public int start { get; set; }
        public int end { get; set; }
        public int diff { get; set; }
        public int average { get; set; }
        public bool tracked { get; set; }
        public bool accelerated { get; set; }
        public string timestamp { get; set; }
        public Guid parent { get; set; }
        public int X { get; set; }
        public int Y { get; set; }
        public Rectangle currentRect { get; set; }
        public int elapsedFrames { get; set; }



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
