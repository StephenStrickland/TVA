using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using Emgu.CV.UI;
using Emgu.CV;
using Emgu.CV.CvEnum;
using Emgu.CV.Structure;
using System.Drawing;

namespace WindowsFormsApplicationTVA
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
            //BuildTest("hello world", 400, 200, Color.Aquamarine, "wasssuuuuuupppppppp i hope thsi works", 80, 50, Color.Crimson);
        }

        public static void BuildTest(string windowName, Int32 windowWidth, Int32 windowHeight, Color bgColor, string text, Int32 startX, Int32 startY,
            Color txtColor)
        {

            CvInvoke.cvNamedWindow(windowName);

            using (Image<Bgr, Byte> windowImg = new Image<Bgr, byte>(windowWidth, windowHeight, new Bgr(bgColor.B, bgColor.G, bgColor.R)))
            {
                MCvFont textFont = new MCvFont(Emgu.CV.CvEnum.FONT.CV_FONT_HERSHEY_COMPLEX, 1.0, 1.0);

                windowImg.Draw(text, ref textFont, new Point(startX, startY), new Bgr(txtColor.B, txtColor.G, txtColor.R));

                CvInvoke.cvShowImage(windowName, windowImg);

                CvInvoke.cvWaitKey(0);
                CvInvoke.cvDestroyWindow(windowName);


            }

        }

    }
}
