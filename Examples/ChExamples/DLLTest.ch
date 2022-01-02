/* Example program to open SigLib DLL from Ch */

#include <stdio.h>
#include <dlfcn.h>
#include <siglib.h>

#define ARRAY_LENGTH	5

SFLOAT	*pArray1, *pArray2, *pResults;

int main()

{
	void	*SigLib_handle;
	void	*SigLib_fptr;
	SFIX	i;
	SFLOAT	SigLibVersion;

	printf("Opening DLL\n");

	SigLib_handle = dlopen ("libsiglib.dl", RTLD_LAZY);
	if (SigLib_handle == NULL)
	{
		fprintf(_stderr, "Error: dlopen(): %s\n", dlerror());
		fprintf(_stderr, "       cannot get SigLib_handle in MyChs.chs\n");
		exit(-1);
	}

	printf("DLL Opened\n");

						//	Get and print the SigLib version number
	SigLib_fptr = dlsym (SigLib_handle, "SUF_SiglibVersion_chdl");
	if (SigLib_NULL == FPtr)
	{
		printf("Error: %s(): dlsym(): %s\n", __func__, dlerror());
		exit(-1);
	}
	dlrunfun(SigLib_fptr, &SigLibVersion, SUF_SiglibVersion);

	printf ("SigLibVersion = %lf\n", SigLibVersion);
	printf ("\nTesting debug printf function\n");

						//	Process some data in arrays
	printf ("\nAdding the data in two SigLib arrays\n");

	SigLib_fptr = dlsym (SigLib_handle, "SDA_Add2_chdl");
	if (SigLib_NULL == FPtr)
	{
		printf("Error: %s(): dlsym(): %s\n", __func__, dlerror());
		exit(-1);
	}

	pArray1 = SUF_VectorArrayAllocate (ARRAY_LENGTH);
	pArray2 = SUF_VectorArrayAllocate (ARRAY_LENGTH);
	pResults = SUF_VectorArrayAllocate (ARRAY_LENGTH);

	for (i = 0; i < ARRAY_LENGTH; i++)			// Initialise the data in the arrays
	{
		*(pArray1+i) = (SFLOAT)i;
		*(pArray2+i) = (SFLOAT)(i*2);
	}

						// Add the data in the two arrays
	dlrunfun(SigLib_fptr, NULL, SDA_Add2, pArray1, pArray2, pResults, ARRAY_LENGTH);

	for (i = 0; i < ARRAY_LENGTH; i++)			// Print the results
	{
		printf ("Result [%ld] = %lf\n", i, *(pResults+i));
	}

	SUF_MemoryFree (pArray1);				/* Free memory */
	SUF_MemoryFree (pArray2);
	SUF_MemoryFree (pResults);

	dlclose(SigLib_handle);
}

