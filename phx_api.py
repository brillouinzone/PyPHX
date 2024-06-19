from phx_types import *
import ctypes,os

class PyPHX:
    def __init__(self, libpath=None):
        if libpath is None:
            raise ValueError("must supply a path to the phx_api.dll (create by compiling the phx_api.h with all .lib files linked.)")


        # Function to check if a function exists in the DLL
        def check_function_exists(dll, func_name):
            try:
                getattr(dll, func_name)
                return True
            except AttributeError:
                return False

        # Load the DLL
        dll_path = libpath  # Adjust the path to your DLL
        if not os.path.exists(dll_path):
            raise FileNotFoundError(f"Cannot find DLL at path: {dll_path}")
        else:
                        # Load the DLL
            self.phx_lib = ctypes.CDLL(libpath)


        # Define function prototypes
        self.phx_lib.Action.restype = ctypes.c_int
        self.phx_lib.Action.argtypes = [tHandle, ctypes.c_int, ctypes.c_int, ctypes.c_void_p]

        self.phx_lib.Create.restype = ctypes.c_int
        self.phx_lib.Create.argtypes = [ctypes.POINTER(tHandle), PHX_C_CALL]

        self.phx_lib.Open.restype = ctypes.c_int
        self.phx_lib.Open.argtypes = [tHandle]

        self.phx_lib.Close.restype = ctypes.c_int
        self.phx_lib.Close.argtypes = [ctypes.POINTER(tHandle)]

        self.phx_lib.Destroy.restype = ctypes.c_int
        self.phx_lib.Destroy.argtypes = [ctypes.POINTER(tHandle)]

        self.phx_lib.InitLive.restype = None
        self.phx_lib.InitLive.argtypes = [ctypes.POINTER(tPhxLive)]

        self.phx_lib.ParameterGet.restype = ctypes.c_int
        self.phx_lib.ParameterGet.argtypes = [tHandle, ctypes.c_int, ctypes.POINTER(ui32)]

        self.phx_lib.ParameterSet.restype = ctypes.c_int
        self.phx_lib.ParameterSet.argtypes = [tHandle, ctypes.c_int, ctypes.c_void_p]

    def InitLive(self,sPhxLive):
        return self.phx_lib.InitLive(ctypes.byref(sPhxLive))

    def Create(self,  error_handler):
        handle = tHandle(0)
        return self.phx_lib.Create(ctypes.byref(handle), error_handler), handle.value

    def Open(self, handle):
        return self.phx_lib.Open(handle)

    def ParameterSet(self, handle, etparam, data):
        return self.phx_lib.ParameterSet(handle, etparam, ctypes.byref(data))

    def ParameterGet(self, handle, param):
        data = ui32()
        status = self.phx_lib.ParameterGet(handle, param, ctypes.byref(data))
        return status, data.value

        # self.phx_lib.StreamRead.restype = etStat
        # self.phx_lib.StreamRead.argtypes = [tHandle, etAcq, ctypes.c_void_p]
        #
        # self.phx_lib.PHX_ControlRead.restype = etStat
        # self.phx_lib.PHX_ControlRead.argtypes = [tHandle, etControlPort, ctypes.c_void_p, ctypes.POINTER(ui8),
        #                                     ctypes.POINTER(ui32), ui32]
        #
        # self.phx_lib.PHX_ControlReset.restype = etStat
        # self.phx_lib.PHX_ControlReset.argtypes = [tHandle, etControlPort, ctypes.c_void_p, ui32]
        #
        # self.phx_lib.PHX_ControlWrite.restype = etStat
        # self.phx_lib.PHX_ControlWrite.argtypes = [tHandle, etControlPort, ctypes.c_void_p, ctypes.POINTER(ui8),
        #                                      ctypes.POINTER(ui32), ui32]
        #
        # self.phx_lib.PHX_BufferParameterGet.restype = etStat
        # self.phx_lib.PHX_BufferParameterGet.argtypes = [tHandle, tBufferHandle, etBufferParam, ctypes.c_void_p]
        #
        # self.phx_lib.PHX_BufferParameterSet.restype = etStat
        # self.phx_lib.PHX_BufferParameterSet.argtypes = [tHandle, tBufferHandle, etBufferParam, ctypes.c_void_p]
        #
        # self.phx_lib.PHX_ErrHandlerDefault.restype = None
        # self.phx_lib.PHX_ErrHandlerDefault.argtypes = [ctypes.c_char_p, etStat, ctypes.c_char_p]
        #
        # self.phx_lib.PHX_ErrCodeDecode.restype = None
        # self.phx_lib.PHX_ErrCodeDecode.argtypes = [ctypes.c_char_p, etStat]
        #
        # self.phx_lib.PHX_DebugDefaultTraceHandler.restype = None
        # self.phx_lib.PHX_DebugDefaultTraceHandler.argtypes = [ctypes.POINTER(ui8)]
        #
        # self.phx_lib.PHX_MutexCreate.restype = etStat
        # self.phx_lib.PHX_MutexCreate.argtypes = [tHandle, ctypes.POINTER(tPHX), ctypes.c_char_p]
        #
        # self.phx_lib.PHX_MutexDestroy.restype = etStat
        # self.phx_lib.PHX_MutexDestroy.argtypes = [tHandle, ctypes.POINTER(tPHX)]
        #
        # self.phx_lib.PHX_MutexAcquire.restype = etStat
        # self.phx_lib.PHX_MutexAcquire.argtypes = [tHandle, tPHX, ui32]
        #
        # self.phx_lib.PHX_MutexRelease.restype = etStat
        # self.phx_lib.PHX_MutexRelease.argtypes = [tHandle, tPHX]
        #
        # self.phx_lib.PHX_SemaphoreCreate.restype = etStat
        # self.phx_lib.PHX_SemaphoreCreate.argtypes = [tHandle, ctypes.POINTER(tPHX), ui32, ui32]
        #
        # self.phx_lib.PHX_SemaphoreDestroy.restype = etStat
        # self.phx_lib.PHX_SemaphoreDestroy.argtypes = [tHandle, ctypes.POINTER(tPHX)]
        #
        # self.phx_lib.PHX_SemaphoreSignal.restype = etStat
        # self.phx_lib.PHX_SemaphoreSignal.argtypes = [tHandle, tPHX, ui32]
        #
        # self.phx_lib.PHX_SemaphoreWaitWithTimeout.restype = etStat
        # self.phx_lib.PHX_SemaphoreWaitWithTimeout.argtypes = [tHandle, tPHX, ui32]
        #
        # self.phx_lib.PHX_ComParameterGet.restype = etStat
        # self.phx_lib.PHX_ComParameterGet.argtypes = [ui32, etComParam, ctypes.c_void_p]
        #
        # self.phx_lib.PHX_MemoryAlloc.restype = etStat
        # self.phx_lib.PHX_MemoryAlloc.argtypes = [tHandle, ctypes.POINTER(ctypes.c_void_p), ui32, ui32, ui32]
        #
        # self.phx_lib.PHX_MemoryFreeAndNull.restype = None
        # self.phx_lib.PHX_MemoryFreeAndNull.argtypes = [tHandle, ctypes.POINTER(ctypes.c_void_p)]



