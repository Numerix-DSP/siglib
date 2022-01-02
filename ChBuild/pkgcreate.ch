#!/bin/ch
#include <unistd.h>   // for access()
#include <dlfcn.h>    // for dlopen()
#include <chshell.h>  // for chinfo()

string_t pkgname="siglib";	// define package name
chinfo_t info;              // for Ch version number
string_t cwd = _cwd;        // the current working directory
string_t debugFile;         // compilation debug information in Windows
string_t debug;
string_t makecmd;
#if defined(_WIN32_)
    debug=">nul 2>nul";         // surpress messages during cleaning  in Windows
    debugFile =">logfile 2>&1"; // compilation debug information in 'logfile'
    makecmd = "nmake -f Makefile.win";
#else
    makecmd = "make -f Makefile";
#endif

//make sure pgkcreate.ch is run from the current working directory
if(access("pkgcreate.ch", R_OK)) {
    echo Run ./pkgcreate.ch in the current directory.
    exit(-1);
}
// run this script in proper Ch version
chinfo(&info);
if ((info.vermajor*100+ info.verminor*10 +  info.vermicro) < 501) {
   echo "To run this script, you need to install Ch version 5.0.1.12201 or higher"
   echo "You can download the latest version from http://www.softintegration.com/download"
   exit(-1);
}

//echo clean up existing directory and create new ones
if (!access(pkgname, F_OK))
  rm -rf $pkgname
mkdir $pkgname
mkdir $pkgname/dl $pkgname/lib $pkgname/include $pkgname/demos $pkgname/bin

echo Calling sed to remove "const", "SIGLIB_FUNC_DECL" and "SIGLIB_PTR_DECL"
sed -e "s/const //g" include/siglib.h > include/siglib2.h
sed -e "s/SIGLIB_FUNC_DECL //g" include/siglib2.h > include/siglib3.h
sed -e "s/SIGLIB_PTR_DECL//g" include/siglib3.h > include/siglib4.h
cp include/siglib4.h $pkgname/include/siglib.h

//echo copying header files and demo programs ...
//cp -rf demos/* $pkgname/demos

echo extracting function prototypes ...
processhfile("?", 0, ";",  stradd(pkgname, "/include/siglib.h"), "chfcreate/siglib.h", NULL);

//echo removing special functions
//removeFuncProto("chfcreate/siglib.h", "SUF_Debugfprintf", 0);

echo generating siglib_chdl.c in c/siglib and *.chf in $pkgname/lib
c2chf chfcreate/siglib.h -h _siglibCh_handle -o c c -o chf $pkgname/lib -l SUF_Debugfprintf  SUF_Debugvfprintf

//echo Hit CR to continue ...
//getchar();

echo patching special .chf files
//cp -f chfhandmade/*.chf $pkgname/lib
chmod 644 $pkgname/lib/*.chf

echo making .dl files
cd c
$makecmd clean $debug
cp ../lib/*.lib .
$makecmd $debugFile

cd $cwd
echo .DLL path and testing .dl file
//#pragma exec _path=stradd(_path, "siglib/bin");
if (dlopen("./c/libsiglib.dl", RTLD_LAZY) == NULL) {
    printf("Error: test of loading siglib.dl: %s\n", dlerror());
    exit(-1);
}
cd c
mv -f libsiglib.dl $cwd/$pkgname/dl
$makecmd clean $debug
echo package $pkgname created successfully!

