using System;
using System.Collections.Generic;
using System.Runtime.InteropServices; // DllImport
using System.Text;

namespace SigLibTest
{
    unsafe class Program
    {
        [DllImport("siglib.dll")]
        public static extern double SDS_Max(double a, double b);

        [DllImport("siglib.dll")]
        public static extern double SUF_SiglibVersion();

                // Note long in C is 32 bit and in C# int is 32 bit
        [DllImport("siglib.dll")]
        public static extern void SDA_Add2(double[] a, double[] b, [Out] double[] c, int length);

        static void Main(string[] args)
        {
            double Version;
            Version = SUF_SiglibVersion();
            Console.WriteLine("SigLib Version : {0}", Version);

            double a = 1.0;
            double b = 2.0;
            double c;
            c = SDS_Max(a, b);
            Console.WriteLine("The maximum of {0} and {1} is {2}", a, b, c);

            double[] Src1 = new double[3] { 1.1f, 2.2f, 3.3f };
            double[] Src2 = new double[3] { 4.4f, 5.5f, 6.6f };
            double[] Dst = new double[3] { 7.7f, 8.8f, 9.9f };

            fixed (double* pSrc1 = &Src1[0])
            fixed (double* pSrc2 = &Src2[0])
            fixed (double* pDst  = &Dst[0])
            {
                SDA_Add2(Src1, Src2, Dst, 3);

                Console.WriteLine("The sum of two arrays :");
                foreach (double num in Dst)
                {
                    System.Console.WriteLine("{0:F1}", num);
                }
            }
        }
    }
}
