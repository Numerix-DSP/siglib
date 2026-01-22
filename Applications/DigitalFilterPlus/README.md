

                           Digital Filter Plus™

                       Digital Filter Design Program

                             Version 3.00

                              02-06-2021


# Introduction

This is the complete package for DFPlus digital filter design
program.

Due to it's age, this program is now deprecated and completely unsupported.
If you would like to design filters then you are recommended to use the
pyFilter applications, which you can find in the SigLib download.

Full documentation is in the docs folder.

Binary releases for Linux, macOS and Windows are in the releases folder.

If you wish to build the app for yourself then please follow these commands:

# Installation Notes

## Windows - Tested with Visual Studio 2022

Extract the latest version of wxWidgets .zip from https://www.wxwidgets.org/downloads/ to: C:\wxWidgets-3.2.4
Set the environment variable WXWIN to: C:\wxWidgets-3.2.4
Open a Visual Studio command prompt here: C:\wxWidgets-3.2.4\build\msw
Generate a release build:
nmake -f makefile.vc BUILD=release clean && nmake -f makefile.vc BUILD=release RUNTIME_LIBS=static
Ensure the SIGLIB_PATH environment variable is set correctly e.g.: Z:\Development\siglib

Open a Visual Studio command prompt here: siglib\ngl
Run the following command to build the Numerix Graphics Library:
build

Open a Visual Studio command prompt here: siglib\Applications\DigitalFilterPlus\src
Run the following command to build Digital Filter Plus:
nmake -f makefile.vc BUILD=release RUNTIME_LIBS=static WX_DEBUG=0

To perform a clean build:
nmake -f makefile.vc clean

## Linux

sudo apt install -y build-essential libgtk2.0-dev libgtk-3-dev mesa-utils freeglut3-dev libjpeg-dev liblzma-dev
Extract the latest version of wxWidgets .bz2 from https://www.wxwidgets.org/downloads/ to your development folder: tar -xjvf wxWidgets-3.2.4.tar.bz2
cd wxWidgets-3.2.4/build
mkdir build_gtk
cd build_gtk
../../configure  --with-gtk=3 --with-opengl
make
sudo make install
ldconfig
export WXWIN=~/wxWidgets-3.2.4

Open a command prompt here: siglib/ngl
Run the following command to build the Numerix Graphics Library:
./build_linux.sh

Open a command prompt here: siglib/Applications/DigitalFilterPlus/src
Run the following command to build Digital Filter Plus:
make -f makefile.lx

To perform a clean build:
make -f makefile.lx clean

## macOS

Extract the latest version of wxWidgets .bz2 from https://www.wxwidgets.org/downloads/ to your development folder: tar -xjvf wxWidgets-3.2.4.tar.bz2
cd wxWidgets-3.2.4/build
mkdir build-cocoa
cd build-cocoa
../../configure
../../configure --disable-shared --without-libtiff
make
sudo make install
export WXWIN=~/wxWidgets-3.2.4

Open a command prompt here: siglib/ngl
Run the following command to build the Numerix Graphics Library:
./build_macos.sh

Open a command prompt here: siglib/Applications/DigitalFilterPlus/src
Run the following command to build Digital Filter Plus:
make -f makefile.macos

To perform a clean build:
make -f makefile.macos clean

# History

VERSION : 3.00

RELEASE DATE : 03/05/2024

Open sourced

# License

This software is licensed under the terms of the GPL V2 license.

Digital Filter Plus, DFPlus and SigLib are trademarks of Delta Numerix all other
trademarks acknowledged.

For additional information, please contact :

Delta Numerix
Email : info@numerix-dsp.com
WWW : http:\\www.numerix-dsp.com

