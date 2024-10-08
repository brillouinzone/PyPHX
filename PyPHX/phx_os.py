import ctypes
from ctypes import c_void_p, c_size_t

# Load standard C library
libc = ctypes.CDLL('msvcrt.dll')

# Define Python equivalents of the C macros
def _PHX_Malloc(size):
    return libc.malloc(c_size_t(size))

def _PHX_Free(ptr):
    libc.free(c_void_p(ptr))

# Load Windows API for Sleep
kernel32 = ctypes.WinDLL('kernel32')

def _PHX_SleepMs(milliseconds):
    kernel32.Sleep(ctypes.c_uint(milliseconds))


# Pointer type definition
if ctypes.sizeof(ctypes.c_void_p) == 8:
    ptr_t = ctypes.c_uint64
else:
    ptr_t = ctypes.c_uint32
