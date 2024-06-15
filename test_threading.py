import threading
import os
from common import *
from phx_os import *
from phx_api import *
import numpy as np


# Load the DLL
directory="C:/Users/Public/Documents/Active Silicon/ActiveSDK v01.08.12/examples/build/windows/output/x64_Release"
dll_path = os.path.abspath(os.path.join(directory,'phx_live_dll.dll'))
phxdll = ctypes.CDLL(dll_path)


# define the function prototypes
phxdll.InitPhxCmd.argtypes = []
phxdll.InitPhxCmd.restype = tPhxCmd

phxdll.InitCameraRegs.argtypes = []
phxdll.InitCameraRegs.restype = tCxpRegisters

phxdll.InitMemory.argtypes = [tPhxCmd, tCxpRegisters]
phxdll.InitMemory.restype = ctypes.c_int

phxdll.InitPhxCommonKbInit.argtypes = []
phxdll.InitPhxCommonKbInit.restype = None

phxdll.InitPhxCommonKbClose.argtypes = []
phxdll.InitPhxCommonKbClose.restype = None

phxdll.Initphxlive.argtypes = [tPhxCmd, tCxpRegisters, stImageBuff]
phxdll.Initphxlive.restype = ctypes.c_int

phxdll.stop_looping.argtypes = []
phxdll.stop_looping.restype = None

display_buffer = stImageBuff()
display_buffer.pvAddress = ctypes.cast(ctypes.create_string_buffer(1024 * 1280),
                                       ctypes.c_void_p)  # Example buffer size
display_buffer.pvContext = None

def phxlive():
    import sys
    # argc = len(sys.argv)
    #
    # argv = (ctypes.c_char_p * argc)(*map(lambda arg: ctypes.create_string_buffer(arg.encode('utf-8')), sys.argv))

    sPhxCmd = phxdll.InitPhxCmd()
    sCameraRegs = phxdll.InitCameraRegs()
    nStatus = phxdll.InitMemory(sPhxCmd, sCameraRegs)
    # Initialize the display buffer

    phxdll.InitPhxCommonKbInit()
    display_buffer = phxdll.Initphxlive(sPhxCmd, sCameraRegs, display_buffer)
    phxdll.InitPhxCommonKbClose()

    return display_buffer



def main():
    # Start the C function in a separate thread

    loop_thread = threading.Thread(target=phxlive)
    loop_thread.start()
    print("doing other stuff now")
    # Stop the loop after some time or based on some condition
    # Assuming the buffer size and dimensions are known
    buffer_size = 640 * 480  # Example: width * height
    buffer_type = ctypes.POINTER(ctypes.c_ubyte * buffer_size)
    buffer_ptr = ctypes.cast(display_buffer.pvAddress, buffer_type)
    buffer_array = np.ctypeslib.as_array(buffer_ptr.contents)

    # Reshape the array if necessary
    width = 640
    height = 480
    image = buffer_array.reshape((height, width))

    # Now you can work with the image as a NumPy array
    print(image)

    import time

    time.sleep(5)  # Example delay
    phxdll.stop_looping()

    # Wait for the loop thread to exit
    loop_thread.join()


if __name__ == "__main__":
    main()