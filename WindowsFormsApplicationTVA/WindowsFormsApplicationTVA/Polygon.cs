using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;

namespace WindowsFormsApplicationTVA
{
    public class Polygon
    {
        public Polygon()
        {
            
           // setSides(TOP, BOTTOM);
        }
        /*
         *Points go Clock Wise starting in the lower left corner
         * 
         * bottom, left, top, right
         * 
         * 
         */


        //takes the Corner Points and sets them to the appropriate Point
        public void setPoints(PictureBox b)
        {
            if (Corners.Count > 3)
            {


                List<Point> tmp = Corners;
                tmp = tmp.OrderBy(xx => xx.Y).ThenByDescending(xx => xx.X).ToList();
                Top = tmp.First();
                Bottom = tmp.Last();

                tmp = tmp.Where(xx => xx != Top && xx != Bottom).OrderBy(xx => xx.X).ToList();
                Left = tmp[0];
                Right = tmp[1];

                Console.WriteLine(Left.ToString());
                Console.WriteLine(Right.ToString());
                Console.WriteLine(Top.ToString());
                Console.WriteLine(Bottom.ToString());

                //Left = b.PointToScreen(Left);
                //Right = b.PointToScreen(Right);
                //Top = b.PointToScreen(Top);
                //Bottom = b.PointToScreen(Bottom);

                //Left = tmp.Where(xx => xx != Top && xx != Bottom && (xx.X <= Bottom.X || xx.X <= Top.X) && xx.Y > Bottom.Y).FirstOrDefault();
                //Right = tmp.Where(xx => (xx != Top && xx != Bottom && xx != Left)).FirstOrDefault();
            }
        }
        public bool ContainsPoint(Point p)
        {
            GraphicsPath path = new GraphicsPath();
            path.AddPolygon(Corners.ToArray());
            return path.IsVisible(p);
        }

        //this is not limited to just the current polygon, give it any array of points and it will test if it is inside.
        public bool partialPolygonContainsPoint(Point[] pts, Point target)
        {
            GraphicsPath path = new GraphicsPath();
            path.AddPolygon(pts);
           return  path.IsVisible(target);
        }

        //takes four corners of rect and tests to see if atleast on point is in rect
        public bool partialPolygonContainsRect(Point[] pts, Rectangle target)
        {
             bool result = false;
            Point p1 = target.Location;
            Point p2 = new Point(target.X + target.Width, target.Y);
            Point p3 = new Point(target.X + target.Width, target.Y+target.Height);
            Point p4 = new Point(target.X + target.Width, target.Y+target.Height);
            Point p5 = getCenterofRect(target);

            if (partialPolygonContainsPoint(pts, p1) || partialPolygonContainsPoint(pts, p2) || partialPolygonContainsPoint(pts, p3) || partialPolygonContainsPoint(pts, p4) || partialPolygonContainsPoint(pts, p5))
                result = true;

            return result;
        }

        //returns the midpoint of two points
        public Point midPoint(Point lower, Point upper)
        {
            double y = Math.Abs(upper.Y - lower.Y)/2;
             double x = Math.Abs(upper.X - lower.X)/2;
             Point p = new Point((int)x, (int)y);
             return p;
        }

        //takes four corners of rect and tests to see if atleast on point is in rect
        public bool ContainsRect(Rectangle r)
        {
            bool result = false;
            Point p1 = r.Location;
            Point p2 = new Point(r.X + r.Width, r.Y);
            Point p3 = new Point(r.X + r.Width, r.Y+r.Height);
            Point p4 = new Point(r.X , r.Y+r.Height);
            Point p5 = getCenterofRect(r);

            if (ContainsPoint(p1) || ContainsPoint(p2) || ContainsPoint(p3) || ContainsPoint(p4) || ContainsPoint(p5))
                result = true;

            return result;
        }

        public Point getCenterofRect(Rectangle rect)
        {
            return new Point(rect.Left + rect.Width / 2, rect.Top + rect.Height / 2);
        }
        public Point Right {get; set;}
        public Point Left { get; set; }
        public Point Top { get; set; }
        public Point Bottom { get; set; }
        public GraphicsPath polGraphic { get; set; }
        public List<Point> Corners { get; set; }
        public List<Point> TopSide { get; set; }
        public List<Point> BottomSide { get; set; }
        //not used
        enum Sides
        {
            Top, Left, Right, Bottom
        };

        /*
         *List of corners 
         * 
         * set top bottom sides
         * see if point is inside and rects as well.
         * 
         * 
         * 
         * */

    }
}
