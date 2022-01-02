<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: ce_dynamic_library_emulator - Win32 (WCE emulator) Release--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP4C.tmp" with contents
[
/nologo /W3 /I "c:\siglib\include" /D _WIN32_WCE=420 /D "WIN32_PLATFORM_PSPC=400" /D "_i386_" /D UNDER_CE=420 /D "UNICODE" /D "_UNICODE" /D "_X86_" /D "x86" /D "NDEBUG" /D "_USRDLL" /D "CE_DYNAMIC_LIBRARY_EMULATOR_EXPORTS" /D "SIGLIB_DLL_SOURCE" /Fp"emulatorRel/ce_dynamic_library_emulator.pch" /YX /Fo"emulatorRel/" /Gs8192 /GF /O2 /c 
"C:\siglib\src\adaptive.c"
"C:\siglib\src\arbfft.c"
"C:\siglib\src\chirpz.c"
"C:\siglib\src\comms.c"
"C:\siglib\src\complex.c"
"C:\siglib\src\complexa.c"
"C:\siglib\src\control.c"
"C:\siglib\src\convolve.c"
"C:\siglib\src\correlat.c"
"C:\siglib\src\datatype.c"
"C:\siglib\src\decint.c"
"C:\siglib\src\delay.c"
"C:\siglib\src\dsputil2.c"
"C:\siglib\src\dsputils.c"
"C:\siglib\src\dtmf.c"
"C:\siglib\src\fdfilter.c"
"C:\siglib\src\ffourier.c"
"C:\siglib\src\filter.c"
"C:\siglib\src\firfilt.c"
"C:\siglib\src\fourier.c"
"C:\siglib\src\icoder.c"
"C:\siglib\src\iirfilt.c"
"C:\siglib\src\image.c"
"C:\siglib\src\matrix.c"
"C:\siglib\src\minmax.c"
"C:\siglib\src\mod_a.c"
"C:\siglib\src\mod_d.c"
"C:\siglib\src\mux.c"
"C:\siglib\src\prbs.c"
"C:\siglib\src\pspect.c"
"C:\siglib\src\regress.c"
"C:\siglib\src\siggen.c"
"C:\siglib\src\siglib.c"
"C:\siglib\src\smath.c"
"C:\siglib\src\speech.c"
"C:\siglib\src\stats.c"
"C:\siglib\src\timing.c"
"C:\siglib\src\trig.c"
"C:\siglib\src\viterbi.c"
"C:\siglib\src\window.c"
]
Creating command line "cl.exe @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP4C.tmp" 
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP4D.tmp" with contents
[
commctrl.lib coredll.lib corelibc.lib /nologo /base:"0x00100000" /stack:0x10000,0x1000 /entry:"_DllMainCRTStartup" /dll /incremental:no /pdb:"emulatorRel/ce_dynamic_library_emulator.pdb" /nodefaultlib:"OLDNAMES.lib" /nodefaultlib:libc.lib /nodefaultlib:libcd.lib /nodefaultlib:libcmt.lib /nodefaultlib:libcmtd.lib /nodefaultlib:msvcrt.lib /nodefaultlib:msvcrtd.lib /out:"emulatorRel/ce_dynamic_library_emulator.dll" /implib:"emulatorRel/ce_dynamic_library_emulator.lib" /subsystem:windowsce,4.20 /MACHINE:IX86 
.\emulatorRel\adaptive.obj
.\emulatorRel\arbfft.obj
.\emulatorRel\chirpz.obj
.\emulatorRel\comms.obj
.\emulatorRel\complex.obj
.\emulatorRel\complexa.obj
.\emulatorRel\control.obj
.\emulatorRel\convolve.obj
.\emulatorRel\correlat.obj
.\emulatorRel\datatype.obj
.\emulatorRel\decint.obj
.\emulatorRel\delay.obj
.\emulatorRel\dsputil2.obj
.\emulatorRel\dsputils.obj
.\emulatorRel\dtmf.obj
.\emulatorRel\fdfilter.obj
.\emulatorRel\ffourier.obj
.\emulatorRel\filter.obj
.\emulatorRel\firfilt.obj
.\emulatorRel\fourier.obj
.\emulatorRel\icoder.obj
.\emulatorRel\iirfilt.obj
.\emulatorRel\image.obj
.\emulatorRel\matrix.obj
.\emulatorRel\minmax.obj
.\emulatorRel\mod_a.obj
.\emulatorRel\mod_d.obj
.\emulatorRel\mux.obj
.\emulatorRel\prbs.obj
.\emulatorRel\pspect.obj
.\emulatorRel\regress.obj
.\emulatorRel\siggen.obj
.\emulatorRel\siglib.obj
.\emulatorRel\smath.obj
.\emulatorRel\speech.obj
.\emulatorRel\stats.obj
.\emulatorRel\timing.obj
.\emulatorRel\trig.obj
.\emulatorRel\viterbi.obj
.\emulatorRel\window.obj
]
Creating command line "link.exe @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP4D.tmp"
<h3>Output Window</h3>
Compiling...
adaptive.c
arbfft.c
chirpz.c
comms.c
complex.c
complexa.c
control.c
convolve.c
correlat.c
datatype.c
decint.c
delay.c
dsputil2.c
dsputils.c
dtmf.c
fdfilter.c
ffourier.c
filter.c
firfilt.c
fourier.c
icoder.c
iirfilt.c
image.c
matrix.c
minmax.c
mod_a.c
mod_d.c
mux.c
prbs.c
pspect.c
regress.c
siggen.c
siglib.c
smath.c
speech.c
stats.c
timing.c
trig.c
viterbi.c
window.c
Linking...
   Creating library emulatorRel/ce_dynamic_library_emulator.lib and object emulatorRel/ce_dynamic_library_emulator.exp




<h3>Results</h3>
ce_dynamic_library_emulator.dll - 0 error(s), 0 warning(s)
</pre>
</body>
</html>
