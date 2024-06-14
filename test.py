import ctypes
import os

# Load the DLL
directory="C:/Users/Public/Documents/Active Silicon/ActiveSDK v01.08.12/examples/build/windows/output/x64_Release"
dll_path = os.path.abspath(os.path.join(directory,'phx_live_dll.dll'))
phxdll = ctypes.CDLL(dll_path)

#define the data structures
etParamValue = ctypes.c_int  # Assuming etParamValue is an enum represented as an int
tFlag = ctypes.c_int  # Assuming tFlag is represented as an int

# tHandle
# Define the integer types based on the header file
i8 = ctypes.c_int8
ui8 = ctypes.c_uint8
i16 = ctypes.c_int16
ui16 = ctypes.c_uint16
i32 = ctypes.c_int32
ui32 = ctypes.c_uint32
i64 = ctypes.c_int64
ui64 = ctypes.c_uint64

# Define m32 as signed 32-bit integer
m32 = ctypes.c_int32

# Define tHandle as m32
tHandle = m32

# Define etParam


PHX_MAX_FILE_LENGTH = 128
# Define the tPhxCmd structure
class tPhxCmd(ctypes.Structure):
    _fields_ = [
        ("dwBoardNumber", ctypes.c_uint32),
        ("eBoardNumber", etParamValue),
        ("dwChannelNumber", ctypes.c_uint32),
        ("eChannelNumber", etParamValue),
        ("bConfigFileName", ctypes.c_char * PHX_MAX_FILE_LENGTH),
        ("bOutputFileName", ctypes.c_char * PHX_MAX_FILE_LENGTH),
        ("pszConfigFileName", ctypes.c_char_p),
        ("pszOutputFileName", ctypes.c_char_p),
        ("dwRecordingNumBuffers", ctypes.c_uint32),
        ("dwRecordingFrameRate", ctypes.c_uint32),
        ("dwRecordingQualityFactor", ctypes.c_uint32),
        ("bRecordingFormat", ctypes.c_char * PHX_MAX_FILE_LENGTH),
        ("pszRecordingFormat", ctypes.c_char_p),
        ("dwCxpBitRate", ctypes.c_uint32),
        ("eCxpBitRate", etParamValue),
        ("dwCxpLinks", ctypes.c_uint32),
        ("eCxpLinks", etParamValue),
        ("dwConfigMode", ctypes.c_uint32),
        ("eConfigMode", etParamValue),
        ("fWaitForKill", tFlag)
    ]

# Define the tCxpRegisters structure
class tCxpRegisters(ctypes.Structure):
    _fields_ = [
        ("dwAcqStartAddress", ctypes.c_uint32),
        ("dwAcqStartValue", ctypes.c_uint32),
        ("dwAcqStopAddress", ctypes.c_uint32),
        ("dwAcqStopValue", ctypes.c_uint32),
        ("dwPixelFormatAddress", ctypes.c_uint32)
    ]

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

phxdll.Initphxlive.argtypes = [tPhxCmd, tCxpRegisters]
phxdll.Initphxlive.restype = ctypes.c_int


def main():
    import sys
    # argc = len(sys.argv)
    #
    # argv = (ctypes.c_char_p * argc)(*map(lambda arg: ctypes.create_string_buffer(arg.encode('utf-8')), sys.argv))

    sPhxCmd = phxdll.InitPhxCmd()
    sCameraRegs = phxdll.InitCameraRegs()
    nStatus = phxdll.InitMemory(sPhxCmd, sCameraRegs)

    phxdll.InitPhxCommonKbInit()
    nStatus = phxdll.Initphxlive(sPhxCmd, sCameraRegs)
    phxdll.InitPhxCommonKbClose()

    return nStatus

if __name__ == "__main__":
    main()