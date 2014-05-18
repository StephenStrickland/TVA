using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Emgu.CV;
using Emgu.Util;



namespace WindowsFormsApplicationTVA
{
    public class drawing
    {
    }

    //    rectangle mrect;
   

    //        //initiate rectangle with mouse down event
    //protected override void onmousedown(mouseeventargs e)
    //{
    //    mrect = new rectangle(e.x, e.y, 0, 0);
    //    this.invalidate();
    //}

    //        //check if mouse is down and being draged, then draw rectangle
    //protected override void onmousemove(mouseeventargs e)
    //{
    //    if( e.button == mousebuttons.left)
    //    {
    //        mrect = new rectangle(mrect.left, mrect.top, e.x - mrect.left, e.y - mrect.top);
    //        this.invalidate();
    //    }
    //}

    //        //draw the rectangle on paint event
    //protected override void onpaint(painteventargs e)
    //{
    //            //draw a rectangle with 2pixel wide line
    //    using(pen pen = new pen(color.red, 2))
    //    {
    //    e.graphics.drawrectangle(pen, mrect);
    //    }

    //}
    }


//int main()  
//{  
//    src=cvLoadImage("lena.jpg",1);  
//    dst=cvCloneImage(src);  
//    cvNamedWindow("src",1);  
//    cvSetMouseCallback( "src", on_mouse, 0 );  
      
//    cvShowImage("src",src);  
//    cvWaitKey(0);   
//    cvDestroyAllWindows();  
//    cvReleaseImage(&src);  
//    cvReleaseImage(&dst);  
//    return 0;  
//} 