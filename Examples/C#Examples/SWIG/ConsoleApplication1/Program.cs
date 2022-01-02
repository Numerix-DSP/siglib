using System;
using System.Collections.Generic;
using System.Text;
using siglib_csharp;

namespace AddTest
{
	public class Program
	{
		static void Main(string[] args)
		{
            SWIGTYPE_p_double a = siglib_wrap.new_doubleArray(3);
            SWIGTYPE_p_double b = siglib_wrap.new_doubleArray(3);
            SWIGTYPE_p_double c = siglib_wrap.new_doubleArray(3);

			for (int i=0; i<3; i++)	{				    // Fill the arrays with some data
                siglib_wrap.doubleArray_setitem(a, i, i + 10);
                siglib_wrap.doubleArray_setitem(b, i, i * 2);
			}

			Console.WriteLine("a = ");
			for (int i=0; i<3; i++) {
                Console.WriteLine("    " + siglib_wrap.doubleArray_getitem(a, i));
			}

			Console.WriteLine("b = ");
			for (int i=0; i<3; i++) {
                Console.WriteLine("    " + siglib_wrap.doubleArray_getitem(b, i));
			}

            siglib_wrap.SDA_Add2(a, b, c, 3);		    // Call the add() function

			Console.WriteLine("c = ");                  // Print out the results
			for (int i=0; i<3; i++) {
                Console.WriteLine("    " + siglib_wrap.doubleArray_getitem(c, i));
			}

            siglib_wrap.delete_doubleArray(a);		    // Clean up the pointers
            siglib_wrap.delete_doubleArray(b);
            siglib_wrap.delete_doubleArray(c);

			System.Console.ReadLine();                  // Pause at the end to display the results
		}
	}
}
