/* Example program to call SigLib functions from Ch */

#include <stdio.h>
#include <dlfcn.h>
#include <siglibch.h>			/* SigLib Ch Header file */

#define ARRAY_LENGTH	5

SFLOAT	*pArray1, *pArray2, *pResults;

int main()

{
	void	*SigLib_handle;
	void	*SigLib_fptr;
	SFIX	i;
	SFLOAT	SigLibVersion;


	SigLibVersion = SUF_SiglibVersion();
	printf ("SigLibVersion = %lf\n", SigLibVersion);

						//	Process some data in arrays
	printf ("\nAdding the data in two SigLib arrays\n");

	pArray1 = SUF_VectorArrayAllocate (ARRAY_LENGTH);
	pArray2 = SUF_VectorArrayAllocate (ARRAY_LENGTH);
	pResults = SUF_VectorArrayAllocate (ARRAY_LENGTH);

	for (i = 0; i < ARRAY_LENGTH; i++)
	{
		*(pArray1+i) = (SFLOAT)i;
		*(pArray2+i) = (SFLOAT)(i*2);
	}

	SDA_Add2 (pArray1, pArray2, pResults, ARRAY_LENGTH);

	for (i = 0; i < ARRAY_LENGTH; i++)
	{
		printf ("Result [%ld] = %lf\n", i, *(pResults+i));
	}

	SUF_MemoryFree (pArray1);				/* Free memory */
	SUF_MemoryFree (pArray2);
	SUF_MemoryFree (pResults);
}


