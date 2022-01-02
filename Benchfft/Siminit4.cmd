mr
;4K ROM INTERNAL
ma      0x00000000,0x1000 ,ram           ;2K EPROM
ma 0x100000, 0x40 , ram
;Com port 0 registers
ma 0x100040, 1, ram
ma 0x100041, 1, iport
ma 0x100042, 1, ram;oport
;Com port 1 registers
ma 0x100050, 1, ram
ma 0x100051, 1, iport
ma 0x100052, 1, oport
;Com port 2 registers
ma 0x100060, 1, ram
ma 0x100061, 1, iport
ma 0x100062, 1, oport
;Com port 3 registers
ma 0x100070, 1, ram
ma 0x100071, 1, iport
ma 0x100072, 1, oport
;Com port 4 registers
ma 0x100080, 1, ram
ma 0x100081, 1, iport
ma 0x100082, 1, oport
;Com port 5 registers
ma 0x100090, 1, ram
ma 0x100091, 1, iport
ma 0x100092, 1, oport
;DMA channel registers
ma 0x1000a0,0x60,ram
;LOCAL BUS
ma      0x002ff800,0x0400,ram          ;1K SRAM BLK0
ma      0x002ffc00,0x0400,ram          ;1K SRAM BLK1

ma      0x00300000,0x08000,ram         ;128K SRAM LOCAL, STRB0
ma      0x00308000,0x08000,ram         ;128K SRAM LOCAL, STRB0
ma      0x80000000,0x08000,ram         ;128K SRAM GLOBAL, STRB0

ma 0xffffffff,1,ram
;sconfig \bin\bin40\init.clr


