using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WindowsFormsApplicationTVA
{
    class Analysis
    {
        public double Average(double t1, double t2)
        {
            double average = (t1 + t2) / 2;
            return average;
        }
        public double CalTimeDiff(double t1, double t2)
        {
            double td = t2 - t1;
            return td;
        }
        public double SSD(double t1, double t2,double vf,double vi,double v,double grade)
        {
            double g = 32.2;
            double a = 11.2;
            double G = grade / 100;
            double t= CalTimeDiff(t1,t2);
            //if G>0
            double SSD= v*t+(vf*vf-vi*vi)/2*g*(a/g+G);
            //else
            // SSD= v*t+(vf*vf-vi*vi)/2*g*(a/g+G);
            return SSD;
        }
        int Isaccelerating(double T0,double T1,double T2)
        {
            double t1 = CalTimeDiff(T0, T1);
            double t2 = CalTimeDiff(T1, T2);
            if (t1 == t2)
                return 1;//no acceleration
            else if (t1 > t2)
                return 2;//accelerated
            else
                return 3;//decelerated
        }
        static double f(double x) 
        { 
            double f=Math.Exp(-x*x/2)/Math.Sqrt(2*Math.PI); 
            return f; 
        } 
  [STAThread] 
//create a textBox to show the result in the form
        private void createCurve() 
        { 
            int S=2; 
            double Q=0; 
            double b=Convert.ToDouble(textBoxU.Text); 
            while(true) 
            {      
                double a=b-S; 
                int M=1,N=1,k=1,m=1; 
                double ep,I,h;   
                ep=0.000000000001;     
                h=b-a; 
                I=h*(f(a)+f(b))/2;     
                double[,] T=new double[5000,5000]; 
                T[1,1]=I; 
                while(1>0) 
                { 
                     N=(int)Math.Pow(2,m-1); 
                     if(N>5000) 
                     {
                         break; 
                     } 
                     else 
                     { 
                         h=h/2; 
                         I=I/2; 
                         for(int i=1;i<=N;i++) 
                            I=I+h*f(a+(2*i-1)*h); 
                         T[m+1,1]=I; 
                         M=2*N; 
                         k=1;   
                        while(M>1) 
                        { 
                            T[m+1,k+1]=(Math.Pow(4,k)*T[m+1,k]-T[m,k])/(Math.Pow(4,k)-1); 
                            M=M/2; 
                            k=k+1; 
                        } 
                    if(Math.Abs(T[k,k]-T[k-1,k-1])<ep)     
                         break; 
                    m=m+1; 
                } 
            } 
            I=T[k,k]; 
            Q=Q+I; 
            if(Math.Abs(I)<ep) 
                break; 
            b=a;S=2*S;     
            textBoxResult.Text=Convert.ToString(Math.Round(Q,10)); 
         } 
  }
