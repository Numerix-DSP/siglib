# siglib
SigLib Digital Signal Processing and Machine Learning Library

## What is SigLib?

SigLib is a highly portable DSP and ML library that supports many different architectures and
operating systems, include:

* x86
* ARM Cortex-A and Cortex-M
* RISC-V
* PowerPC
* DSPs from TI, ADI etc.

## Why use SigLib?

SigLib includes over 1000 fully tested DSP functions and now includes functions for Artificial
Intelligence and Machine Learning.

SigLib is the easiest way to develop portable signal processing applications because the code can be
developed graphically on a standard desktop or laptop computer (Windows, Linux or Mac OSX) and then
re-compiled for the target DSP, without the graphical functionality.

## Installation

Ensure you have the required dependencies installed, as per the following section:

### Dependencies

#### Gnuplot and Gnuplot/C

SigLib uses [Gnuplot](http://gnuplot.info/) and Gnuplot/C for displaying signals and data graphically.

Gnuplot/C is included in the SigLib package and pipes data to Gnuplot.

Under Windows, download and install the gp528-win64-mingw.exe package
from here :
https://sourceforge.net/projects/gnuplot/files/gnuplot/5.2.8/gp528-win64-mingw.exe/download .
add the location of the installation to the system path.

Under Ubuntu Linux, use the following command::

    sudo apt install gnuplot

### SigLib Installation

Extract the .zip archive and set the appropriate compiler environment variables.

#### Building The Source

All of the source is included in the 'src' folder, which includes batch files, shell scripts and makefiles
for the different supported architectures.

## Documentation

Documentation is available in the folder 'docs', in particular :

* siglib_users.pdf  - SigLib User's Manual - This is the place to start for installation and overview
information
* siglib_ref.pdf    - SigLib Function Reference Manual - This is the place to go for detailed functional
overviews

The easiest way to navigate the folders is to use the provided html files, starting with [Welcome.html](./welcome.html).

## License

SigLib is free for educational and personal use, all other use cases require a developer's license,
which is available from: [Delta Numerix](https://numerix-dsp.com).

[SigLib Licensing Details](./license.html).

Delta Numerix are specialists in writing and supporting Signal Processing and Machine Learning
applications.

To inquire about our services, contact us at numerix@numerix-dsp.com.

Copyright ©; 2022, Delta Numerix All rights reserved.
SigLib and Digital Filter Plus are trademarks of Delta Numerix All
other trademarks acknowledged.

Delta Numerix are continuously increasing the functionality of SigLib
and reserve the right to alter the product at any time.
