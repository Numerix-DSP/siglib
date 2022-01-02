;/*******************************************************************/
;/* siminit.cmd   v2.00                                             */  
;/* Copyright (c) 1996-1998  Texas Instruments Incorporated         */ 
;/*******************************************************************/
;
;;  For Memory Map 0
;ma      0x00000000, 0x01000000, RAM     ; External Memory Space CE0
;ma      0x01000000, 0x00400000, RAM     ; External Memory Space CE1
;ma      0x01400000, 0x00010000, RAM     ; Internal Program Memory
;ma      0x01800000, 0x00400000, RAM     ; Internal Peripheral Space
;ma      0x02000000, 0x01000000, RAM     ; External Memory Space CE2
;ma      0x03000000, 0x01000000, RAM     ; External Memory Space CE3
;ma      0x80000000, 0x00010000, RAM     ; Internal Data Memory

;  For Memory Map 1
ma      0x00000000, 0x00010000, RAM     ; Internal Program Memory
ma      0x00400000, 0x01000000, RAM     ; External Memory Space CE0
ma      0x01400000, 0x00400000, RAM     ; External Memory Space CE1
ma      0x01800000, 0x00400000, RAM     ; Internal Peripheral Space
ma      0x02000000, 0x01000000, RAM     ; External Memory Space CE2
ma      0x03000000, 0x01000000, RAM     ; External Memory Space CE3
ma      0x80000000, 0x00010000, RAM     ; Internal Data Memory
use     .

