// SigLib Java Example program
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

public class main {

	static {
		try {
				System.loadLibrary("siglib_wrap");
			} catch (UnsatisfiedLinkError e) {
				System.err.println("Native code library failed to load.\n" + e);
				System.exit(1);
			}
		}

	public static void main(String argv[]) {

													// First create some objects using the pointer library.
		System.out.println("Testing the add library");
		SWIGTYPE_p_double a = siglib_wrap.new_doubleArray(3);
		SWIGTYPE_p_double b = siglib_wrap.new_doubleArray(3);
		SWIGTYPE_p_double c = siglib_wrap.new_doubleArray(3);

		for (int i=0; i<3; i++)						// Fill the arrays with some data
		{
			siglib_wrap.doubleArray_setitem(a, i, i+10);
			siglib_wrap.doubleArray_setitem(b, i, i*2);
		}

		System.out.println("a = ");
		for (int i=0; i<3; i++)
		{
			System.out.println("    " + siglib_wrap.doubleArray_getitem(a, i));
		}

		System.out.println("b = ");
		for (int i=0; i<3; i++)
		{
			System.out.println("    " + siglib_wrap.doubleArray_getitem(b, i));
		}

		siglib_wrap.SDA_Add2(a, b, c , 3);					// Call the add() function

		System.out.println("c = ");					// Print out the results
		for (int i=0; i<3; i++)
		{
			System.out.println("    " + siglib_wrap.doubleArray_getitem(c, i));
		}

		siglib_wrap.delete_doubleArray(a);				// Clean up the pointers
		siglib_wrap.delete_doubleArray(b);
		siglib_wrap.delete_doubleArray(c);
	}
}
