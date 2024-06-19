import os
from PyPHX import *

# Load the DLL


localpath= os.path.dirname(os.path.realpath(__file__))
# Load the DLL
dll_path = os.path.abspath(os.path.join(localpath,'..','build','x64_Release', 'phx.dll'))
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

phxdll.Initphxlive.argtypes = [tPhxCmd, tCxpRegisters]
phxdll.Initphxlive.restype = ctypes.c_int


def main():
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