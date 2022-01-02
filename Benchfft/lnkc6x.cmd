/****************************************************************************/
/*  Linker command file for the TI TMS320C62x & C67x  v3.00 Compiler        */
/****************************************************************************/
-c
-heap  0x2000
-stack 0x4000

/* Memory Map 0 */
/* 
MEMORY
{
        EXT0:   o = 00000000h   l = 01000000h
        EXT1:   o = 01000000h   l = 00400000h
        PMEM:   o = 01400000h   l = 00010000h 
        EXT2:   o = 02000000h   l = 01000000h
        EXT3:   o = 03000000h   l = 01000000h
        BMEM:   o = 80000000h   l = 00010000h 
}
*/

/* Memory Map 1 - the default */
MEMORY
{
        PMEM:   o = 00000000h   l = 00010000h 
        EXT0:   o = 00400000h   l = 01000000h
        EXT1:   o = 01400000h   l = 00400000h
        EXT2:   o = 02000000h   l = 01000000h
        EXT2:   o = 03000000h   l = 01000000h
        BMEM:   o = 80000000h   l = 00010000h 
} 

SECTIONS
{
    .text       >       PMEM
    .stack      >       BMEM
    .bss        >       BMEM
    .cinit      >       BMEM
    .cio        >       BMEM 
    .const      >       BMEM
    .data       >       BMEM
    .switch     >       BMEM 
    .sysmem     >       BMEM
/*    .far        >       EXT2*/
    .far        >       BMEM
}


