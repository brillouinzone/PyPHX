import threading
from PyPHX.common import *
from PyPHX.phx_api import *
import numpy as np
import matplotlib.pyplot as plt


# Load the DLL
directory="C:/Users/Public/Documents/Active Silicon/ActiveSDK v01.08.12/examples/build/windows/output/x64_Release"
dll_path = os.path.abspath(os.path.join(directory,'phx_live_dll_2.dll'))
phxdll = ctypes.CDLL(dll_path)


# define the function prototypes
# Define the return types of the functions
# phxdll.get_buffer_address.restype = ctypes.POINTER(ctypes.c_ubyte) # use if 8 bit image
phxdll.get_buffer_address.restype = ctypes.POINTER(ctypes.c_uint16)  # Change to 16-bit
phxdll.get_buffer_width.restype = ctypes.c_uint32
phxdll.get_buffer_height.restype = ctypes.c_uint32

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

phxdll.Initphxlive.argtypes = [tPhxCmd, tCxpRegisters]
phxdll.Initphxlive.restype = c_int

phxdll.stop_looping.argtypes = []
phxdll.stop_looping.restype = None

phxdll.access_buffer.argtypes = []
phxdll.access_buffer.restype = None

# display_buffer = stImageBuff()
#
# display_buffer.pvAddress = ctypes.cast(ctypes.create_string_buffer(640 * 480),
#                                        ctypes.c_void_p)  # Example buffer size
# display_buffer.pvContext = None

def phxlive():
    # argc = len(sys.argv)
    #
    # argv = (ctypes.c_char_p * argc)(*map(lambda arg: ctypes.create_string_buffer(arg.encode('utf-8')), sys.argv))

    sPhxCmd = phxdll.InitPhxCmd()
    sCameraRegs = phxdll.InitCameraRegs()
    nStatus = phxdll.InitMemory(sPhxCmd, sCameraRegs)
    # Initialize the display buffer

    phxdll.InitPhxCommonKbInit()
    phxdll.Initphxlive(sPhxCmd, sCameraRegs)
    phxdll.InitPhxCommonKbClose()

    return 0



def main():
    # Start the C function in a separate thread

    import time
    loop_thread = threading.Thread(target=phxlive)
    loop_thread.start()
    time.sleep(1)

    print("waiting for capture trigger")

    # Wait for the loop thread to exit
    t0=time.perf_counter()
    while (time.perf_counter()-t0 < 3):
        tic = 1000*time.perf_counter()
        phxdll.access_buffer()
        toc = 1000*time.perf_counter()
        elapsed = toc - tic
        print(f"time elapsed{elapsed:0.5f}")
    time.sleep(1)
    # Get the buffer details
    buffer_address = phxdll.get_buffer_address()
    buffer_width = phxdll.get_buffer_width()
    buffer_height = phxdll.get_buffer_height()


    if buffer_address:
        # Convert the buffer to a NumPy array
        buffer_size = buffer_width * buffer_height
        buffer_array = np.ctypeslib.as_array(buffer_address, shape=(buffer_size,))
        buffer_image = buffer_array.reshape((buffer_height, buffer_width))
        plt.figure()
        plt.imshow(buffer_image)
        print(f"npmax = {np.max(buffer_image)}")
        plt.show()

    else:
        print("Buffer address is NULL")


    # Wait for the loop thread to exit
    phxdll.stop_looping()
    loop_thread.join()




if __name__ == "__main__":
    main()