# Example usage of the etFctErrorHandler in Python
# def example_error_handler(source, status, message):
#     print(f"Error from {source}: {status} - {message}")
#
# error_handler = etFctErrorHandler(example_error_handler)
if __name__ == '__main__':
   print(etParam.PHX_ACQ_CONTINUOUS)
   print(etParamValue.PHX_ACQ_LINE_DOUBLE_2)
   print(etParamValue.PHX_USR_FORMAT_BAYER_BG14P)
   print(etParamValue.PHX_CAMTRIG_SRC_TTLIN_CH4_0)
   print(etParamValue.PHX_TIMERA1_TRIG_SRC_24VIN_CH8_0_FALL)
   print(etParamValue.PHX_CXP_BITRATE_MODE_CXP10)
   print(etParamValue.PHX_CXP_DISCOVERY_MODE_4X)
   print(etParamValue.PHX_INTRPT_CAMERA_TRIGGER)
   print(etAcq.PHX_CXP_ERRCOUNT_CRC_EVENT_RST)

   # Example usage
   # Replace 'example_handle' and other variables with actual values appropriate for your context.
   directory = "C:/Users/Public/Documents/Active Silicon/ActiveSDK v01.08.12/examples/build/windows/output/x64_Release"
   dll_path = os.path.abspath(os.path.join(directory, 'phx_live_dll_2.dll'))
   mydll = PyPHX(libpath = dll_path)
   example_handle = tHandle()
   error_handler = etFctErrorHandler(lambda x, y, z: print(f"Error: {x}, Status: {y}, Msg: {z}"))

   status = mydll.Create(ctypes.byref(example_handle), error_handler)
   print(f"PHX_Create Status: {status}")
