/****************************************************************************/
/*  Linker command file for the TI TMS320C4x Compiler                       */
/****************************************************************************/

-c		/* use 'C' link conventions for ROM model */
-ar		/* retain relocation entries in executable module */
-stack 512	/* Specify the stack size */
-heap 2048	/* Specify the system (dynamic) memory size */

MEMORY
{
	IRAM0:	origin = 002FF800h	length = 0800h  /* Int 0, 1k   */
/*	IRAM1:	origin = 002FFC00h	length = 0400h*/	/* Int 1, 1k   */
	ERAM0:	origin = 00300000h	length = 8000h	/* Ext 0, 32k  */
	ERAM1:	origin = 00308000h	length = 8000h  /* Ext 1, 32k  */
	ERAM2:	origin = 80000000h	length = 8000h  /* Ext 2, 32k  */
}

SECTIONS
{


GROUP: {
	.text:  {}		/* Code */
	.bss:   {}		/* Global & static vars (beware models) */
	.cinit: {}		/* Initialization tables */
	}	>ERAM0

GROUP: {
	.sysmem: {}		/* Dynamic memory */
	.data: {		/* ASSEMBLER DATA */
/*		_source_array = .;  . += 1024;*/	/* Define source_array space */
/*		_imag_data = .;  . += 1024;*/	/* Define imag_data space */
		}
	}	>ERAM1

GROUP: {
	.const:	{}		/* Constants */
	.stack: {}		/* SYSTEM STACK */
	}	>IRAM0


}	/* End of lnk4x.cmd */


