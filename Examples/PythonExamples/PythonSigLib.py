# SigLib Python Example Program
# Copyright (c) 2024 Delta Numerix

import ctypes
import numpy as np
import platform as _platform

print ('platform.system : ', _platform.system())
print ('platform.machine: ', _platform.machine())

A = np.array([3.4, 1.8, -2.8, 6.4])
B = np.zeros((A.size), dtype=np.double)

if _platform.system() == "Linux":
  lib = ctypes.cdll.LoadLibrary('../../lib/linux/siglib.so.1.0')
elif _platform.system() == "Darwin":
  lib = ctypes.cdll.LoadLibrary('../../lib/macos_x86_64/siglib.dylib')
elif _platform.system() == "Windows":
  lib = ctypes.cdll.LoadLibrary('../../lib/Microsoft/dynamic_library_64/Release/siglib.dll')

SDA_AbsMax = lib.SDA_AbsMax
SDA_AbsMax.restype = ctypes.c_double
absMax = SDA_AbsMax(ctypes.c_void_p(A.ctypes.data), ctypes.c_int(A.size))

SDA_SortMinToMax = lib.SDA_SortMinToMax
SDA_SortMinToMax(ctypes.c_void_p(A.ctypes.data), ctypes.c_void_p(B.ctypes.data), ctypes.c_int(A.size))

print ('absMax: ', absMax)
print ('B: %s' % B)

