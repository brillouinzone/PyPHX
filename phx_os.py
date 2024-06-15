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

# General type definitions
i8 = ctypes.c_int8
ui8 = ctypes.c_uint8
i16 = ctypes.c_int16
ui16 = ctypes.c_uint16
i32 = ctypes.c_int32
ui32 = ctypes.c_uint32
i64 = ctypes.c_int64
ui64 = ctypes.c_uint64
m16 = ctypes.c_int32
mu16 = ctypes.c_uint32
m32 = ctypes.c_int32
mu32 = ctypes.c_uint32

# Pointer type definition
if ctypes.sizeof(ctypes.c_void_p) == 8:
    ptr_t = ctypes.c_uint64
else:
    ptr_t = ctypes.c_uint32

# Type Definitions
tHandle = mu32
tFlag = ui32
tPhxFnName = ctypes.c_char_p
tPhxErrStr = ctypes.c_char_p
tPHX = ctypes.c_void_p

tPhxPhysAddr = ui64
tPhxPhysLen = ui64