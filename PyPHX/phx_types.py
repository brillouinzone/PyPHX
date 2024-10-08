from ctypes import c_int, Structure, c_void_p, c_uint64, c_uint32, c_ulonglong, POINTER, c_uint8, c_uint16, CFUNCTYPE, c_char_p
import ctypes
# /* API & Library function parameter definitions
#  * ============================================
# _FnTypes()
# */

# Define the custom types
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

tHandle = mu32
tFlag = ui32
tPhxFnName = ctypes.c_char_p
tPhxErrStr = ctypes.c_char_p
tPHX = ctypes.c_void_p
tPhxPhysAddr = ui64
tPhxPhysLen = ui64

etParamValue = ctypes.c_int  # Assuming etParamValue is an enum represented as an int
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
# Define an error handler function in Python
def error_handler_function(error_msg, status, additional_msg):
    print(f"Error: {error_msg.decode()}, Status: {status}, Additional: {additional_msg.decode()}")

class tPhxLive(ctypes.Structure):
    _fields_ = [
        ("dwBufferReadyCount", ui32),
        ("fBufferReady", tFlag),
        ("fFifoOverflow", tFlag)
    ]

class etFnTypes:
    FNTYPE_EMASK = c_int(0xF0000000).value

    FNTYPE_PHA_API = c_int(0x00000000).value
    FNTYPE_PHA_LIB = c_int(0x10000000).value
    FNTYPE_PHC_API = c_int(0x20000000).value
    FNTYPE_PHC_LIB = c_int(0x30000000).value
    FNTYPE_PHD_API = c_int(0x40000000).value
    FNTYPE_PHD_LIB = c_int(0x50000000).value
    FNTYPE_PDL_API = c_int(0x60000000).value
    FNTYPE_PDL_LIB = c_int(0x70000000).value
    FNTYPE_PCC_API = c_int(0x80000000).value
    FNTYPE_PCC_LIB = c_int(0x90000000).value

    FNTYPE_PHX_API = c_int(0xC0000000).value
    FNTYPE_PHX_LIB = c_int(0xD0000000).value

# /* PHX_Function Definitions
#  * ========================
#  * These enums are used as magic numbers for all parameters passed to the specific functions,
#  * ie any parameter passed to the Acquisition functions will have the top 8 bits set to 3.
#  * This is used to confirm that the parameter passed is valid for the function.
#  */

class etPhxFn:
    PHX_EMASK_FN = c_int(etFnTypes.FNTYPE_EMASK | 0x000F0000).value
    PHX_CAMERACONFIG_LOAD = c_int(etFnTypes.FNTYPE_PHX_API | 0x00010000).value
    PHX_SETANDGET = c_int(etFnTypes.FNTYPE_PHX_API | 0x00020000).value
    PHX_ACQUIRE = c_int(etFnTypes.FNTYPE_PHX_API | 0x00030000).value
    PHX_ACTION = c_int(etFnTypes.FNTYPE_PHX_API | 0x00040000).value
    PHX_TEST = c_int(etFnTypes.FNTYPE_PHX_API | 0x00050000).value
    PHX_COM = c_int(etFnTypes.FNTYPE_PHX_API | 0x00060000).value
    PHX_PRE_OPEN = c_int(etFnTypes.FNTYPE_PHX_API | 0x00070000).value
    PHX_CONTROL = c_int(etFnTypes.FNTYPE_PHX_API | 0x00080000).value
    PHX_FBD_SETANDGET = c_int(etFnTypes.FNTYPE_PHX_API | 0x00090000).value
    PHX_CAN_SETANDGET = c_int(etFnTypes.FNTYPE_PHX_API | 0x000A0000).value
    PHX_PRIVATE_SETANDGET = c_int(etFnTypes.FNTYPE_PHX_API | 0x000B0000).value

    # For backwards compatibility
    PHX_CAMERACONFIG_SAVE = PHX_ACTION

# /* PHX_CameraConfigLoad Definitions
#  * ================================
#  */

class etCamConfigLoad:
    PHX_EMASK_BOARD = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x0007).value
    PHX_BOARD_AUTO = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x0000).value
    PHX_BOARD1 = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x0001).value
    PHX_BOARD2 = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x0002).value
    PHX_BOARD3 = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x0003).value
    PHX_BOARD4 = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x0004).value
    PHX_BOARD5 = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x0005).value
    PHX_BOARD6 = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x0006).value
    PHX_BOARD7 = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x0007).value
    PHX_BOARD_MAX = c_int(7).value

    PHX_EMASK_CHANNEL = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x0030).value
    PHX_CHANNEL_AUTO = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x0000).value
    PHX_CHANNEL_A = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x0010).value
    PHX_CHANNEL_B = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x0020).value
    PHX_CHANNEL_MAX = c_int(2).value
    PHX_CHANNEL_ONLY = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x0040).value

    PHX_EMASK_MODE = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x0280).value
    PHX_MODE_NORMAL = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x0000).value
    PHX_COMMS_ONLY = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x0080).value
    PHX_ACQ_ONLY = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x0200).value

    PHX_EMASK_UPDATE = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x0100).value
    PHX_UPDATE_FIRMWARE = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x0100).value
    PHX_NO_RECONFIGURE = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x0400).value

    PHX_EMASK_TYPE = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0xC000).value
    PHX_EMASK_VARIANT = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x3808).value
    PHX_TYPE_AUTO = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x0000).value
    PHX_DIGITAL = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x4000).value

    PHX_D24CL_PCI32 = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x4800).value
    PHX_D24CL_PE1 = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x4808).value
    PHX_D24AVDS_PE1 = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x5008).value

    PHX_D32_PCI32 = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x5000).value

    PHX_D36_PCI32 = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x5800).value
    PHX_D36_PCI64 = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x6000).value
    PHX_D36_PCI64U = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x6800).value
    PHX_D36_PE1 = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x5808).value

    PHX_D10HDSDI_PE1 = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x6808).value
    PHX_D20HDSDI_PE1 = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x6008).value

    PHX_D48CL_PCI32 = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x4008).value
    PHX_D48CL_PCI64 = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x7000).value
    PHX_D24CL_PE1_MIR = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x7008).value
    PHX_D48CL_PCI64U = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x7800).value
    PHX_D48CL_PE1 = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x7808).value

    PHX_ANALOGUE = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0x8000).value

    PHX_DIGITAL2 = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0xC000).value
    PHX_D36_PE4 = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0xD808).value
    PHX_D10HDSDI_PE4 = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0xE808).value
    PHX_D20HDSDI_PE4 = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0xE008).value
    PHX_D48CL_PE4 = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0xF008).value
    PHX_D64CL_PE4 = c_int(etPhxFn.PHX_CAMERACONFIG_LOAD | 0xD008).value

    # For backwards compatibility
    PHX_CHANNEL1 = PHX_CHANNEL_A
    PHX_CHANNEL2 = PHX_CHANNEL_B


# /* PHX_Action Definitions
#  * ======================
#  */
class etActionParam:
    PHX_EMASK_SAVE = c_int(etPhxFn.PHX_ACTION | 0x0007).value
    PHX_SAVE_CAM = c_int(etPhxFn.PHX_ACTION | 0x0001).value
    PHX_SAVE_SYS = c_int(etPhxFn.PHX_ACTION | 0x0002).value
    PHX_SAVE_APP = c_int(etPhxFn.PHX_ACTION | 0x0004).value
    PHX_SAVE_ALL = c_int(etPhxFn.PHX_ACTION | 0x0007).value

# For backwards compatibility
etCamConfigSave = etActionParam


# /* Expansion Macros
#  * ===============================
#  */
# #define _PHX_ENUM(_e,_x,_v)   _x = (int) ((ui32) _e | ((ui32) _v << 8 ))

# Define the _PHX_ENUM function
def _PHX_ENUM(_e, _v):
    return c_int(c_uint32(_e).value | (c_uint32(_v).value << 8)).value

class etParam:
    PHX_PARAM_MASK = c_int(0xffffff00).value
    PHX_INVALID_PARAM = 0
    PHX_CACHE_FLUSH = 1
    PHX_FORCE_REWRITE = 2

    PHX_ACQ_CONTINUOUS = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 1)
    PHX_ACQ_NUM_BUFFERS = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 2)
    PHX_ACQ_SKIP = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 3)
    PHX_FGTRIG_SRC = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 4)
    PHX_FGTRIG_MODE = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 5)
    PHX_ACQ_FIELD_MODE = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 6)
    PHX_ACQ_XSUB = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 7)
    PHX_ACQ_YSUB = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 8)

    PHX_CAM_ACTIVE_XLENGTH = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 10)
    PHX_CAM_ACTIVE_YLENGTH = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 11)
    PHX_CAM_ACTIVE_XOFFSET = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 12)
    PHX_CAM_ACTIVE_YOFFSET = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 13)
    PHX_CAM_CLOCK_POLARITY = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 14)
    PHX_CAM_FORMAT = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 15)
    PHX_CAM_NUM_TAPS = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 16)
    PHX_CAM_SRC_DEPTH = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 17)
    PHX_CAM_SRC_COL = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 18)
    PHX_CAM_HTAP_DIR = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 19)
    PHX_CAM_HTAP_TYPE = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 20)
    PHX_CAM_HTAP_NUM = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 21)
    PHX_CAM_VTAP_DIR = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 22)
    PHX_CAM_VTAP_TYPE = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 23)
    PHX_CAM_VTAP_NUM = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 24)
    PHX_CAM_TYPE = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 25)
    PHX_CAM_XBINNING = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 26)
    PHX_CAM_YBINNING = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 27)
    PHX_COMMS_DATA = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 28)
    PHX_COMMS_FLOW = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 29)
    PHX_COMMS_INCOMING = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 30)
    PHX_COMMS_OUTGOING = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 31)
    PHX_COMMS_PARITY = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 32)
    PHX_COMMS_SPEED = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 33)
    PHX_COMMS_STANDARD = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 34)
    PHX_COMMS_STOP = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 35)
    PHX_DATASRC = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 36)
    PHX_DATASTREAM_VALID = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 37)
    PHX_BUS_FORMAT = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 38)
    PHX_DST_PTR_TYPE = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 39)
    PHX_DST_PTRS_VIRT = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 40)
    PHX_DUMMY_PARAM = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 41)
    PHX_ERROR_FIRST_ERRNUM = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 42)
    PHX_ERROR_FIRST_ERRSTRING = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 43)
    PHX_ERROR_HANDLER = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 44)
    PHX_ERROR_LAST_ERRNUM = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 45)
    PHX_ERROR_LAST_ERRSTRING = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 46)
    PHX_COUNTE1_VALUE_NOW = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 47)

    PHX_NUM_BOARDS = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 61)
    PHX_IO_CCIO_A = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 62)  # Absolute
    PHX_IO_CCIO_A_OUT = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 63)
    PHX_IO_CCIO_B = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 64)
    PHX_IO_CCIO_B_OUT = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 65)

    PHX_IO_OPTO_SET = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 68)
    PHX_IO_OPTO_CLR = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 69)
    PHX_IO_OPTO = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 70)
    PHX_IO_TTL_A = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 71)
    PHX_IO_TTL_A_OUT = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 72)
    PHX_IO_TTL_B = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 73)
    PHX_IO_TTL_B_OUT = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 74)
    PHX_TIMEOUT_CAPTURE = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 75)
    PHX_TIMEOUT_DMA = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 76)
    PHX_TIMEOUT_TRIGGER = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 77)
    PHX_INTRPT_SET = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 78)
    PHX_INTRPT_CLR = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 79)
    PHX_REV_HW = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 80)
    PHX_REV_HW_MAJOR = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 81)
    PHX_REV_HW_MINOR = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 82)
    PHX_REV_SW = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 83)
    PHX_REV_SW_MAJOR = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 84)
    PHX_REV_SW_MINOR = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 85)
    PHX_REV_SW_SUBMINOR = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 86)
    PHX_ROI_DST_XOFFSET = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 87)
    PHX_ROI_DST_YOFFSET = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 88)
    PHX_ROI_SRC_XOFFSET = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 89)
    PHX_ROI_SRC_YOFFSET = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 90)
    PHX_ROI_XLENGTH = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 91)
    PHX_ROI_YLENGTH = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 92)
    PHX_BUF_DST_XLENGTH = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 93)
    PHX_BUF_DST_YLENGTH = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 94)
    PHX_ACQ_STATUS = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 95)

    PHX_BOARD_PROPERTIES = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 102)
    PHX_ROI_XLENGTH_SCALED = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 103)
    PHX_ROI_YLENGTH_SCALED = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 104)
    PHX_BUF_SET = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 105)
    PHX_BUF_SET_COLOUR = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 106)
    PHX_LUT_COUNT = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 107)
    PHX_LUT_INFO = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 108)
    PHX_REV_HW_SUBMINOR = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 109)
    PHX_LUT_CORRECT = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 110)
    PHX_LINETRIG_SRC = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 111)
    PHX_LINETRIG_TIMER_CTRL = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 112)
    PHX_TIMERA1_PERIOD = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 113)
    PHX_CAMTRIG_SRC = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 114)
    PHX_EXP_CTRLIO_1 = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 115)
    PHX_TIMERM1_WIDTH = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 116)
    PHX_EXP_CTRLIO_2 = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 117)
    PHX_TIMERM2_WIDTH = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 118)
    PHX_EXP_LINESTART = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 119)
    PHX_FGTRIG_DELAY_MODE = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 120)
    PHX_TIMERD1_VALUE = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 121)
    PHX_EVENTCOUNT_SRC = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 122)
    PHX_EVENTGATE_SRC = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 123)
    PHX_CAM_HTAP_ORDER = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 124)
    PHX_CAM_VTAP_ORDER = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 125)
    PHX_EVENT_CONTEXT = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 126)
    PHX_CAM_DATA_VALID = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 127)
    PHX_COUNT_BUFFER_READY = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 128)
    PHX_COUNT_BUFFER_READY_NOW = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 129)
    PHX_BIT_SHIFT = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 130)
    PHX_IO_CCIO = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 131)  # Relative
    PHX_IO_CCIO_OUT = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 132)
    PHX_IO_TTL = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 133)  # Relative
    PHX_IO_TTL_OUT = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 134)
    PHX_IO_OPTO_A = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 135)  # Absolute
    PHX_IO_OPTO_B = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 136)
    PHX_IO_TIMER_A1_PERIOD = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 137)  # Absolute
    PHX_IO_TIMER_A2_PERIOD = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 138)
    PHX_IO_TIMER_B1_PERIOD = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 139)  # Absolute
    PHX_IO_TIMER_B2_PERIOD = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 140)
    PHX_IO_OPTO_OUT = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 141)
    PHX_IO_OPTO_A_OUT = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 142)
    PHX_IO_OPTO_B_OUT = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 143)
    PHX_FGTRIG_ALIGN = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 144)
    PHX_DST_ENDIAN = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 145)
    PHX_ACQ_CHAIN = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 146)
    PHX_ACQ_BLOCKING = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 147)
    PHX_DST_PTRS_PHYS32 = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 148)
    PHX_BOARD_INFO = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 149)
    PHX_DATARATE_TEST = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 150)
    PHX_CAM_CLOCK_MAX = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 151)
    PHX_COUNTE1_VALUE_DB = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 152)
    PHX_CHAN_SYNC_MODE = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 153)
    PHX_ACQ_BUFFER_START = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 154)
    PHX_LUT_BYPASS = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 155)
    PHX_COMMS_PORT_NAME = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 156)
    PHX_CVB_PARAM = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 157)
    PHX_USR_FORMAT = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 158)
    PHX_ACQ_AUTO_RESTART = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 159)
    PHX_ACQ_HSCALE = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 160)
    PHX_MERGE_CHAN = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 161)
    PHX_MERGE_INTRPT_SET = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 162)
    PHX_MERGE_INTRPT_CLR = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 163)
    PHX_CLSER_INDEX = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 164)
    PHX_FIFO_BUFFER_COUNT = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 165)
    PHX_REV_FW_FLASH = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 166)
    PHX_REV_FW_LIB = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 167)
    PHX_PCIE_INFO = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 168)
    PHX_TAP_MODE = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 169)
    PHX_ACQ_IMAGES_PER_BUFFER = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 170)
    PHX_DST_PTRS_PHYS64 = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 171)
    PHX_CAM_VTAP_INTERLEAVE_NUM = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 172)
    PHX_REV_DRV = _PHX_ENUM(etPhxFn.PHX_SETANDGET, 173)

    PHX_IO_CCOUT_A = PHX_IO_CCIO_A
    PHX_IO_CCOUT_B = PHX_IO_CCIO_B
    PHX_IO_CCOUT = PHX_IO_CCIO
    PHX_MASK_CCIO = c_int(0x00000003).value
    PHX_MASK_CCOUT = c_int(0x0000000f).value
    PHX_MASK_OPTO = c_int(0x0000000f).value

    PHX_CONFIG_FILE = _PHX_ENUM(etPhxFn.PHX_PRE_OPEN, 1)
    PHX_BOARD_VARIANT = _PHX_ENUM(etPhxFn.PHX_PRE_OPEN, 2)
    PHX_BOARD_NUMBER = _PHX_ENUM(etPhxFn.PHX_PRE_OPEN, 3)
    PHX_CHANNEL_NUMBER = _PHX_ENUM(etPhxFn.PHX_PRE_OPEN, 4)
    PHX_CONFIG_MODE = _PHX_ENUM(etPhxFn.PHX_PRE_OPEN, 5)
    PHX_CONFIG_FLUSH = _PHX_ENUM(etPhxFn.PHX_PRE_OPEN, 6)
    PHX_BOARD_BUS_ADDRESS = _PHX_ENUM(etPhxFn.PHX_PRE_OPEN, 7)

    PHX_CXP_INFO = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 1)
    PHX_CXP_BITRATE = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 2)
    PHX_CXP_POCXP = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 3)
    PHX_CXP_DISCOVERY = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 4)
    PHX_CXP_LINKTEST_UPLINK_OK = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 5)
    PHX_CXP_LINKTEST_UPLINK_ERR = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 6)
    PHX_CXP_LINKTEST_DOWNLINK_OK = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 7)
    PHX_CXP_LINKTEST_DOWNLINK_ERR = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 8)
    PHX_CXP_ERRCOUNT_LINK_LOSS = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 9)
    PHX_CXP_ERRCOUNT_8B10B = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 10)
    PHX_CXP_ERRCOUNT_CRC_STREAM = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 11)
    PHX_CXP_ERRCOUNT_CRC_CTRL = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 12)
    PHX_CXP_ERRCOUNT_CRC_EVENT = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 13)
    PHX_CAMERA_POWER = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 15)
    PHX_CXP_BITRATE_MODE = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 16)
    PHX_CXP_POCXP_MODE = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 17)
    PHX_CXP_DISCOVERY_MODE = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 18)
    PHX_CXP_UPLINK_TEST = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 19)
    PHX_CXP_DOWNLINK_TEST = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 20)
    PHX_IO_422IN_CHX = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 21)
    PHX_IO_422IN_CH1 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 22)
    PHX_IO_422IN_CH2 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 23)
    PHX_IO_422IN_CH3 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 24)
    PHX_IO_422IN_CH4 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 25)
    PHX_IO_OPTOIN_CHX = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 26)
    PHX_IO_OPTOIN_CH1 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 27)
    PHX_IO_OPTOIN_CH2 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 28)
    PHX_IO_OPTOIN_CH3 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 29)
    PHX_IO_OPTOIN_CH4 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 30)
    PHX_IO_TTLIN_CHX = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 31)
    PHX_IO_TTLIN_CH1 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 32)
    PHX_IO_TTLIN_CH2 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 33)
    PHX_IO_TTLIN_CH3 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 34)
    PHX_IO_TTLIN_CH4 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 35)
    PHX_IO_422OUT_CHX = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 36)
    PHX_IO_422OUT_CH1 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 37)
    PHX_IO_422OUT_CH2 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 38)
    PHX_IO_422OUT_CH3 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 39)
    PHX_IO_422OUT_CH4 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 40)
    PHX_IO_OPTOOUT_CHX = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 41)
    PHX_IO_OPTOOUT_CH1 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 42)
    PHX_IO_OPTOOUT_CH2 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 43)
    PHX_IO_OPTOOUT_CH3 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 44)
    PHX_IO_OPTOOUT_CH4 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 45)
    PHX_IO_TTLOUT_CHX = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 46)
    PHX_IO_TTLOUT_CH1 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 47)
    PHX_IO_TTLOUT_CH2 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 48)
    PHX_IO_TTLOUT_CH3 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 49)
    PHX_IO_TTLOUT_CH4 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 50)
    PHX_IO_CCOUT_CHX = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 51)
    PHX_IO_CCOUT_CH1 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 52)
    PHX_IO_CCOUT_CH2 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 53)
    PHX_IO_CCOUT_CH3 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 54)
    PHX_IO_CCOUT_CH4 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 55)
    PHX_CAMTRIG_ENCODER_MODE = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 56)
    PHX_CAMTRIG_ENCODER_SRC = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 57)
    PHX_CAMTRIG_MULTIPLIER = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 58)
    PHX_CAMTRIG_DIVIDER = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 59)
    PHX_CAMTRIG_CXPTRIG_SRC = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 60)
    PHX_CAMTRIG_CXPTRIG_MODE = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 61)
    PHX_CAMTRIG_DELAY_MODE = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 62)
    PHX_TIMERD2_VALUE = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 63)
    PHX_FW_NUM_DESIGNS = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 64)
    PHX_FW_DESIGN_LIB = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 65)
    PHX_FW_DESIGN_PROG = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 66)
    PHX_FW_DESIGN_FLASH = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 67)
    PHX_STR_DESIGN_LIB = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 68)
    PHX_STR_DESIGN_FLASH = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 69)
    PHX_FGTRIG_FILTER_NS = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 70)
    PHX_CAMTRIG_FILTER_NS = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 71)
    PHX_CAMTRIG_ENCODER_FILTER_NS = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 72)
    PHX_TIMERA1_PERIOD_NS = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 73)
    PHX_TIMERD1FG_VALUE_NS = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 74)
    PHX_TIMERM1_WIDTH_NS = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 75)
    PHX_TIMERM2_WIDTH_NS = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 76)
    PHX_TIMERD2_VALUE_NS = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 77)
    PHX_FPGA_CORE_TEMP = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 78)
    PHX_CAMTRIG_DELAY_MODE_D1CAM = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 79)
    PHX_CAMTRIG_DELAY_MODE_D2CAM = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 80)
    PHX_TIMERD1FG_COUNT = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 81)
    PHX_TIMERA1_MODE = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 82)
    PHX_TIMERA1_PULSE_COUNT = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 83)
    PHX_TIMERA1_SWTRIG = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 84)
    PHX_TIMERD1CAM_VALUE_NS = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 85)
    PHX_TIMERD2CAM_VALUE_NS = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 86)
    PHX_IMAGE_TIMESTAMP_MODE = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 87)
    PHX_DST_ALIGNMENT = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 88)
    PHX_CXP_HEADER = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 90)
    PHX_IRIG_TRIG_COUNT = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 91)
    PHX_IO_24VOUT_CHX = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 92)
    PHX_IO_24VOUT_CH1 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 93)
    PHX_IO_24VOUT_CH2 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 94)
    PHX_IO_24VOUT_CH3 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 95)
    PHX_IO_24VOUT_CH4 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 96)
    PHX_IO_24VIN_CHX = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 97)
    PHX_IO_24VIN_CH1 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 98)
    PHX_IO_24VIN_CH2 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 99)
    PHX_IO_24VIN_CH3 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 100)
    PHX_IO_24VIN_CH4 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 101)
    PHX_IO_24VIN_CH5 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 102)
    PHX_IO_24VIN_CH6 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 103)
    PHX_IO_24VIN_CH7 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 104)
    PHX_IO_24VIN_CH8 = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 105)
    PHX_SERIAL_NUMBER = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 106)
    PHX_RETAIN_CXP_DISCOVERY = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 107)
    PHX_CL_LVAL_FILTER = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 108)
    PHX_CXP_ERRCOUNT_DUP_FIXED = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 109)
    PHX_CXP_ERRCOUNT_DUP_NFIXED = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 110)
    PHX_CXP_ERRCOUNT_FEC_FIXED = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 111)
    PHX_CXP_ERRCOUNT_FEC_NFIXED = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 112)
    PHX_ACQ_BUFFER_MEMSET_MODE = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 113)
    PHX_ACQ_BUFFER_MEMSET_VALUE = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 114)
    PHX_CL_POCL_MODE = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 115)
    PHX_TIMERA1_TRIG_SRC = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 116)
    PHX_TIMERA1_TRIG_FILTER_NS = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 117)
    PHX_MEZZANINE_GUID = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 118)
    PHX_USER_ID = _PHX_ENUM(etPhxFn.PHX_FBD_SETANDGET, 119)

    PHX_CL_INFO = PHX_CXP_INFO

# recall
# def _PHX_ENUM(_e, _v):
#     return c_int(c_uint32(_e).value | (c_uint32(_v).value << 8)).value


# etAction enum
class etAction:
    PHX_CONFIG_SAVE = _PHX_ENUM(etPhxFn.PHX_ACTION, 1)
    PHX_FIRMWARE_PROG = _PHX_ENUM(etPhxFn.PHX_ACTION, 2)

# etControlPort enum
class etControlPort:
    PHX_COMMS_PORT = _PHX_ENUM(etPhxFn.PHX_CONTROL, 0x0000)
    PHX_REGISTER_HOST = _PHX_ENUM(etPhxFn.PHX_CONTROL, 0x0001)
    PHX_REGISTER_DEVICE = _PHX_ENUM(etPhxFn.PHX_CONTROL, 0x0002)

# etComParam enum
class etComParam:
    PHX_COM_NUM_PORTS = _PHX_ENUM(etPhxFn.PHX_COM, 0x0000)
    PHX_COM_CLSER_CONFIG = _PHX_ENUM(etPhxFn.PHX_COM, 0x0001)
    PHX_COM_WIN_CONFIG = _PHX_ENUM(etPhxFn.PHX_COM, 0x0002)
    PHX_COM_GET_HANDLE = _PHX_ENUM(etPhxFn.PHX_COM, 0x0003)
    PHX_COM_GET_BOARD_CHAN_INDEX = _PHX_ENUM(etPhxFn.PHX_COM, 0x0004)

class etParamValue:
    PHX_INVALID_PARAMVALUE = 0

    # Boolean flags
    PHX_ENABLE = 1
    PHX_DISABLE = 2

    # PHX_COMMS_DATA
    PHX_COMMS_DATA_5 = c_int(c_uint32(etParam.PHX_COMMS_DATA).value + 1).value
    PHX_COMMS_DATA_6 = c_int(c_uint32(etParam.PHX_COMMS_DATA).value + 2).value
    PHX_COMMS_DATA_7 = c_int(c_uint32(etParam.PHX_COMMS_DATA).value + 3).value
    PHX_COMMS_DATA_8 = c_int(c_uint32(etParam.PHX_COMMS_DATA).value + 4).value

    # PHX_COMMS_STOP
    PHX_COMMS_STOP_1 = c_int(c_uint32(etParam.PHX_COMMS_STOP).value + 1).value
    PHX_COMMS_STOP_1_5 = c_int(c_uint32(etParam.PHX_COMMS_STOP).value + 2).value
    PHX_COMMS_STOP_2 = c_int(c_uint32(etParam.PHX_COMMS_STOP).value + 3).value

    # PHX_COMMS_PARITY
    PHX_COMMS_PARITY_NONE = c_int(c_uint32(etParam.PHX_COMMS_PARITY).value + 1).value
    PHX_COMMS_PARITY_EVEN = c_int(c_uint32(etParam.PHX_COMMS_PARITY).value + 2).value
    PHX_COMMS_PARITY_ODD = c_int(c_uint32(etParam.PHX_COMMS_PARITY).value + 3).value

    # PHX_COMMS_FLOW
    PHX_COMMS_FLOW_NONE = c_int(c_uint32(etParam.PHX_COMMS_FLOW).value + 1).value
    PHX_COMMS_FLOW_HW = c_int(c_uint32(etParam.PHX_COMMS_FLOW).value + 2).value
    PHX_COMMS_FLOW_SW = c_int(c_uint32(etParam.PHX_COMMS_FLOW).value + 3).value

    # PHX_COMMS_STANDARD
    PHX_COMMS_STANDARD_RS232 = c_int(c_uint32(etParam.PHX_COMMS_STANDARD).value + 1).value
    PHX_COMMS_STANDARD_LVDS = c_int(c_uint32(etParam.PHX_COMMS_STANDARD).value + 2).value

    # PHX_IO_OPTO_OUT_SET/CLR
    PHX_IO_OPTO_OUT1 = 0x00000001
    PHX_IO_OPTO_OUT2 = 0x00000002
    PHX_IO_OPTO_OUT3 = 0x00000004
    PHX_IO_OPTO_OUT4 = 0x00000008

    # PHX_IO_OPTO_SET/CLR
    PHX_IO_OPTO1 = 0x00000001
    PHX_IO_OPTO2 = 0x00000002
    PHX_IO_OPTO3 = 0x00000004
    PHX_IO_OPTO4 = 0x00000008

    # PHX_ACQ_STATUS
    PHX_ACQ_STATUS_IDLE = c_int(c_uint32(etParam.PHX_ACQ_STATUS).value + 1).value
    PHX_ACQ_STATUS_IN_PROGRESS = c_int(c_uint32(etParam.PHX_ACQ_STATUS).value + 2).value
    PHX_ACQ_STATUS_WAITING_FOR_TRIGGER = c_int(c_uint32(etParam.PHX_ACQ_STATUS).value + 3).value

    # PHX_CAM_TYPE
    PHX_CAM_LINESCAN_ROI = c_int(c_uint32(etParam.PHX_CAM_TYPE).value + 1).value
    PHX_CAM_LINESCAN_NO_ROI = c_int(c_uint32(etParam.PHX_CAM_TYPE).value + 2).value
    PHX_CAM_AREASCAN_ROI = c_int(c_uint32(etParam.PHX_CAM_TYPE).value + 3).value
    PHX_CAM_AREASCAN_NO_ROI = c_int(c_uint32(etParam.PHX_CAM_TYPE).value + 4).value
    PHX_CAM_MULTILINESCAN_ROI = c_int(c_uint32(etParam.PHX_CAM_TYPE).value + 5).value

    # PHX_CAM_FORMAT
    PHX_CAM_INTERLACED = c_int(c_uint32(etParam.PHX_CAM_FORMAT).value + 1).value
    PHX_CAM_NON_INTERLACED = c_int(c_uint32(etParam.PHX_CAM_FORMAT).value + 2).value

    # PHX_CAM_SRC_COL
    PHX_CAM_SRC_MONO = 0x00000001
    PHX_CAM_SRC_RGB = 0x00000003
    PHX_CAM_SRC_BAYER_RG = c_int(c_uint32(etParam.PHX_CAM_SRC_COL).value + 1).value
    PHX_CAM_SRC_BAYER_GR = c_int(c_uint32(etParam.PHX_CAM_SRC_COL).value + 2).value
    PHX_CAM_SRC_BAYER_GB = c_int(c_uint32(etParam.PHX_CAM_SRC_COL).value + 3).value
    PHX_CAM_SRC_BAYER_BG = c_int(c_uint32(etParam.PHX_CAM_SRC_COL).value + 4).value
    PHX_CAM_SRC_YUV422 = c_int(c_uint32(etParam.PHX_CAM_SRC_COL).value + 5).value
    PHX_CAM_SRC_RGBA = c_int(c_uint32(etParam.PHX_CAM_SRC_COL).value + 6).value
    PHX_CAM_SRC_RGBM = c_int(c_uint32(etParam.PHX_CAM_SRC_COL).value + 7).value

    # PHX_CAM_HTAP_DIR
    PHX_CAM_HTAP_LEFT = c_int(c_uint32(etParam.PHX_CAM_HTAP_DIR).value + 1).value
    PHX_CAM_HTAP_RIGHT = c_int(c_uint32(etParam.PHX_CAM_HTAP_DIR).value + 2).value
    PHX_CAM_HTAP_CONVERGE = c_int(c_uint32(etParam.PHX_CAM_HTAP_DIR).value + 3).value
    PHX_CAM_HTAP_DIVERGE = c_int(c_uint32(etParam.PHX_CAM_HTAP_DIR).value + 4).value

    # PHX_CAM_HTAP_TYPE
    PHX_CAM_HTAP_LINEAR = c_int(c_uint32(etParam.PHX_CAM_HTAP_TYPE).value + 1).value
    PHX_CAM_HTAP_OFFSET_1 = c_int(c_uint32(etParam.PHX_CAM_HTAP_TYPE).value + 2).value
    PHX_CAM_HTAP_ALTERNATE = c_int(c_uint32(etParam.PHX_CAM_HTAP_TYPE).value + 3).value
    PHX_CAM_HTAP_OFFSET_2 = c_int(c_uint32(etParam.PHX_CAM_HTAP_TYPE).value + 4).value
    PHX_CAM_HTAP_SPAN = c_int(c_uint32(etParam.PHX_CAM_HTAP_TYPE).value + 5).value

    # PHX_CAM_HTAP_ORDER
    PHX_CAM_HTAP_ASCENDING = c_int(c_uint32(etParam.PHX_CAM_HTAP_ORDER).value + 1).value
    PHX_CAM_HTAP_DESCENDING = c_int(c_uint32(etParam.PHX_CAM_HTAP_ORDER).value + 2).value

    # PHX_CAM_VTAP_DIR
    PHX_CAM_VTAP_TOP = c_int(c_uint32(etParam.PHX_CAM_VTAP_DIR).value + 1).value
    PHX_CAM_VTAP_BOTTOM = c_int(c_uint32(etParam.PHX_CAM_VTAP_DIR).value + 2).value
    PHX_CAM_VTAP_CONVERGE = c_int(c_uint32(etParam.PHX_CAM_VTAP_DIR).value + 3).value
    PHX_CAM_VTAP_DIVERGE = c_int(c_uint32(etParam.PHX_CAM_VTAP_DIR).value + 4).value

    # PHX_CAM_VTAP_TYPE
    PHX_CAM_VTAP_LINEAR = c_int(c_uint32(etParam.PHX_CAM_VTAP_TYPE).value + 1).value
    PHX_CAM_VTAP_OFFSET = c_int(c_uint32(etParam.PHX_CAM_VTAP_TYPE).value + 2).value
    PHX_CAM_VTAP_ALTERNATE = c_int(c_uint32(etParam.PHX_CAM_VTAP_TYPE).value + 3).value

    # PHX_CAM_VTAP_ORDER
    PHX_CAM_VTAP_ASCENDING = c_int(c_uint32(etParam.PHX_CAM_VTAP_ORDER).value + 1).value
    PHX_CAM_VTAP_DESCENDING = c_int(c_uint32(etParam.PHX_CAM_VTAP_ORDER).value + 2).value

    # PHX_CAM_CLOCK_POLARITY
    PHX_CAM_CLOCK_POS = c_int(c_uint32(etParam.PHX_CAM_CLOCK_POLARITY).value + 1).value
    PHX_CAM_CLOCK_NEG = c_int(c_uint32(etParam.PHX_CAM_CLOCK_POLARITY).value + 2).value

    # PHX_CAM_CLOCK_MAX
    PHX_CAM_CLOCK_MAX_DEFAULT = c_int(c_uint32(etParam.PHX_CAM_CLOCK_MAX).value + 1).value
    PHX_CAM_CLOCK_MAX_85MHZ = c_int(c_uint32(etParam.PHX_CAM_CLOCK_MAX).value + 2).value

    # PHX_ACQ_TYPE
    PHX_ACQ_FRAME_12 = c_int(c_uint32(etParam.PHX_ACQ_FIELD_MODE).value + 1).value
    PHX_ACQ_FRAME_21 = c_int(c_uint32(etParam.PHX_ACQ_FIELD_MODE).value + 2).value
    PHX_ACQ_FIELD_12 = c_int(c_uint32(etParam.PHX_ACQ_FIELD_MODE).value + 3).value
    PHX_ACQ_FIELD_21 = c_int(c_uint32(etParam.PHX_ACQ_FIELD_MODE).value + 4).value
    PHX_ACQ_FIELD_1 = c_int(c_uint32(etParam.PHX_ACQ_FIELD_MODE).value + 5).value
    PHX_ACQ_FIELD_2 = c_int(c_uint32(etParam.PHX_ACQ_FIELD_MODE).value + 6).value
    PHX_ACQ_FIELD_NEXT = c_int(c_uint32(etParam.PHX_ACQ_FIELD_MODE).value + 7).value
    PHX_ACQ_LINE_DOUBLE_12 = c_int(c_uint32(etParam.PHX_ACQ_FIELD_MODE).value + 8).value
    PHX_ACQ_LINE_DOUBLE_21 = c_int(c_uint32(etParam.PHX_ACQ_FIELD_MODE).value + 9).value
    PHX_ACQ_LINE_DOUBLE_NEXT = c_int(c_uint32(etParam.PHX_ACQ_FIELD_MODE).value + 10).value
    PHX_ACQ_LINE_DOUBLE_1 = c_int(c_uint32(etParam.PHX_ACQ_FIELD_MODE).value + 11).value
    PHX_ACQ_LINE_DOUBLE_2 = c_int(c_uint32(etParam.PHX_ACQ_FIELD_MODE).value + 12).value

    # PHX_ACQ_XSUB
    PHX_ACQ_X1 = c_int(c_uint32(etParam.PHX_ACQ_XSUB).value + 1).value
    PHX_ACQ_X2 = c_int(c_uint32(etParam.PHX_ACQ_XSUB).value + 2).value
    PHX_ACQ_X4 = c_int(c_uint32(etParam.PHX_ACQ_XSUB).value + 3).value
    PHX_ACQ_X8 = c_int(c_uint32(etParam.PHX_ACQ_XSUB).value + 4).value

    # PHX_DST_PTR_TYPE
    PHX_DST_PTR_INTERNAL = c_int(c_uint32(etParam.PHX_DST_PTR_TYPE).value + 1).value
    PHX_DST_PTR_USER_VIRT = c_int(c_uint32(etParam.PHX_DST_PTR_TYPE).value + 2).value
    PHX_DST_PTR_USER_PHYS = c_int(c_uint32(etParam.PHX_DST_PTR_TYPE).value + 3).value
    PHX_DST_PTR_USER_CUDA = c_int(c_uint32(etParam.PHX_DST_PTR_TYPE).value + 4).value

    # PHX_DATASTREAM_VALID
    PHX_DATASTREAM_ALWAYS = c_int(c_uint32(etParam.PHX_DATASTREAM_VALID).value + 1).value
    PHX_DATASTREAM_LINE_ONLY = c_int(c_uint32(etParam.PHX_DATASTREAM_VALID).value + 2).value
    PHX_DATASTREAM_FRAME_ONLY = c_int(c_uint32(etParam.PHX_DATASTREAM_VALID).value + 3).value
    PHX_DATASTREAM_FRAME_AND_LINE = c_int(c_uint32(etParam.PHX_DATASTREAM_VALID).value + 4).value

    # PHX_DATASRC
    PHX_DATASRC_CAMERA = c_int(c_uint32(etParam.PHX_DATASRC).value + 1).value
    PHX_DATASRC_SIMULATOR_STATIC = c_int(c_uint32(etParam.PHX_DATASRC).value + 2).value
    PHX_DATASRC_SIMULATOR_ROLL = c_int(c_uint32(etParam.PHX_DATASRC).value + 3).value
    PHX_DATASRC_SIMULATOR_PRBS = c_int(c_uint32(etParam.PHX_DATASRC).value + 4).value

    # PHX_BUS_FORMAT
    PHX_BUS_FORMAT_MONO8 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 1).value
    PHX_BUS_FORMAT_MONO16 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 2).value
    PHX_BUS_FORMAT_MONO32 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 3).value
    PHX_BUS_FORMAT_MONO36 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 4).value
    PHX_BUS_FORMAT_BGR5 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 5).value
    PHX_BUS_FORMAT_BGR565 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 6).value
    PHX_BUS_FORMAT_XBGR8 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 7).value
    PHX_BUS_FORMAT_BGRX8 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 8).value
    PHX_BUS_FORMAT_BGR16 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 9).value
    PHX_BUS_FORMAT_RGB5 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 10).value
    PHX_BUS_FORMAT_RGB565 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 11).value
    PHX_BUS_FORMAT_XRGB8 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 12).value
    PHX_BUS_FORMAT_RGBX8 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 13).value
    PHX_BUS_FORMAT_RGB16 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 14).value
    PHX_BUS_FORMAT_BGR101210 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 15).value
    PHX_BUS_FORMAT_RGB101210 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 16).value
    PHX_BUS_FORMAT_BGR8 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 17).value
    PHX_BUS_FORMAT_RGB8 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 18).value
    PHX_BUS_FORMAT_MONO10 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 19).value
    PHX_BUS_FORMAT_MONO12 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 20).value
    PHX_BUS_FORMAT_MONO14 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 21).value
    PHX_DST_FORMAT_BAY8 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 22).value
    PHX_DST_FORMAT_BAY10 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 23).value
    PHX_DST_FORMAT_BAY12 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 24).value
    PHX_DST_FORMAT_BAY14 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 25).value
    PHX_DST_FORMAT_BAY16 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 26).value
    PHX_DST_FORMAT_2Y12 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 27).value
    PHX_BUS_FORMAT_BGR12 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 28).value
    PHX_BUS_FORMAT_RGB12 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 29).value
    PHX_BUS_FORMAT_YUV422_8 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 30).value
    PHX_DST_FORMAT_Y12B = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 31).value
    PHX_BUS_FORMAT_RGB8_PLANAR = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 32).value
    PHX_BUS_FORMAT_MONO10P = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 33).value
    PHX_BUS_FORMAT_MONO14P = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 34).value
    PHX_BUS_FORMAT_RGBA8 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 35).value
    PHX_BUS_FORMAT_RGBA10 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 36).value
    PHX_BUS_FORMAT_RGBA12 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 37).value
    PHX_BUS_FORMAT_RGBA14 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 38).value
    PHX_BUS_FORMAT_RGBA16 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 39).value
    PHX_BUS_FORMAT_BAYER_GR8 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 40).value
    PHX_BUS_FORMAT_BAYER_RG8 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 41).value
    PHX_BUS_FORMAT_BAYER_GB8 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 42).value
    PHX_BUS_FORMAT_BAYER_BG8 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 43).value
    PHX_BUS_FORMAT_BAYER_GR10 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 44).value
    PHX_BUS_FORMAT_BAYER_RG10 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 45).value
    PHX_BUS_FORMAT_BAYER_GB10 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 46).value
    PHX_BUS_FORMAT_BAYER_BG10 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 47).value
    PHX_BUS_FORMAT_BAYER_GR12 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 48).value
    PHX_BUS_FORMAT_BAYER_RG12 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 49).value
    PHX_BUS_FORMAT_BAYER_GB12 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 50).value
    PHX_BUS_FORMAT_BAYER_BG12 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 51).value
    PHX_BUS_FORMAT_BAYER_GR14 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 52).value
    PHX_BUS_FORMAT_BAYER_RG14 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 53).value
    PHX_BUS_FORMAT_BAYER_GB14 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 54).value
    PHX_BUS_FORMAT_BAYER_BG14 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 55).value
    PHX_BUS_FORMAT_BAYER_GR16 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 56).value
    PHX_BUS_FORMAT_BAYER_RG16 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 57).value
    PHX_BUS_FORMAT_BAYER_GB16 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 58).value
    PHX_BUS_FORMAT_BAYER_BG16 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 59).value
    PHX_BUS_FORMAT_BGR10 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 60).value
    PHX_BUS_FORMAT_RGB10 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 61).value
    PHX_BUS_FORMAT_BGR14 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 62).value
    PHX_BUS_FORMAT_RGB14 = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 63).value
    PHX_BUS_FORMAT_MONO12P = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 64).value
    PHX_BUS_FORMAT_BAYER_GR10P = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 65).value
    PHX_BUS_FORMAT_BAYER_RG10P = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 66).value
    PHX_BUS_FORMAT_BAYER_GB10P = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 67).value
    PHX_BUS_FORMAT_BAYER_BG10P = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 68).value
    PHX_BUS_FORMAT_BAYER_GR12P = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 69).value
    PHX_BUS_FORMAT_BAYER_RG12P = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 70).value
    PHX_BUS_FORMAT_BAYER_GB12P = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 71).value
    PHX_BUS_FORMAT_BAYER_BG12P = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 72).value
    PHX_BUS_FORMAT_BAYER_GR14P = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 73).value
    PHX_BUS_FORMAT_BAYER_RG14P = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 74).value
    PHX_BUS_FORMAT_BAYER_GB14P = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 75).value
    PHX_BUS_FORMAT_BAYER_BG14P = c_int(c_uint32(etParam.PHX_BUS_FORMAT).value + 76).value

    # PHX_USR_FORMAT
    PHX_USR_FORMAT_MONO8 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 1).value
    PHX_USR_FORMAT_MONO16 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 2).value
    PHX_USR_FORMAT_MONO32 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 3).value
    PHX_USR_FORMAT_MONO36 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 4).value
    PHX_USR_FORMAT_BGR5 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 5).value
    PHX_USR_FORMAT_BGR565 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 6).value
    PHX_USR_FORMAT_XBGR8 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 7).value
    PHX_USR_FORMAT_BGRX8 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 8).value
    PHX_USR_FORMAT_BGR16 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 9).value
    PHX_USR_FORMAT_RGB5 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 10).value
    PHX_USR_FORMAT_RGB565 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 11).value
    PHX_USR_FORMAT_XRGB8 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 12).value
    PHX_USR_FORMAT_RGBX8 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 13).value
    PHX_USR_FORMAT_RGB16 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 14).value
    PHX_USR_FORMAT_BGR101210 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 15).value
    PHX_USR_FORMAT_RGB101210 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 16).value
    PHX_USR_FORMAT_BGR8 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 17).value
    PHX_USR_FORMAT_RGB8 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 18).value
    PHX_USR_FORMAT_MONO10 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 19).value
    PHX_USR_FORMAT_MONO12 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 20).value
    PHX_USR_FORMAT_MONO14 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 21).value
    PHX_USER_FORMAT_BAY8 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 22).value
    PHX_USER_FORMAT_BAY10 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 23).value
    PHX_USER_FORMAT_BAY12 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 24).value
    PHX_USER_FORMAT_BAY14 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 25).value
    PHX_USER_FORMAT_BAY16 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 26).value
    PHX_USER_FORMAT_2Y12 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 27).value
    PHX_USR_FORMAT_BGR12 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 28).value
    PHX_USR_FORMAT_RGB12 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 29).value
    PHX_USR_FORMAT_YUV422_8 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 30).value
    PHX_USER_FORMAT_Y12B = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 31).value
    PHX_USR_FORMAT_RGB8_PLANAR = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 32).value
    PHX_USR_FORMAT_MONO10P = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 33).value
    PHX_USR_FORMAT_MONO14P = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 34).value
    PHX_USR_FORMAT_RGBA8 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 35).value
    PHX_USR_FORMAT_RGBA10 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 36).value
    PHX_USR_FORMAT_RGBA12 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 37).value
    PHX_USR_FORMAT_RGBA14 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 38).value
    PHX_USR_FORMAT_RGBA16 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 39).value
    PHX_USR_FORMAT_BAYER_GR8 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 40).value
    PHX_USR_FORMAT_BAYER_RG8 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 41).value
    PHX_USR_FORMAT_BAYER_GB8 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 42).value
    PHX_USR_FORMAT_BAYER_BG8 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 43).value
    PHX_USR_FORMAT_BAYER_GR10 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 44).value
    PHX_USR_FORMAT_BAYER_RG10 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 45).value
    PHX_USR_FORMAT_BAYER_GB10 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 46).value
    PHX_USR_FORMAT_BAYER_BG10 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 47).value
    PHX_USR_FORMAT_BAYER_GR12 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 48).value
    PHX_USR_FORMAT_BAYER_RG12 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 49).value
    PHX_USR_FORMAT_BAYER_GB12 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 50).value
    PHX_USR_FORMAT_BAYER_BG12 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 51).value
    PHX_USR_FORMAT_BAYER_GR14 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 52).value
    PHX_USR_FORMAT_BAYER_RG14 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 53).value
    PHX_USR_FORMAT_BAYER_GB14 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 54).value
    PHX_USR_FORMAT_BAYER_BG14 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 55).value
    PHX_USR_FORMAT_BAYER_GR16 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 56).value
    PHX_USR_FORMAT_BAYER_RG16 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 57).value
    PHX_USR_FORMAT_BAYER_GB16 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 58).value
    PHX_USR_FORMAT_BAYER_BG16 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 59).value
    PHX_USR_FORMAT_BGR10 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 60).value
    PHX_USR_FORMAT_RGB10 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 61).value
    PHX_USR_FORMAT_BGR14 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 62).value
    PHX_USR_FORMAT_RGB14 = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 63).value
    PHX_USR_FORMAT_MONO12P = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 64).value
    PHX_USR_FORMAT_BAYER_GR10P = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 65).value
    PHX_USR_FORMAT_BAYER_RG10P = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 66).value
    PHX_USR_FORMAT_BAYER_GB10P = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 67).value
    PHX_USR_FORMAT_BAYER_BG10P = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 68).value
    PHX_USR_FORMAT_BAYER_GR12P = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 69).value
    PHX_USR_FORMAT_BAYER_RG12P = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 70).value
    PHX_USR_FORMAT_BAYER_GB12P = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 71).value
    PHX_USR_FORMAT_BAYER_BG12P = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 72).value
    PHX_USR_FORMAT_BAYER_GR14P = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 73).value
    PHX_USR_FORMAT_BAYER_RG14P = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 74).value
    PHX_USR_FORMAT_BAYER_GB14P = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 75).value
    PHX_USR_FORMAT_BAYER_BG14P = c_int(c_uint32(etParam.PHX_USR_FORMAT).value + 76).value


    PHX_LINETRIG_NONE = c_int(c_uint32(etParam.PHX_LINETRIG_SRC).value + 1).value
    PHX_LINETRIG_AUXIN_1_RISING = c_int(c_uint32(etParam.PHX_LINETRIG_SRC).value + 2).value
    PHX_LINETRIG_AUXIN_1_FALLING = c_int(c_uint32(etParam.PHX_LINETRIG_SRC).value + 3).value
    PHX_LINETRIG_CTRLIN_2_RISING = c_int(c_uint32(etParam.PHX_LINETRIG_SRC).value + 4).value
    PHX_LINETRIG_CTRLIN_2_FALLING = c_int(c_uint32(etParam.PHX_LINETRIG_SRC).value + 5).value
    PHX_LINETRIG_AUXIN_2_RISING = c_int(c_uint32(etParam.PHX_LINETRIG_SRC).value + 6).value
    PHX_LINETRIG_AUXIN_2_FALLING = c_int(c_uint32(etParam.PHX_LINETRIG_SRC).value + 7).value
    PHX_LINETRIG_TIMER = c_int(c_uint32(etParam.PHX_LINETRIG_SRC).value + 8).value
    PHX_LINETRIG_AUXIN_A1_RISING = c_int(c_uint32(etParam.PHX_LINETRIG_SRC).value + 9).value
    PHX_LINETRIG_AUXIN_A1_FALLING = c_int(c_uint32(etParam.PHX_LINETRIG_SRC).value + 10).value
    PHX_LINETRIG_AUXIN_A2_RISING = c_int(c_uint32(etParam.PHX_LINETRIG_SRC).value + 11).value
    PHX_LINETRIG_AUXIN_A2_FALLING = c_int(c_uint32(etParam.PHX_LINETRIG_SRC).value + 12).value
    PHX_LINETRIG_AUXIN_B1_RISING = c_int(c_uint32(etParam.PHX_LINETRIG_SRC).value + 13).value
    PHX_LINETRIG_AUXIN_B1_FALLING = c_int(c_uint32(etParam.PHX_LINETRIG_SRC).value + 14).value
    PHX_LINETRIG_AUXIN_B2_RISING = c_int(c_uint32(etParam.PHX_LINETRIG_SRC).value + 15).value
    PHX_LINETRIG_AUXIN_B2_FALLING = c_int(c_uint32(etParam.PHX_LINETRIG_SRC).value + 16).value

    # PHX_LINETRIG_TIMER_CTRL
    PHX_LINETRIG_TIMER_TIME = c_int(c_uint32(etParam.PHX_LINETRIG_TIMER_CTRL).value + 1).value
    PHX_LINETRIG_TIMER_DISABLE = c_int(c_uint32(etParam.PHX_LINETRIG_TIMER_CTRL).value + 2).value
    PHX_LINETRIG_TIMER_LINES = c_int(c_uint32(etParam.PHX_LINETRIG_TIMER_CTRL).value + 3).value

    # PHX_CAMTRIG_SRC
    PHX_CAMTRIG_SRC_LINETRIG = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 1).value
    PHX_CAMTRIG_SRC_FGTRIGA_CHX = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 2).value
    PHX_CAMTRIG_SRC_NONE = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 3).value
    PHX_CAMTRIG_SRC_SWTRIG_CHX = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 4).value
    PHX_CAMTRIG_SRC_AUXIN_1_RISE = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 5).value
    PHX_CAMTRIG_SRC_AUXIN_1_FALL = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 6).value
    PHX_CAMTRIG_SRC_AUXIN_2_RISE = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 7).value
    PHX_CAMTRIG_SRC_AUXIN_2_FALL = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 8).value
    PHX_CAMTRIG_SRC_TIMERA1_CHX = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 9).value
    PHX_CAMTRIG_SRC_AUXIN_A1_RISE = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 10).value
    PHX_CAMTRIG_SRC_AUXIN_A1_FALL = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 11).value
    PHX_CAMTRIG_SRC_AUXIN_A2_RISE = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 12).value
    PHX_CAMTRIG_SRC_AUXIN_A2_FALL = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 13).value
    PHX_CAMTRIG_SRC_AUXIN_B1_RISE = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 14).value
    PHX_CAMTRIG_SRC_AUXIN_B1_FALL = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 15).value
    PHX_CAMTRIG_SRC_AUXIN_B2_RISE = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 16).value
    PHX_CAMTRIG_SRC_AUXIN_B2_FALL = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 17).value
    PHX_CAMTRIG_SRC_FGTRIGA_CH1 = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 18).value
    PHX_CAMTRIG_SRC_FGTRIGA_CH2 = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 19).value
    PHX_CAMTRIG_SRC_FGTRIGA_CH3 = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 20).value
    PHX_CAMTRIG_SRC_FGTRIGA_CH4 = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 21).value
    PHX_CAMTRIG_SRC_SWTRIG_CH1 = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 22).value
    PHX_CAMTRIG_SRC_SWTRIG_CH2 = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 23).value
    PHX_CAMTRIG_SRC_SWTRIG_CH3 = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 24).value
    PHX_CAMTRIG_SRC_SWTRIG_CH4 = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 25).value
    PHX_CAMTRIG_SRC_TIMERA1_CH1 = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 26).value
    PHX_CAMTRIG_SRC_TIMERA1_CH2 = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 27).value
    PHX_CAMTRIG_SRC_TIMERA1_CH3 = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 28).value
    PHX_CAMTRIG_SRC_TIMERA1_CH4 = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 29).value
    PHX_CAMTRIG_SRC_422IN_CHX_0_RISE = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 30).value
    PHX_CAMTRIG_SRC_422IN_CH1_0_RISE = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 31).value
    PHX_CAMTRIG_SRC_422IN_CH2_0_RISE = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 32).value
    PHX_CAMTRIG_SRC_422IN_CH3_0_RISE = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 33).value
    PHX_CAMTRIG_SRC_422IN_CH4_0_RISE = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 34).value
    PHX_CAMTRIG_SRC_422IN_CHX_0_FALL = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 35).value
    PHX_CAMTRIG_SRC_422IN_CH1_0_FALL = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 36).value
    PHX_CAMTRIG_SRC_422IN_CH2_0_FALL = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 37).value
    PHX_CAMTRIG_SRC_422IN_CH3_0_FALL = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 38).value
    PHX_CAMTRIG_SRC_422IN_CH4_0_FALL = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 39).value
    PHX_CAMTRIG_SRC_OPTOIN_CHX_0_RISE = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 40).value
    PHX_CAMTRIG_SRC_OPTOIN_CH1_0_RISE = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 41).value
    PHX_CAMTRIG_SRC_OPTOIN_CH2_0_RISE = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 42).value
    PHX_CAMTRIG_SRC_OPTOIN_CH3_0_RISE = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 43).value
    PHX_CAMTRIG_SRC_OPTOIN_CH4_0_RISE = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 44).value
    PHX_CAMTRIG_SRC_OPTOIN_CHX_0_FALL = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 45).value
    PHX_CAMTRIG_SRC_OPTOIN_CH1_0_FALL = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 46).value
    PHX_CAMTRIG_SRC_OPTOIN_CH2_0_FALL = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 47).value
    PHX_CAMTRIG_SRC_OPTOIN_CH3_0_FALL = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 48).value
    PHX_CAMTRIG_SRC_OPTOIN_CH4_0_FALL = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 49).value
    PHX_CAMTRIG_SRC_TTLIN_CHX_0_RISE = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 50).value
    PHX_CAMTRIG_SRC_TTLIN_CH1_0_RISE = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 51).value
    PHX_CAMTRIG_SRC_TTLIN_CH2_0_RISE = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 52).value
    PHX_CAMTRIG_SRC_TTLIN_CH3_0_RISE = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 53).value
    PHX_CAMTRIG_SRC_TTLIN_CH4_0_RISE = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 54).value
    PHX_CAMTRIG_SRC_TTLIN_CHX_0_FALL = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 55).value
    PHX_CAMTRIG_SRC_TTLIN_CH1_0_FALL = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 56).value
    PHX_CAMTRIG_SRC_TTLIN_CH2_0_FALL = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 57).value
    PHX_CAMTRIG_SRC_TTLIN_CH3_0_FALL = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 58).value
    PHX_CAMTRIG_SRC_TTLIN_CH4_0_FALL = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 59).value
    PHX_CAMTRIG_SRC_ENCODER_CHX = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 60).value
    PHX_CAMTRIG_SRC_ENCODER_CH1 = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 61).value
    PHX_CAMTRIG_SRC_ENCODER_CH2 = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 62).value
    PHX_CAMTRIG_SRC_ENCODER_CH3 = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 63).value
    PHX_CAMTRIG_SRC_ENCODER_CH4 = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 64).value
    PHX_CAMTRIG_SRC_24VIN_CHX_0_RISE = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 65).value
    PHX_CAMTRIG_SRC_24VIN_CHX_0_FALL = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 66).value
    PHX_CAMTRIG_SRC_24VIN_CH1_0_RISE = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 67).value
    PHX_CAMTRIG_SRC_24VIN_CH1_0_FALL = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 68).value
    PHX_CAMTRIG_SRC_24VIN_CH2_0_RISE = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 69).value
    PHX_CAMTRIG_SRC_24VIN_CH2_0_FALL = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 70).value
    PHX_CAMTRIG_SRC_24VIN_CH3_0_RISE = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 71).value
    PHX_CAMTRIG_SRC_24VIN_CH3_0_FALL = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 72).value
    PHX_CAMTRIG_SRC_24VIN_CH4_0_RISE = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 73).value
    PHX_CAMTRIG_SRC_24VIN_CH4_0_FALL = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 74).value
    PHX_CAMTRIG_SRC_24VIN_CH5_0_RISE = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 75).value
    PHX_CAMTRIG_SRC_24VIN_CH5_0_FALL = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 76).value
    PHX_CAMTRIG_SRC_24VIN_CH6_0_RISE = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 77).value
    PHX_CAMTRIG_SRC_24VIN_CH6_0_FALL = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 78).value
    PHX_CAMTRIG_SRC_24VIN_CH7_0_RISE = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 79).value
    PHX_CAMTRIG_SRC_24VIN_CH7_0_FALL = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 80).value
    PHX_CAMTRIG_SRC_24VIN_CH8_0_RISE = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 81).value
    PHX_CAMTRIG_SRC_24VIN_CH8_0_FALL = c_int(c_uint32(etParam.PHX_CAMTRIG_SRC).value + 82).value

    # PHX_CAMTRIG_ENCODER_MODE
    PHX_CAMTRIG_ENCODER_MODE1 = c_int(c_uint32(etParam.PHX_CAMTRIG_ENCODER_MODE).value + 1).value
    PHX_CAMTRIG_ENCODER_MODE2 = c_int(c_uint32(etParam.PHX_CAMTRIG_ENCODER_MODE).value + 2).value
    PHX_CAMTRIG_ENCODER_MODE3 = c_int(c_uint32(etParam.PHX_CAMTRIG_ENCODER_MODE).value + 3).value
    PHX_CAMTRIG_ENCODER_MODE4 = c_int(c_uint32(etParam.PHX_CAMTRIG_ENCODER_MODE).value + 4).value
    PHX_CAMTRIG_ENCODER_MODE5 = c_int(c_uint32(etParam.PHX_CAMTRIG_ENCODER_MODE).value + 5).value

    # PHX_CAMTRIG_ENCODER_SRC
    PHX_CAMTRIG_SRC_422IN_CHX_0 = c_int(c_uint32(etParam.PHX_CAMTRIG_ENCODER_SRC).value + 1).value
    PHX_CAMTRIG_SRC_422IN_CH1_0 = c_int(c_uint32(etParam.PHX_CAMTRIG_ENCODER_SRC).value + 2).value
    PHX_CAMTRIG_SRC_422IN_CH2_0 = c_int(c_uint32(etParam.PHX_CAMTRIG_ENCODER_SRC).value + 3).value
    PHX_CAMTRIG_SRC_422IN_CH3_0 = c_int(c_uint32(etParam.PHX_CAMTRIG_ENCODER_SRC).value + 4).value
    PHX_CAMTRIG_SRC_422IN_CH4_0 = c_int(c_uint32(etParam.PHX_CAMTRIG_ENCODER_SRC).value + 5).value
    PHX_CAMTRIG_SRC_OPTOIN_CHX_0 = c_int(c_uint32(etParam.PHX_CAMTRIG_ENCODER_SRC).value + 6).value
    PHX_CAMTRIG_SRC_OPTOIN_CH1_0 = c_int(c_uint32(etParam.PHX_CAMTRIG_ENCODER_SRC).value + 7).value
    PHX_CAMTRIG_SRC_OPTOIN_CH2_0 = c_int(c_uint32(etParam.PHX_CAMTRIG_ENCODER_SRC).value + 8).value
    PHX_CAMTRIG_SRC_OPTOIN_CH3_0 = c_int(c_uint32(etParam.PHX_CAMTRIG_ENCODER_SRC).value + 9).value
    PHX_CAMTRIG_SRC_OPTOIN_CH4_0 = c_int(c_uint32(etParam.PHX_CAMTRIG_ENCODER_SRC).value + 10).value
    PHX_CAMTRIG_SRC_TTLIN_CHX_0 = c_int(c_uint32(etParam.PHX_CAMTRIG_ENCODER_SRC).value + 11).value
    PHX_CAMTRIG_SRC_TTLIN_CH1_0 = c_int(c_uint32(etParam.PHX_CAMTRIG_ENCODER_SRC).value + 12).value
    PHX_CAMTRIG_SRC_TTLIN_CH2_0 = c_int(c_uint32(etParam.PHX_CAMTRIG_ENCODER_SRC).value + 13).value
    PHX_CAMTRIG_SRC_TTLIN_CH3_0 = c_int(c_uint32(etParam.PHX_CAMTRIG_ENCODER_SRC).value + 14).value
    PHX_CAMTRIG_SRC_TTLIN_CH4_0 = c_int(c_uint32(etParam.PHX_CAMTRIG_ENCODER_SRC).value + 15).value

    # PHX_CAMTRIG_DELAY_MODE
    PHX_CAMTRIG_DELAY_NONE = c_int(c_uint32(etParam.PHX_CAMTRIG_DELAY_MODE).value + 1).value
    PHX_CAMTRIG_DELAY_TIME = c_int(c_uint32(etParam.PHX_CAMTRIG_DELAY_MODE).value + 2).value
    PHX_CAMTRIG_DELAY_LINE = c_int(c_uint32(etParam.PHX_CAMTRIG_DELAY_MODE).value + 3).value

    # PHX_CAMTRIG_CXPTRIG_SRC
    PHX_CAMTRIG_CXPTRIG_NONE = c_int(c_uint32(etParam.PHX_CAMTRIG_CXPTRIG_SRC).value + 1).value
    PHX_CAMTRIG_CXPTRIG_TIMERM1_CHX = c_int(c_uint32(etParam.PHX_CAMTRIG_CXPTRIG_SRC).value + 2).value
    PHX_CAMTRIG_CXPTRIG_TIMERM1_CH1 = c_int(c_uint32(etParam.PHX_CAMTRIG_CXPTRIG_SRC).value + 3).value
    PHX_CAMTRIG_CXPTRIG_TIMERM1_CH2 = c_int(c_uint32(etParam.PHX_CAMTRIG_CXPTRIG_SRC).value + 4).value
    PHX_CAMTRIG_CXPTRIG_TIMERM1_CH3 = c_int(c_uint32(etParam.PHX_CAMTRIG_CXPTRIG_SRC).value + 5).value
    PHX_CAMTRIG_CXPTRIG_TIMERM1_CH4 = c_int(c_uint32(etParam.PHX_CAMTRIG_CXPTRIG_SRC).value + 6).value
    PHX_CAMTRIG_CXPTRIG_TIMERM2_CHX = c_int(c_uint32(etParam.PHX_CAMTRIG_CXPTRIG_SRC).value + 7).value
    PHX_CAMTRIG_CXPTRIG_TIMERM2_CH1 = c_int(c_uint32(etParam.PHX_CAMTRIG_CXPTRIG_SRC).value + 8).value
    PHX_CAMTRIG_CXPTRIG_TIMERM2_CH2 = c_int(c_uint32(etParam.PHX_CAMTRIG_CXPTRIG_SRC).value + 9).value
    PHX_CAMTRIG_CXPTRIG_TIMERM2_CH3 = c_int(c_uint32(etParam.PHX_CAMTRIG_CXPTRIG_SRC).value + 10).value
    PHX_CAMTRIG_CXPTRIG_TIMERM2_CH4 = c_int(c_uint32(etParam.PHX_CAMTRIG_CXPTRIG_SRC).value + 11).value
    PHX_CAMTRIG_CXPTRIG_SW_RISE = c_int(c_uint32(etParam.PHX_CAMTRIG_CXPTRIG_SRC).value + 12).value
    PHX_CAMTRIG_CXPTRIG_SW_FALL = c_int(c_uint32(etParam.PHX_CAMTRIG_CXPTRIG_SRC).value + 13).value
    PHX_CAMTRIG_CXPTRIG_FGTRIGD_CHX = c_int(c_uint32(etParam.PHX_CAMTRIG_CXPTRIG_SRC).value + 14).value
    PHX_CAMTRIG_CXPTRIG_FGTRIGD_CH1 = c_int(c_uint32(etParam.PHX_CAMTRIG_CXPTRIG_SRC).value + 15).value
    PHX_CAMTRIG_CXPTRIG_FGTRIGD_CH2 = c_int(c_uint32(etParam.PHX_CAMTRIG_CXPTRIG_SRC).value + 16).value
    PHX_CAMTRIG_CXPTRIG_FGTRIGD_CH3 = c_int(c_uint32(etParam.PHX_CAMTRIG_CXPTRIG_SRC).value + 17).value
    PHX_CAMTRIG_CXPTRIG_FGTRIGD_CH4 = c_int(c_uint32(etParam.PHX_CAMTRIG_CXPTRIG_SRC).value + 18).value

    # PHX_CAMTRIG_CXPTRIG_MODE
    PHX_CAMTRIG_CXPTRIG_RISEFALL = c_int(c_uint32(etParam.PHX_CAMTRIG_CXPTRIG_MODE).value + 1).value
    PHX_CAMTRIG_CXPTRIG_RISEFALL_INV = c_int(c_uint32(etParam.PHX_CAMTRIG_CXPTRIG_MODE).value + 2).value
    PHX_CAMTRIG_CXPTRIG_RISE = c_int(c_uint32(etParam.PHX_CAMTRIG_CXPTRIG_MODE).value + 3).value
    PHX_CAMTRIG_CXPTRIG_RISE_INV = c_int(c_uint32(etParam.PHX_CAMTRIG_CXPTRIG_MODE).value + 4).value
    PHX_CAMTRIG_CXPTRIG_FALL = c_int(c_uint32(etParam.PHX_CAMTRIG_CXPTRIG_MODE).value + 5).value
    PHX_CAMTRIG_CXPTRIG_FALL_INV = c_int(c_uint32(etParam.PHX_CAMTRIG_CXPTRIG_MODE).value + 6).value

    # PHX_EXP_CTRLIO_1
    PHX_EXP_CTRLIO_1_HW_POS = c_int(c_uint32(etParam.PHX_EXP_CTRLIO_1).value + 1).value
    PHX_EXP_CTRLIO_1_HW_NEG = c_int(c_uint32(etParam.PHX_EXP_CTRLIO_1).value + 2).value
    PHX_EXP_CTRLIO_1_SW_POS = c_int(c_uint32(etParam.PHX_EXP_CTRLIO_1).value + 3).value
    PHX_EXP_CTRLIO_1_SW_NEG = c_int(c_uint32(etParam.PHX_EXP_CTRLIO_1).value + 4).value

    # PHX_EXP_CTRLIO_2
    PHX_EXP_CTRLIO_2_HW_POS = c_int(c_uint32(etParam.PHX_EXP_CTRLIO_2).value + 1).value
    PHX_EXP_CTRLIO_2_HW_NEG = c_int(c_uint32(etParam.PHX_EXP_CTRLIO_2).value + 2).value
    PHX_EXP_CTRLIO_2_SW_POS = c_int(c_uint32(etParam.PHX_EXP_CTRLIO_2).value + 3).value
    PHX_EXP_CTRLIO_2_SW_NEG = c_int(c_uint32(etParam.PHX_EXP_CTRLIO_2).value + 4).value

    # PHX_EXP_LINESTART
    PHX_EXP_LINESTART_LINE = c_int(c_uint32(etParam.PHX_EXP_LINESTART).value + 1).value
    PHX_EXP_LINESTART_CCIO_2 = c_int(c_uint32(etParam.PHX_EXP_LINESTART).value + 2).value
    PHX_EXP_LINESTART_CCIO_A2 = c_int(c_uint32(etParam.PHX_EXP_LINESTART).value + 3).value
    PHX_EXP_LINESTART_CCIO_B2 = c_int(c_uint32(etParam.PHX_EXP_LINESTART).value + 4).value

    # PHX_FGTRIG_SRC
    PHX_FGTRIG_SRC_OPTO_A1 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 1).value
    PHX_FGTRIG_SRC_OPTO_A2 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 2).value
    PHX_FGTRIG_SRC_OPTO_B1 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 3).value
    PHX_FGTRIG_SRC_OPTO_B2 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 4).value
    PHX_FGTRIG_SRC_CTRLIN_A1 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 5).value
    PHX_FGTRIG_SRC_CTRLIN_A2 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 6).value
    PHX_FGTRIG_SRC_CTRLIN_A3 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 7).value
    PHX_FGTRIG_SRC_CTRLIN_B1 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 8).value
    PHX_FGTRIG_SRC_CTRLIN_B2 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 9).value
    PHX_FGTRIG_SRC_CTRLIN_B3 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 10).value
    PHX_FGTRIG_SRC_CCIO_A1 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 11).value
    PHX_FGTRIG_SRC_CCIO_A2 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 12).value
    PHX_FGTRIG_SRC_CCIO_B1 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 13).value
    PHX_FGTRIG_SRC_CCIO_B2 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 14).value
    PHX_FGTRIG_SRC_AUXIN_A1 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 15).value
    PHX_FGTRIG_SRC_AUXIN_A2 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 16).value
    PHX_FGTRIG_SRC_AUXIN_B1 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 17).value
    PHX_FGTRIG_SRC_AUXIN_B2 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 18).value
    PHX_FGTRIG_SRC_OPTO_1 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 19).value
    PHX_FGTRIG_SRC_OPTO_2 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 20).value
    PHX_FGTRIG_SRC_AUXIN_1 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 21).value
    PHX_FGTRIG_SRC_AUXIN_2 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 22).value
    PHX_FGTRIG_SRC_CTRLIN_1 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 23).value
    PHX_FGTRIG_SRC_CTRLIN_2 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 24).value
    PHX_FGTRIG_SRC_CTRLIN_3 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 25).value
    PHX_FGTRIG_SRC_CCIO_1 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 26).value
    PHX_FGTRIG_SRC_CCIO_2 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 27).value
    PHX_FGTRIG_SRC_TIMERA1_CHX = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 28).value
    PHX_FGTRIG_SRC_TIMERA1_CH1 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 29).value
    PHX_FGTRIG_SRC_TIMERA1_CH2 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 30).value
    PHX_FGTRIG_SRC_TIMERA1_CH3 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 31).value
    PHX_FGTRIG_SRC_TIMERA1_CH4 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 32).value
    PHX_FGTRIG_SRC_422IN_CHX_0 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 33).value
    PHX_FGTRIG_SRC_422IN_CH1_0 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 34).value
    PHX_FGTRIG_SRC_422IN_CH2_0 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 35).value
    PHX_FGTRIG_SRC_422IN_CH3_0 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 36).value
    PHX_FGTRIG_SRC_422IN_CH4_0 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 37).value
    PHX_FGTRIG_SRC_OPTOIN_CHX_0 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 38).value
    PHX_FGTRIG_SRC_OPTOIN_CH1_0 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 39).value
    PHX_FGTRIG_SRC_OPTOIN_CH2_0 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 40).value
    PHX_FGTRIG_SRC_OPTOIN_CH3_0 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 41).value
    PHX_FGTRIG_SRC_OPTOIN_CH4_0 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 42).value
    PHX_FGTRIG_SRC_TTLIN_CHX_0 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 43).value
    PHX_FGTRIG_SRC_TTLIN_CH1_0 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 44).value
    PHX_FGTRIG_SRC_TTLIN_CH2_0 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 45).value
    PHX_FGTRIG_SRC_TTLIN_CH3_0 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 46).value
    PHX_FGTRIG_SRC_TTLIN_CH4_0 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 47).value
    PHX_FGTRIG_SRC_24VIN_CHX_0 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 48).value
    PHX_FGTRIG_SRC_24VIN_CH1_0 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 49).value
    PHX_FGTRIG_SRC_24VIN_CH2_0 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 50).value
    PHX_FGTRIG_SRC_24VIN_CH3_0 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 51).value
    PHX_FGTRIG_SRC_24VIN_CH4_0 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 52).value
    PHX_FGTRIG_SRC_24VIN_CH5_0 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 53).value
    PHX_FGTRIG_SRC_24VIN_CH6_0 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 54).value
    PHX_FGTRIG_SRC_24VIN_CH7_0 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 55).value
    PHX_FGTRIG_SRC_24VIN_CH8_0 = c_int(c_uint32(etParam.PHX_FGTRIG_SRC).value + 56).value

    # PHX_FGTRIG_MODE
    PHX_FGTRIG_FREERUN = c_int(c_uint32(etParam.PHX_FGTRIG_MODE).value + 1).value
    PHX_FGTRIG_FIRST_POS_EDGE = c_int(c_uint32(etParam.PHX_FGTRIG_MODE).value + 2).value
    PHX_FGTRIG_FIRST_NEG_EDGE = c_int(c_uint32(etParam.PHX_FGTRIG_MODE).value + 3).value
    PHX_FGTRIG_EACH_POS_EDGE = c_int(c_uint32(etParam.PHX_FGTRIG_MODE).value + 4).value
    PHX_FGTRIG_EACH_NEG_EDGE = c_int(c_uint32(etParam.PHX_FGTRIG_MODE).value + 5).value
    PHX_FGTRIG_FIRST_POS_LEVEL = c_int(c_uint32(etParam.PHX_FGTRIG_MODE).value + 6).value
    PHX_FGTRIG_FIRST_NEG_LEVEL = c_int(c_uint32(etParam.PHX_FGTRIG_MODE).value + 7).value
    PHX_FGTRIG_EACH_POS_LEVEL = c_int(c_uint32(etParam.PHX_FGTRIG_MODE).value + 8).value
    PHX_FGTRIG_EACH_NEG_LEVEL = c_int(c_uint32(etParam.PHX_FGTRIG_MODE).value + 9).value
    PHX_FGTRIG_GATED_POS_LEVEL = c_int(c_uint32(etParam.PHX_FGTRIG_MODE).value + 10).value
    PHX_FGTRIG_GATED_NEG_LEVEL = c_int(c_uint32(etParam.PHX_FGTRIG_MODE).value + 11).value
    PHX_FGTRIG_EACH_POS_EDGE_NR = c_int(c_uint32(etParam.PHX_FGTRIG_MODE).value + 12).value
    PHX_FGTRIG_EACH_NEG_EDGE_NR = c_int(c_uint32(etParam.PHX_FGTRIG_MODE).value + 13).value

    # PHX_FGTRIG_ALIGN
    PHX_FGTRIG_ALIGN_NONE = c_int(c_uint32(etParam.PHX_FGTRIG_ALIGN).value + 1).value
    PHX_FGTRIG_ALIGN_TO_CLK = c_int(c_uint32(etParam.PHX_FGTRIG_ALIGN).value + 2).value
    PHX_FGTRIG_ALIGN_TO_LINE = c_int(c_uint32(etParam.PHX_FGTRIG_ALIGN).value + 3).value
    PHX_FGTRIG_ALIGN_TO_FRAME = c_int(c_uint32(etParam.PHX_FGTRIG_ALIGN).value + 4).value

    # PHX_FGTRIG_DELAY_MODE
    PHX_FGTRIG_DELAY_NONE = c_int(c_uint32(etParam.PHX_FGTRIG_DELAY_MODE).value + 1).value
    PHX_FGTRIG_DELAY_LINE = c_int(c_uint32(etParam.PHX_FGTRIG_DELAY_MODE).value + 2).value
    PHX_FGTRIG_DELAY_TIME = c_int(c_uint32(etParam.PHX_FGTRIG_DELAY_MODE).value + 3).value
    PHX_FGTRIG_DELAY_TIMERM1_CHX = c_int(c_uint32(etParam.PHX_FGTRIG_DELAY_MODE).value + 4).value
    PHX_FGTRIG_DELAY_TIMERM1_CH1 = c_int(c_uint32(etParam.PHX_FGTRIG_DELAY_MODE).value + 5).value
    PHX_FGTRIG_DELAY_TIMERM1_CH2 = c_int(c_uint32(etParam.PHX_FGTRIG_DELAY_MODE).value + 6).value
    PHX_FGTRIG_DELAY_TIMERM1_CH3 = c_int(c_uint32(etParam.PHX_FGTRIG_DELAY_MODE).value + 7).value
    PHX_FGTRIG_DELAY_TIMERM1_CH4 = c_int(c_uint32(etParam.PHX_FGTRIG_DELAY_MODE).value + 8).value

    # PHX_TIMERA1_MODE
    PHX_TIMERA1_MODE_RUN = c_int(c_uint32(etParam.PHX_TIMERA1_MODE).value + 1).value
    PHX_TIMERA1_MODE_N_PULSES = c_int(c_uint32(etParam.PHX_TIMERA1_MODE).value + 2).value
    PHX_TIMERA1_MODE_STOP = c_int(c_uint32(etParam.PHX_TIMERA1_MODE).value + 3).value
    PHX_TIMERA1_MODE_ABORT = c_int(c_uint32(etParam.PHX_TIMERA1_MODE).value + 4).value

    # PHX_TIMERA1_TRIG_SRC
    PHX_TIMERA1_TRIG_SRC_NONE = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 1).value
    PHX_TIMERA1_TRIG_SRC_SWTRIG_CHX = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 2).value
    PHX_TIMERA1_TRIG_SRC_SWTRIG_CH1 = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 3).value
    PHX_TIMERA1_TRIG_SRC_SWTRIG_CH2 = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 4).value
    PHX_TIMERA1_TRIG_SRC_SWTRIG_CH3 = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 5).value
    PHX_TIMERA1_TRIG_SRC_SWTRIG_CH4 = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 6).value
    PHX_TIMERA1_TRIG_SRC_TIMERA1_CH1 = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 7).value
    PHX_TIMERA1_TRIG_SRC_TIMERA1_CH2 = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 8).value
    PHX_TIMERA1_TRIG_SRC_TIMERA1_CH3 = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 9).value
    PHX_TIMERA1_TRIG_SRC_TIMERA1_CH4 = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 10).value
    PHX_TIMERA1_TRIG_SRC_422IN_CHX_0_RISE = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 11).value
    PHX_TIMERA1_TRIG_SRC_422IN_CH1_0_RISE = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 12).value
    PHX_TIMERA1_TRIG_SRC_422IN_CH2_0_RISE = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 13).value
    PHX_TIMERA1_TRIG_SRC_422IN_CH3_0_RISE = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 14).value
    PHX_TIMERA1_TRIG_SRC_422IN_CH4_0_RISE = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 15).value
    PHX_TIMERA1_TRIG_SRC_422IN_CHX_0_FALL = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 16).value
    PHX_TIMERA1_TRIG_SRC_422IN_CH1_0_FALL = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 17).value
    PHX_TIMERA1_TRIG_SRC_422IN_CH2_0_FALL = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 18).value
    PHX_TIMERA1_TRIG_SRC_422IN_CH3_0_FALL = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 19).value
    PHX_TIMERA1_TRIG_SRC_422IN_CH4_0_FALL = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 20).value
    PHX_TIMERA1_TRIG_SRC_OPTOIN_CHX_0_RISE = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 21).value
    PHX_TIMERA1_TRIG_SRC_OPTOIN_CH1_0_RISE = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 22).value
    PHX_TIMERA1_TRIG_SRC_OPTOIN_CH2_0_RISE = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 23).value
    PHX_TIMERA1_TRIG_SRC_OPTOIN_CH3_0_RISE = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 24).value
    PHX_TIMERA1_TRIG_SRC_OPTOIN_CH4_0_RISE = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 25).value
    PHX_TIMERA1_TRIG_SRC_OPTOIN_CHX_0_FALL = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 26).value
    PHX_TIMERA1_TRIG_SRC_OPTOIN_CH1_0_FALL = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 27).value
    PHX_TIMERA1_TRIG_SRC_OPTOIN_CH2_0_FALL = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 28).value
    PHX_TIMERA1_TRIG_SRC_OPTOIN_CH3_0_FALL = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 29).value
    PHX_TIMERA1_TRIG_SRC_OPTOIN_CH4_0_FALL = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 30).value
    PHX_TIMERA1_TRIG_SRC_TTLIN_CHX_0_RISE = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 31).value
    PHX_TIMERA1_TRIG_SRC_TTLIN_CH1_0_RISE = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 32).value
    PHX_TIMERA1_TRIG_SRC_TTLIN_CH2_0_RISE = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 33).value
    PHX_TIMERA1_TRIG_SRC_TTLIN_CH3_0_RISE = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 34).value
    PHX_TIMERA1_TRIG_SRC_TTLIN_CH4_0_RISE = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 35).value
    PHX_TIMERA1_TRIG_SRC_TTLIN_CHX_0_FALL = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 36).value
    PHX_TIMERA1_TRIG_SRC_TTLIN_CH1_0_FALL = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 37).value
    PHX_TIMERA1_TRIG_SRC_TTLIN_CH2_0_FALL = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 38).value
    PHX_TIMERA1_TRIG_SRC_TTLIN_CH3_0_FALL = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 39).value
    PHX_TIMERA1_TRIG_SRC_TTLIN_CH4_0_FALL = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 40).value
    PHX_TIMERA1_TRIG_SRC_24VIN_CHX_0_RISE = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 41).value
    PHX_TIMERA1_TRIG_SRC_24VIN_CHX_0_FALL = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 42).value
    PHX_TIMERA1_TRIG_SRC_24VIN_CH1_0_RISE = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 43).value
    PHX_TIMERA1_TRIG_SRC_24VIN_CH1_0_FALL = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 44).value
    PHX_TIMERA1_TRIG_SRC_24VIN_CH2_0_RISE = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 45).value
    PHX_TIMERA1_TRIG_SRC_24VIN_CH2_0_FALL = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 46).value
    PHX_TIMERA1_TRIG_SRC_24VIN_CH3_0_RISE = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 47).value
    PHX_TIMERA1_TRIG_SRC_24VIN_CH3_0_FALL = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 48).value
    PHX_TIMERA1_TRIG_SRC_24VIN_CH4_0_RISE = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 49).value
    PHX_TIMERA1_TRIG_SRC_24VIN_CH4_0_FALL = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 50).value
    PHX_TIMERA1_TRIG_SRC_24VIN_CH5_0_RISE = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 51).value
    PHX_TIMERA1_TRIG_SRC_24VIN_CH5_0_FALL = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 52).value
    PHX_TIMERA1_TRIG_SRC_24VIN_CH6_0_RISE = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 53).value
    PHX_TIMERA1_TRIG_SRC_24VIN_CH6_0_FALL = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 54).value
    PHX_TIMERA1_TRIG_SRC_24VIN_CH7_0_RISE = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 55).value
    PHX_TIMERA1_TRIG_SRC_24VIN_CH7_0_FALL = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 56).value
    PHX_TIMERA1_TRIG_SRC_24VIN_CH8_0_RISE = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 57).value
    PHX_TIMERA1_TRIG_SRC_24VIN_CH8_0_FALL = c_int(c_uint32(etParam.PHX_TIMERA1_TRIG_SRC).value + 58).value


    # PHX_IMAGE_TIMESTAMP_MODE
    PHX_IMAGE_TIMESTAMP_NONE = c_int(c_uint32(etParam.PHX_IMAGE_TIMESTAMP_MODE).value + 1).value
    PHX_IMAGE_TIMESTAMP_MODE1 = c_int(c_uint32(etParam.PHX_IMAGE_TIMESTAMP_MODE).value + 2).value
    PHX_IMAGE_TIMESTAMP_MODE2 = c_int(c_uint32(etParam.PHX_IMAGE_TIMESTAMP_MODE).value + 3).value

    # PHX_EVENTCOUNT_SRC
    PHX_EVENTCOUNT_LINE = c_int(c_uint32(etParam.PHX_EVENTCOUNT_SRC).value + 1).value
    PHX_EVENTCOUNT_FRAME = c_int(c_uint32(etParam.PHX_EVENTCOUNT_SRC).value + 2).value
    PHX_EVENTCOUNT_TIME = c_int(c_uint32(etParam.PHX_EVENTCOUNT_SRC).value + 3).value

    # PHX_EVENTGATE_SRC
    PHX_EVENTGATE_ACQTRIG = c_int(c_uint32(etParam.PHX_EVENTGATE_SRC).value + 1).value
    PHX_EVENTGATE_FRAME = c_int(c_uint32(etParam.PHX_EVENTGATE_SRC).value + 2).value
    PHX_EVENTGATE_ACQ = c_int(c_uint32(etParam.PHX_EVENTGATE_SRC).value + 3).value
    PHX_EVENTGATE_LINE = c_int(c_uint32(etParam.PHX_EVENTGATE_SRC).value + 4).value

    # PHX_DST_ENDIAN
    PHX_DST_LITTLE_ENDIAN = c_int(c_uint32(etParam.PHX_DST_ENDIAN).value + 1).value
    PHX_DST_BIG_ENDIAN = c_int(c_uint32(etParam.PHX_DST_ENDIAN).value + 2).value

    # PHX_CHAN_SYNC_MODE
    PHX_CHAN_SYNC_NONE = c_int(c_uint32(etParam.PHX_CHAN_SYNC_MODE).value + 1).value
    PHX_CHAN_SYNC_ACQEXPTRIG = c_int(c_uint32(etParam.PHX_CHAN_SYNC_MODE).value + 2).value

    # PHX_CVB_PARAM
    PHX_CVB_WIDTH = c_int(c_uint32(etParam.PHX_CVB_PARAM).value + 1).value
    PHX_CVB_HEIGHT = c_int(c_uint32(etParam.PHX_CVB_PARAM).value + 2).value
    PHX_CVB_PLANES = c_int(c_uint32(etParam.PHX_CVB_PARAM).value + 3).value
    PHX_CVB_BIT_DEPTH = c_int(c_uint32(etParam.PHX_CVB_PARAM).value + 4).value
    PHX_CVB_BYTES_PER_PIXEL = c_int(c_uint32(etParam.PHX_CVB_PARAM).value + 5).value
    PHX_CVB_X_STEP = c_int(c_uint32(etParam.PHX_CVB_PARAM).value + 6).value
    PHX_CVB_Y_STEP = c_int(c_uint32(etParam.PHX_CVB_PARAM).value + 7).value
    PHX_CVB_PLANE_STEP = c_int(c_uint32(etParam.PHX_CVB_PARAM).value + 8).value
    PHX_CVB_MALLOC = c_int(c_uint32(etParam.PHX_CVB_PARAM).value + 9).value

    # PHX_ACQ_AUTO_RESTART
    PHX_ACQ_AUTO_NONE = c_int(c_uint32(etParam.PHX_ACQ_AUTO_RESTART).value + 0x01).value
    PHX_ACQ_AUTO_SYNC_LOST = c_int(c_uint32(etParam.PHX_ACQ_AUTO_RESTART).value + 0x02).value
    PHX_ACQ_AUTO_FIFO_OVERFLOW = c_int(c_uint32(etParam.PHX_ACQ_AUTO_RESTART).value + 0x04).value

    # PHX_BOARD_VARIANT
    # PHX_BOARD_VARIANT
    PHX_BOARD_DIGITAL = c_int(c_uint32(etCamConfigLoad.PHX_DIGITAL).value).value
    PHX_BOARD_PHX_D24CL_PE1 = c_int(c_uint32(etCamConfigLoad.PHX_D24CL_PE1).value).value
    PHX_BOARD_PHX_D24CL_PE1_MIR = c_int(c_uint32(etCamConfigLoad.PHX_D24CL_PE1_MIR).value).value
    PHX_BOARD_PHX_D48CL_PE1 = c_int(c_uint32(etCamConfigLoad.PHX_D48CL_PE1).value).value
    PHX_BOARD_PHX_D48CL_PE4 = c_int(c_uint32(etCamConfigLoad.PHX_D48CL_PE4).value).value
    PHX_BOARD_PHX_D64CL_PE4 = c_int(c_uint32(etCamConfigLoad.PHX_D64CL_PE4).value).value
    PHX_BOARD_PHX_D24CL_PCI32 = c_int(c_uint32(etCamConfigLoad.PHX_D24CL_PCI32).value).value
    PHX_BOARD_PHX_D48CL_PCI32 = c_int(c_uint32(etCamConfigLoad.PHX_D48CL_PCI32).value).value
    PHX_BOARD_PHX_D48CL_PCI64 = c_int(c_uint32(etCamConfigLoad.PHX_D48CL_PCI64).value).value
    PHX_BOARD_PHX_D48CL_PCI64U = c_int(c_uint32(etCamConfigLoad.PHX_D48CL_PCI64U).value).value
    PHX_BOARD_PHX_D10HDSDI_PE1 = c_int(c_uint32(etCamConfigLoad.PHX_D10HDSDI_PE1).value).value
    PHX_BOARD_PHX_D20HDSDI_PE1 = c_int(c_uint32(etCamConfigLoad.PHX_D20HDSDI_PE1).value).value
    PHX_BOARD_PHX_D10HDSDI_PE4 = c_int(c_uint32(etCamConfigLoad.PHX_D10HDSDI_PE4).value).value
    PHX_BOARD_PHX_D20HDSDI_PE4 = c_int(c_uint32(etCamConfigLoad.PHX_D20HDSDI_PE4).value).value
    PHX_BOARD_PHX_D36_PE1 = c_int(c_uint32(etCamConfigLoad.PHX_D36_PE1).value).value
    PHX_BOARD_PHX_D36_PE4 = c_int(c_uint32(etCamConfigLoad.PHX_D36_PE4).value).value
    PHX_BOARD_PHX_D32_PCI32 = c_int(c_uint32(etCamConfigLoad.PHX_D32_PCI32).value).value
    PHX_BOARD_PHX_D36_PCI32 = c_int(c_uint32(etCamConfigLoad.PHX_D36_PCI32).value).value
    PHX_BOARD_PHX_D36_PCI64 = c_int(c_uint32(etCamConfigLoad.PHX_D36_PCI64).value).value
    PHX_BOARD_PHX_D36_PCI64U = c_int(c_uint32(etCamConfigLoad.PHX_D36_PCI64U).value).value
    PHX_BOARD_PHX_D24AVDS_PE1 = c_int(c_uint32(etCamConfigLoad.PHX_D24AVDS_PE1).value).value
    PHX_BOARD_FBD_1XCLD_2PE8 = PHX_BOARD_PHX_D24AVDS_PE1 + 1
    PHX_BOARD_FBD_4XCXP6_2PE8 = PHX_BOARD_FBD_1XCLD_2PE8 + 1
    PHX_BOARD_FBD_2XCLD_2PE8 = PHX_BOARD_FBD_4XCXP6_2PE8 + 1
    PHX_BOARD_FBD_1XCXP6_2PE8 = PHX_BOARD_FBD_2XCLD_2PE8 + 1
    PHX_BOARD_FBD_2XCXP6_2PE8 = PHX_BOARD_FBD_1XCXP6_2PE8 + 1
    PHX_BOARD_FBD_1XCLM_2PE8 = PHX_BOARD_FBD_2XCXP6_2PE8 + 1
    PHX_BOARD_FBD_2XCLM_2PE8 = PHX_BOARD_FBD_1XCLM_2PE8 + 1
    PHX_BOARD_FBD_1XCLD_TTLA_2PE8 = PHX_BOARD_FBD_2XCLM_2PE8 + 1
    PHX_BOARD_FBD_4XCXP6_TTLA_2PE8 = PHX_BOARD_FBD_1XCLD_TTLA_2PE8 + 1
    PHX_BOARD_FBD_1XCLM_TTLA_2PE8 = PHX_BOARD_FBD_4XCXP6_TTLA_2PE8 + 1
    PHX_BOARD_FBD_1XCLD_2PE8_MIR = PHX_BOARD_FBD_1XCLM_TTLA_2PE8 + 1
    PHX_BOARD_FBD_4XCXP6_2PE8_MIR = PHX_BOARD_FBD_1XCLD_2PE8_MIR + 1
    PHX_BOARD_FBD_1XCLM_2PE8_MIR = PHX_BOARD_FBD_4XCXP6_2PE8_MIR + 1
    PHX_BOARD_FBD_1XCXPLR1_2PE8 = PHX_BOARD_FBD_1XCLM_2PE8_MIR + 1
    PHX_BOARD_FBD_1XCLD_2PE4 = PHX_BOARD_FBD_1XCXPLR1_2PE8 + 1
    PHX_BOARD_FBD_1XCLD_2PE4_SW = PHX_BOARD_FBD_1XCLD_2PE4 + 1
    PHX_BOARD_FBD_1XCLD_2PE4L = PHX_BOARD_FBD_1XCLD_2PE4_SW + 1
    PHX_BOARD_FBD_1XCLD_3CPCIS_2PE4 = PHX_BOARD_FBD_1XCLD_2PE4L + 1
    PHX_BOARD_FBD_1XCXP6_2PE8_MIR = PHX_BOARD_FBD_1XCLD_3CPCIS_2PE4 + 1
    PHX_BOARD_FBD_4XCXP6_3PE4 = PHX_BOARD_FBD_1XCXP6_2PE8_MIR + 1
    PHX_BOARD_FBD_1XCXP6_2PE4L = PHX_BOARD_FBD_4XCXP6_3PE4 + 1
    PHX_BOARD_FBD_4XCXP6_2PE8_MIR_A = PHX_BOARD_FBD_1XCXP6_2PE4L + 1
    PHX_BOARD_FBD_4XCXP12_3PE4 = PHX_BOARD_FBD_4XCXP6_2PE8_MIR_A + 1
    PHX_BOARD_FBD_4XCXP12_3PE8 = PHX_BOARD_FBD_4XCXP12_3PE4 + 1
    PHX_BOARD_FBD_4XCXP12_3PE8_MIR = PHX_BOARD_FBD_4XCXP12_3PE8 + 1

    # PHX_BOARD_NUMBER
    PHX_BOARD_NUMBER_AUTO = c_int(c_uint32(etCamConfigLoad.PHX_BOARD_AUTO).value).value
    PHX_BOARD_NUMBER_1 = c_int(c_uint32(etCamConfigLoad.PHX_BOARD1).value).value
    PHX_BOARD_NUMBER_2 = c_int(c_uint32(etCamConfigLoad.PHX_BOARD2).value).value
    PHX_BOARD_NUMBER_3 = c_int(c_uint32(etCamConfigLoad.PHX_BOARD3).value).value
    PHX_BOARD_NUMBER_4 = c_int(c_uint32(etCamConfigLoad.PHX_BOARD4).value).value
    PHX_BOARD_NUMBER_5 = c_int(c_uint32(etCamConfigLoad.PHX_BOARD5).value).value
    PHX_BOARD_NUMBER_6 = c_int(c_uint32(etCamConfigLoad.PHX_BOARD6).value).value
    PHX_BOARD_NUMBER_7 = c_int(c_uint32(etCamConfigLoad.PHX_BOARD7).value).value

    # PHX_CHANNEL_NUMBER
    PHX_CHANNEL_NUMBER_AUTO = c_int(c_uint32(etCamConfigLoad.PHX_CHANNEL_AUTO).value).value
    PHX_CHANNEL_NUMBER_1 = c_int(c_uint32(etCamConfigLoad.PHX_CHANNEL_A).value).value
    PHX_CHANNEL_NUMBER_2 = c_int(c_uint32(etCamConfigLoad.PHX_CHANNEL_B).value).value
    PHX_CHANNEL_NUMBER_3 = c_int(c_uint32(etCamConfigLoad.PHX_CHANNEL_B).value + 1).value
    PHX_CHANNEL_NUMBER_4 = c_int(c_uint32(etCamConfigLoad.PHX_CHANNEL_B).value + 2).value
    PHX_CHANNEL_NUMBER_5 = c_int(c_uint32(etCamConfigLoad.PHX_CHANNEL_B).value + 3).value
    PHX_CHANNEL_NUMBER_6 = c_int(c_uint32(etCamConfigLoad.PHX_CHANNEL_B).value + 4).value
    PHX_CHANNEL_NUMBER_7 = c_int(c_uint32(etCamConfigLoad.PHX_CHANNEL_B).value + 5).value
    PHX_CHANNEL_NUMBER_8 = c_int(c_uint32(etCamConfigLoad.PHX_CHANNEL_B).value + 6).value

    # PHX_CONFIG_MODE
    PHX_CONFIG_NORMAL = c_int(c_uint32(etCamConfigLoad.PHX_MODE_NORMAL).value).value
    PHX_CONFIG_COMMS_ONLY = c_int(c_uint32(etCamConfigLoad.PHX_COMMS_ONLY).value).value
    PHX_CONFIG_ACQ_ONLY = c_int(c_uint32(etCamConfigLoad.PHX_ACQ_ONLY).value).value
    PHX_CONFIG_NO_CHILDREN = c_int(c_uint32(etCamConfigLoad.PHX_COMMS_ONLY).value | c_uint32(etCamConfigLoad.PHX_ACQ_ONLY).value).value

    # PHX_BOARD_BUS_ADDRESS
    PHX_BOARD_INVALID_BUS_ADDRESS = c_int(0x80000000).value

    # PHX_CXP_BITRATE_MODE
    PHX_CXP_BITRATE_MODE_AUTO = c_int(c_uint32(etParam.PHX_CXP_BITRATE_MODE).value + 1).value
    PHX_CXP_BITRATE_MODE_CXP1 = c_int(c_uint32(etParam.PHX_CXP_BITRATE_MODE).value + 2).value
    PHX_CXP_BITRATE_MODE_CXP2 = c_int(c_uint32(etParam.PHX_CXP_BITRATE_MODE).value + 3).value
    PHX_CXP_BITRATE_MODE_CXP3 = c_int(c_uint32(etParam.PHX_CXP_BITRATE_MODE).value + 4).value
    PHX_CXP_BITRATE_MODE_CXP5 = c_int(c_uint32(etParam.PHX_CXP_BITRATE_MODE).value + 5).value
    PHX_CXP_BITRATE_MODE_CXP6 = c_int(c_uint32(etParam.PHX_CXP_BITRATE_MODE).value + 6).value
    PHX_CXP_BITRATE_MODE_CXP10 = c_int(c_uint32(etParam.PHX_CXP_BITRATE_MODE).value + 7).value
    PHX_CXP_BITRATE_MODE_CXP12 = c_int(c_uint32(etParam.PHX_CXP_BITRATE_MODE).value + 8).value

    # PHX_CXP_BITRATE
    PHX_CXP_BITRATE_UNKNOWN = c_int(c_uint32(etParam.PHX_CXP_BITRATE).value + 1).value
    PHX_CXP_BITRATE_CXP1 = PHX_CXP_BITRATE_MODE_CXP1
    PHX_CXP_BITRATE_CXP2 = PHX_CXP_BITRATE_MODE_CXP2
    PHX_CXP_BITRATE_CXP3 = PHX_CXP_BITRATE_MODE_CXP3
    PHX_CXP_BITRATE_CXP5 = PHX_CXP_BITRATE_MODE_CXP5
    PHX_CXP_BITRATE_CXP6 = PHX_CXP_BITRATE_MODE_CXP6
    PHX_CXP_BITRATE_CXP10 = PHX_CXP_BITRATE_MODE_CXP10
    PHX_CXP_BITRATE_CXP12 = PHX_CXP_BITRATE_MODE_CXP12

    # PHX_CXP_POCXP_MODE
    PHX_CXP_POCXP_MODE_AUTO = c_int(c_uint32(etParam.PHX_CXP_POCXP_MODE).value + 1).value
    PHX_CXP_POCXP_MODE_OFF = c_int(c_uint32(etParam.PHX_CXP_POCXP_MODE).value + 2).value
    PHX_CXP_POCXP_MODE_TRIP_RESET = c_int(c_uint32(etParam.PHX_CXP_POCXP_MODE).value + 3).value
    PHX_CXP_POCXP_MODE_FORCEON = c_int(c_uint32(etParam.PHX_CXP_POCXP_MODE).value + 4).value

    # PHX_CL_POCL_MODE
    PHX_CL_POCL_MODE_AUTO = c_int(c_uint32(etParam.PHX_CL_POCL_MODE).value + 1).value
    PHX_CL_POCL_MODE_OFF = c_int(c_uint32(etParam.PHX_CL_POCL_MODE).value + 2).value
    PHX_CL_POCL_MODE_TRIP_RESET = c_int(c_uint32(etParam.PHX_CL_POCL_MODE).value + 3).value

    # PHX_CXP_DISCOVERY_MODE
    PHX_CXP_DISCOVERY_MODE_AUTO = c_int(c_uint32(etParam.PHX_CXP_DISCOVERY_MODE).value + 1).value
    PHX_CXP_DISCOVERY_MODE_1X = c_int(c_uint32(etParam.PHX_CXP_DISCOVERY_MODE).value + 2).value
    PHX_CXP_DISCOVERY_MODE_2X = c_int(c_uint32(etParam.PHX_CXP_DISCOVERY_MODE).value + 3).value
    PHX_CXP_DISCOVERY_MODE_4X = c_int(c_uint32(etParam.PHX_CXP_DISCOVERY_MODE).value + 4).value

    # PHX_CXP_DISCOVERY
    PHX_CXP_DISCOVERY_UNKNOWN = c_int(c_uint32(etParam.PHX_CXP_DISCOVERY).value + 1).value
    PHX_CXP_DISCOVERY_1X = PHX_CXP_DISCOVERY_MODE_1X
    PHX_CXP_DISCOVERY_2X = PHX_CXP_DISCOVERY_MODE_2X
    PHX_CXP_DISCOVERY_4X = PHX_CXP_DISCOVERY_MODE_4X

    # PHX_DST_ALIGMNENT
    PHX_DST_LSB_ALIGNED = c_int(c_uint32(etParam.PHX_DST_ALIGNMENT).value + 1).value
    PHX_DST_MSB_ALIGNED = c_int(c_uint32(etParam.PHX_DST_ALIGNMENT).value + 2).value

    # PHX_ACQ_BUFFER_MEMSET_MODE
    PHX_ACQ_BUFFER_MEMSET_NONE = c_int(c_uint32(etParam.PHX_ACQ_BUFFER_MEMSET_MODE).value + 1).value
    PHX_ACQ_BUFFER_MEMSET_START = c_int(c_uint32(etParam.PHX_ACQ_BUFFER_MEMSET_MODE).value + 2).value
    PHX_ACQ_BUFFER_MEMSET_ALWAYS = c_int(c_uint32(etParam.PHX_ACQ_BUFFER_MEMSET_MODE).value + 3).value

    # PHX_INTRPT_CONDITIONS
    PHX_INTRPT_TEST = 0x00000001
    PHX_INTRPT_BUFFER_READY = 0x00000002
    PHX_INTRPT_FIFO_OVERFLOW = 0x00000004
    PHX_INTRPT_FRAME_LOST = 0x00000008
    PHX_INTRPT_CAPTURE_COMPLETE = 0x00000010
    PHX_INTRPT_FRAME_START = 0x00000020
    PHX_INTRPT_FRAME_END = 0x00000040
    PHX_INTRPT_LINE_START = 0x00000080
    PHX_INTRPT_LINE_END = 0x00000100
    PHX_INTRPT_FGTRIG_START = 0x00000200
    PHX_INTRPT_FGTRIG_END = 0x00000400
    PHX_INTRPT_TIMEOUT = 0x00000800
    PHX_INTRPT_SYNC_LOST = 0x00001000
    PHX_INTRPT_TIMERA1 = 0x00002000
    PHX_INTRPT_CXP_ERROR = 0x00004000
    PHX_INTRPT_TIMERM1 = 0x00010000
    PHX_INTRPT_TIMERM2 = 0x00020000
    PHX_INTRPT_CAMERA_TRIGGER = 0x00040000
    PHX_INTRPT_GLOBAL_ENABLE = c_int(0x80000000).value

class etPhxIOMethod:
    PHX_EMASK_IO_METHOD = 0x3F000000
    PHX_IO_METHOD_WRITE = 0x00000000
    PHX_IO_METHOD_READ = 0x00000000
    PHX_IO_METHOD_BIT_SET = 0x01000000
    PHX_IO_METHOD_BIT_CLR = 0x02000000
    PHX_IO_METHOD_BIT_TIMERMX_POS = 0x04000000
    PHX_IO_METHOD_BIT_FGTRIGD_POS = 0x05000000
    PHX_IO_METHOD_BIT_FIFO_WARN_POS = 0x06000000
    PHX_IO_METHOD_BIT_TIMERMX_NEG = 0x08000000
    PHX_IO_METHOD_BIT_FGTRIGD_NEG = 0x09000000
    PHX_IO_METHOD_BIT_FIFO_WARN_NEG = 0x0A000000
    PHX_IO_METHOD_BIT_TIMERM1_POS_CHX = 0x10000000
    PHX_IO_METHOD_BIT_TIMERM1_NEG_CHX = 0x11000000
    PHX_IO_METHOD_BIT_TIMERM1_POS_CH1 = 0x12000000
    PHX_IO_METHOD_BIT_TIMERM1_NEG_CH1 = 0x13000000
    PHX_IO_METHOD_BIT_TIMERM1_POS_CH2 = 0x14000000
    PHX_IO_METHOD_BIT_TIMERM1_NEG_CH2 = 0x15000000
    PHX_IO_METHOD_BIT_TIMERM1_POS_CH3 = 0x16000000
    PHX_IO_METHOD_BIT_TIMERM1_NEG_CH3 = 0x17000000
    PHX_IO_METHOD_BIT_TIMERM1_POS_CH4 = 0x18000000
    PHX_IO_METHOD_BIT_TIMERM1_NEG_CH4 = 0x19000000
    PHX_IO_METHOD_BIT_TIMERM2_POS_CHX = 0x1A000000
    PHX_IO_METHOD_BIT_TIMERM2_NEG_CHX = 0x1B000000
    PHX_IO_METHOD_BIT_TIMERM2_POS_CH1 = 0x1C000000
    PHX_IO_METHOD_BIT_TIMERM2_NEG_CH1 = 0x1D000000
    PHX_IO_METHOD_BIT_TIMERM2_POS_CH2 = 0x1E000000
    PHX_IO_METHOD_BIT_TIMERM2_NEG_CH2 = 0x1F000000
    PHX_IO_METHOD_BIT_TIMERM2_POS_CH3 = 0x20000000
    PHX_IO_METHOD_BIT_TIMERM2_NEG_CH3 = 0x21000000
    PHX_IO_METHOD_BIT_TIMERM2_POS_CH4 = 0x22000000
    PHX_IO_METHOD_BIT_TIMERM2_NEG_CH4 = 0x23000000
    PHX_IO_METHOD_BIT_FGTRIGD_POS_CHX = 0x24000000
    PHX_IO_METHOD_BIT_FGTRIGD_NEG_CHX = 0x25000000
    PHX_IO_METHOD_BIT_FGTRIGD_POS_CH1 = 0x26000000
    PHX_IO_METHOD_BIT_FGTRIGD_NEG_CH1 = 0x27000000
    PHX_IO_METHOD_BIT_FGTRIGD_POS_CH2 = 0x28000000
    PHX_IO_METHOD_BIT_FGTRIGD_NEG_CH2 = 0x29000000
    PHX_IO_METHOD_BIT_FGTRIGD_POS_CH3 = 0x2A000000
    PHX_IO_METHOD_BIT_FGTRIGD_NEG_CH3 = 0x2B000000
    PHX_IO_METHOD_BIT_FGTRIGD_POS_CH4 = 0x2C000000
    PHX_IO_METHOD_BIT_FGTRIGD_NEG_CH4 = 0x2D000000

    # Retained for backwards compatibility with previous software releases
    PHX_IO_METHOD_BIT_TIMER_POS = 0x04000000
    PHX_IO_METHOD_BIT_HW_POS = 0x04000000
    PHX_IO_METHOD_BIT_ACQTRIG_POS = 0x05000000
    PHX_IO_METHOD_BIT_TIMER_NEG = 0x08000000
    PHX_IO_METHOD_BIT_HW_NEG = 0x08000000
    PHX_IO_METHOD_BIT_ACQTRIG_NEG = 0x09000000
class etBoardInfo:
    PHX_BOARD_INFO_PCI_3V = 0x00000001
    PHX_BOARD_INFO_PCI_5V = 0x00000002
    PHX_BOARD_INFO_PCI_33M = 0x00000004
    PHX_BOARD_INFO_PCI_66M = 0x00000008
    PHX_BOARD_INFO_PCI_32B = 0x00000010
    PHX_BOARD_INFO_PCI_64B = 0x00000020
    PHX_BOARD_INFO_LVDS = 0x00000040
    PHX_BOARD_INFO_CL = 0x00000080
    PHX_BOARD_INFO_CHAIN_MASTER = 0x00000100
    PHX_BOARD_INFO_CHAIN_SLAVE = 0x00000200
    PHX_BOARD_INFO_PCI_EXPRESS = 0x00000400
    PHX_BOARD_INFO_CL_BASE = 0x00000800
    PHX_BOARD_INFO_CL_MEDIUM = 0x00001000
    PHX_BOARD_INFO_CL_FULL = 0x00002000
    PHX_BOARD_INFO_BOARD_3V = 0x00010000
    PHX_BOARD_INFO_BOARD_5V = 0x00020000
    PHX_BOARD_INFO_BOARD_33M = 0x00040000
    PHX_BOARD_INFO_BOARD_66M = 0x00080000
    PHX_BOARD_INFO_BOARD_32B = 0x00100000
    PHX_BOARD_INFO_BOARD_64B = 0x00200000

class etPcieInfo:
    PHX_CXP_LINKS_MAX = 4
    PHX_EMASK_PCIE_INFO_LINK_GEN = 0x00000003
    PHX_PCIE_INFO_UNKNOWN = 0x00000000
    PHX_PCIE_INFO_LINK_GEN1 = 0x00000001
    PHX_PCIE_INFO_LINK_GEN2 = 0x00000002
    PHX_PCIE_INFO_LINK_GEN3 = 0x00000003
    PHX_EMASK_PCIE_INFO_LINK_X = 0x0000001C
    PHX_PCIE_INFO_LINK_X1 = 0x00000004
    PHX_PCIE_INFO_LINK_X2 = 0x00000008
    PHX_PCIE_INFO_LINK_X4 = 0x0000000C
    PHX_PCIE_INFO_LINK_X8 = 0x00000010
    PHX_PCIE_INFO_LINK_X12 = 0x00000014
    PHX_PCIE_INFO_LINK_X16 = 0x00000018
    PHX_PCIE_INFO_LINK_X32 = 0x0000001C
    PHX_EMASK_PCIE_INFO_FG_GEN = 0x00000300
    PHX_PCIE_INFO_FG_GEN1 = 0x00000100
    PHX_PCIE_INFO_FG_GEN2 = 0x00000200
    PHX_PCIE_INFO_FG_GEN3 = 0x00000300
    PHX_EMASK_PCIE_INFO_FG_X = 0x00001C00
    PHX_PCIE_INFO_FG_X1 = 0x00000400
    PHX_PCIE_INFO_FG_X2 = 0x00000800
    PHX_PCIE_INFO_FG_X4 = 0x00000C00
    PHX_PCIE_INFO_FG_X8 = 0x00001000
    PHX_PCIE_INFO_FG_X12 = 0x00001400
    PHX_PCIE_INFO_FG_X16 = 0x00001800
    PHX_PCIE_INFO_FG_X32 = 0x00001C00
    PHX_EMASK_PCIE_INFO_SLOT_GEN = 0x00030000
    PHX_PCIE_INFO_SLOT_GEN1 = 0x00010000
    PHX_PCIE_INFO_SLOT_GEN2 = 0x00020000
    PHX_PCIE_INFO_SLOT_GEN3 = 0x00030000
    PHX_EMASK_PCIE_INFO_SLOT_X = 0x001C0000
    PHX_PCIE_INFO_SLOT_X1 = 0x00040000
    PHX_PCIE_INFO_SLOT_X2 = 0x00080000
    PHX_PCIE_INFO_SLOT_X4 = 0x000C0000
    PHX_PCIE_INFO_SLOT_X8 = 0x00100000
    PHX_PCIE_INFO_SLOT_X12 = 0x00140000
    PHX_PCIE_INFO_SLOT_X16 = 0x00180000
    PHX_PCIE_INFO_SLOT_X32 = 0x001C0000

class etCxpInfo:
    PHX_CXP_CAMERA_DISCOVERED = 0x00000001
    PHX_CXP_CAMERA_IS_POCXP = 0x00000002
    PHX_CXP_POCXP_UNAVAILABLE = 0x00000004
    PHX_CXP_POCXP_TRIPPED = 0x00000008
    PHX_CXP_LINK1_USED = 0x00000100
    PHX_CXP_LINK2_USED = 0x00000200
    PHX_CXP_LINK3_USED = 0x00000400
    PHX_CXP_LINK4_USED = 0x00000800
    PHX_CXP_LINK1_MASTER = 0x00010000
    PHX_CXP_LINK2_MASTER = 0x00020000
    PHX_CXP_LINK3_MASTER = 0x00040000
    PHX_CXP_LINK4_MASTER = 0x00080000

class etClInfo:
    PHX_CL_CAMERA_CONNECTED = 0x00000001
    PHX_CL_CAMERA_IS_POCL = 0x00000002
    PHX_CL_POCL_UNAVAILABLE = 0x00000004
    PHX_CL_POCL_TRIPPED = 0x00000008

class etAcq:
    PHX_START = _PHX_ENUM(etPhxFn.PHX_ACQUIRE, 1)
    PHX_CHECK_AND_WAIT = _PHX_ENUM(etPhxFn.PHX_ACQUIRE, 2)
    PHX_CHECK_AND_RETURN = _PHX_ENUM(etPhxFn.PHX_ACQUIRE, 3)
    PHX_STOP = _PHX_ENUM(etPhxFn.PHX_ACQUIRE, 4)
    PHX_ABORT = _PHX_ENUM(etPhxFn.PHX_ACQUIRE, 5)
    PHX_BUFFER_GET = _PHX_ENUM(etPhxFn.PHX_ACQUIRE, 6)
    PHX_BUFFER_RELEASE = _PHX_ENUM(etPhxFn.PHX_ACQUIRE, 7)
    PHX_BUFFER_ABORT = _PHX_ENUM(etPhxFn.PHX_ACQUIRE, 8)
    PHX_EVENT_HANDLER = _PHX_ENUM(etPhxFn.PHX_ACQUIRE, 9)
    PHX_START_IMMEDIATELY = _PHX_ENUM(etPhxFn.PHX_ACQUIRE, 10)
    PHX_SWTRIG = _PHX_ENUM(etPhxFn.PHX_ACQUIRE, 11)
    PHX_UNLOCK = _PHX_ENUM(etPhxFn.PHX_ACQUIRE, 12)
    PHX_AUTO_WHITE_BALANCE = _PHX_ENUM(etPhxFn.PHX_ACQUIRE, 13)
    PHX_AUTO_RESTART = _PHX_ENUM(etPhxFn.PHX_ACQUIRE, 14)
    PHX_BUFFER_GET_MERGED = _PHX_ENUM(etPhxFn.PHX_ACQUIRE, 15)
    PHX_USER_LOCK = _PHX_ENUM(etPhxFn.PHX_ACQUIRE, 16)
    PHX_USER_UNLOCK = _PHX_ENUM(etPhxFn.PHX_ACQUIRE, 17)
    PHX_BUFFER_OBJECT_GET = _PHX_ENUM(etPhxFn.PHX_ACQUIRE, 18)
    PHX_BUFFER_MAKE_READY = _PHX_ENUM(etPhxFn.PHX_ACQUIRE, 19)

    PHX_CXP_LINKTEST_UPLINK_OK_RST = _PHX_ENUM(etPhxFn.PHX_ACQUIRE, 128)
    PHX_CXP_LINKTEST_UPLINK_ERR_RST = _PHX_ENUM(etPhxFn.PHX_ACQUIRE, 129)
    PHX_CXP_LINKTEST_DOWNLINK_OK_RST = _PHX_ENUM(etPhxFn.PHX_ACQUIRE, 130)
    PHX_CXP_LINKTEST_DOWNLINK_ERR_RST = _PHX_ENUM(etPhxFn.PHX_ACQUIRE, 131)
    PHX_CXP_ERRCOUNT_LINK_LOSS_RST = _PHX_ENUM(etPhxFn.PHX_ACQUIRE, 132)
    PHX_CXP_ERRCOUNT_8B10B_RST = _PHX_ENUM(etPhxFn.PHX_ACQUIRE, 133)
    PHX_CXP_ERRCOUNT_CRC_STREAM_RST = _PHX_ENUM(etPhxFn.PHX_ACQUIRE, 134)
    PHX_CXP_ERRCOUNT_CRC_CTRL_RST = _PHX_ENUM(etPhxFn.PHX_ACQUIRE, 135)
    PHX_CXP_ERRCOUNT_CRC_EVENT_RST = _PHX_ENUM(etPhxFn.PHX_ACQUIRE, 136)
    PHX_CXP_ERRCOUNT_DUP_FIXED_RST = _PHX_ENUM(etPhxFn.PHX_ACQUIRE, 137)
    PHX_CXP_ERRCOUNT_DUP_NFIXED_RST = _PHX_ENUM(etPhxFn.PHX_ACQUIRE, 138)
    PHX_CXP_ERRCOUNT_FEC_FIXED_RST = _PHX_ENUM(etPhxFn.PHX_ACQUIRE, 139)
    PHX_CXP_ERRCOUNT_FEC_NFIXED_RST = _PHX_ENUM(etPhxFn.PHX_ACQUIRE, 140)

    # For backwards compatibility
    PHX_EXPOSE = PHX_SWTRIG
#
# /* This structure is used to specify the address and context of a block of
#  * memory (used to specify user allocated image buffers). It can be used
#  * either for virtual or physical addresses.
#  */
class stImageBuff(Structure):
    _fields_ = [
        ("pvAddress", c_void_p),
        ("pvContext", c_void_p)
    ]

# /* This structure is used to specify the address, size and context of a
#  * block of memory (used to specify user locked image buffers).
#  */
class stUserBuff(Structure):
    _fields_ = [
        ("pvAddress", c_void_p),
        ("qwSizeBytes", c_uint64),
        ("pvContext", c_void_p)
    ]

# /* This structure is used to specify address and lngth of physical blocks of memory.
#  */
class stPhysBuff(Structure):
    _fields_ = [
        ("uiAddress", c_ulonglong),  # Assuming tPhxPhysAddr is a 64-bit address
        ("uiLength", c_uint32)       # Assuming tPhxPhysLen is a 32-bit length
    ]

class stPhxTimeStamp(Structure):
    _fields_ = [
        ("qwTime", c_uint64),
        ("qwEvent", c_uint64)
    ]

class stPhxCxpHeader(Structure):
    _fields_ = [
        ("uiHeaderIndication", c_uint32),
        ("uiStreamID", c_uint32),
        ("uiSrcTag", c_uint32),
        ("uiSizeX", c_uint32),
        ("uiSizeY", c_uint32),
        ("uiOffsetX", c_uint32),
        ("uiOffsetY", c_uint32),
        ("uiDWordCount", c_uint32),
        ("uiPixelFormat", c_uint32),
        ("uiTapGeometry", c_uint32),
        ("uiFlags", c_uint32)
    ]

class etBufferParam:
    PHX_BUFFER_VIRTUAL_ADDR = 0
    PHX_BUFFER_CONTEXT = 1
    PHX_BUFFER_SEQUENCE_TAG = 2
    PHX_BUFFER_TIMESTAMP = 3
    PHX_BUFFER_NUM_TIMESTAMPS = 4
    PHX_BUFFER_TIMESTAMP_ADDR = 5

# Constants
PHX_MAX_BUFFS = 0x00007FFF
PHX_BUFFS_MASK = 0x00007FFF
PHX_MAX_UINT64 = c_uint64(-1).value

# Enum for Timeouts
class eTimeouts:
    PHX_TIMEOUT_IMMEDIATE = 0
    PHX_TIMEOUT_INFINITE = -1
    PHX_TIMEOUT_PROTOCOL = -2

# Enum for Status Definitions
class eStat:
    PHX_OK = 0
    PHX_ERROR_BAD_HANDLE = 1
    PHX_ERROR_BAD_PARAM = 2
    PHX_ERROR_BAD_PARAM_VALUE = 3
    PHX_ERROR_READ_ONLY_PARAM = 4
    PHX_ERROR_OPEN_FAILED = 5
    PHX_ERROR_INCOMPATIBLE = 6
    PHX_ERROR_HANDSHAKE = 7
    PHX_ERROR_INTERNAL_ERROR = 8
    PHX_ERROR_OVERFLOW = 9
    PHX_ERROR_NOT_IMPLEMENTED = 10
    PHX_ERROR_HW_PROBLEM = 11
    PHX_ERROR_NOT_SUPPORTED = 12
    PHX_ERROR_OUT_OF_RANGE = 13
    PHX_ERROR_MALLOC_FAILED = 14
    PHX_ERROR_SYSTEM_CALL_FAILED = 15
    PHX_ERROR_FILE_OPEN_FAILED = 16
    PHX_ERROR_FILE_CLOSE_FAILED = 17
    PHX_ERROR_FILE_INVALID = 18
    PHX_ERROR_BAD_MEMBER = 19
    PHX_ERROR_HW_NOT_CONFIGURED = 20
    PHX_ERROR_INVALID_FLASH_PROPERTIES = 21
    PHX_ERROR_ACQUISITION_STARTED = 22
    PHX_ERROR_INVALID_POINTER = 23
    PHX_ERROR_LIB_INCOMPATIBLE = 24
    PHX_ERROR_SLAVE_MODE = 25

    # Phoenix display library errors
    PHX_ERROR_DISPLAY_CREATE_FAILED = 26
    PHX_ERROR_DISPLAY_DESTROY_FAILED = 27
    PHX_ERROR_DDRAW_INIT_FAILED = 28
    PHX_ERROR_DISPLAY_BUFF_CREATE_FAILED = 29
    PHX_ERROR_DISPLAY_BUFF_DESTROY_FAILED = 30
    PHX_ERROR_DDRAW_OPERATION_FAILED = 31

    # Registry errors
    PHX_ERROR_WIN32_REGISTRY_ERROR = 32

    PHX_ERROR_PROTOCOL_FAILURE = 33
    PHX_ERROR_CXP_INVALID_ADDRESS = 34
    PHX_ERROR_CXP_INVALID_DATA = 35
    PHX_ERROR_CXP_INVALID_CONTROL = 36
    PHX_ERROR_CXP_WRITE_TO_READ_ONLY = 37
    PHX_ERROR_CXP_READ_FROM_WRITE_ONLY = 38
    PHX_ERROR_CXP_SIZE_TOO_LARGE = 39
    PHX_ERROR_CXP_INCORRECT_SIZE = 40
    PHX_ERROR_CXP_MALFORMED_PACKET = 41
    PHX_ERROR_CXP_FAILED_CRC = 42
    PHX_ERROR_CXP_COMMAND_MISMATCH = 43
    PHX_ERROR_CXP_WAIT_ACK_DATA = 44
    PHX_ERROR_CXP_SIZE_MISMATCH = 45
    PHX_ERROR_CXP_STATUS_MISMATCH = 46
    PHX_ERROR_CXP_ACK_TIMEOUT = 47
    PHX_ERROR_CXP_WAIT_ACK_TIMEOUT = 48
    PHX_ERROR_CXP_USER_ACK_TIMEOUT = 49
    PHX_ERROR_CXP_INITIAL_ACK_TIMEOUT = 50
    PHX_ERROR_CXP_TENTATIVE_ACK_TIMEOUT = 51
    PHX_ERROR_CXP_RX_PACKET_INVALID = 52
    PHX_ERROR_CXP_RX_PACKET_CRC_ERROR = 53
    PHX_ERROR_CXP_INVALID_READ_ACK = 54
    PHX_ERROR_CXP_INVALID_WRITE_ACK = 55
    PHX_ERROR_CXP_INVALID_TENTATIVE_ACK = 56
    PHX_ERROR_CXP_INVALID_RESET_ACK = 57
    PHX_ERROR_CXP_INVALID_WAIT_ACK = 58

    PHX_WARNING_TIMEOUT = 0x8000
    PHX_WARNING_FLASH_RECONFIG = 0x8001
    PHX_WARNING_ZBT_RECONFIG = 0x8002
    PHX_WARNING_NOT_PHX_COM = 0x8003
    PHX_WARNING_NO_PHX_BOARD_REGISTERED = 0x8004
    PHX_WARNING_TIMEOUT_EXTENDED = 0x8005
    PHX_WARNING_FW_PARTIALLY_UPDATED = 0x8006
    PHX_WARNING_FW_STATUS_VERIFY = 0x8007
    PHX_WARNING_FW_DATA_VERIFY = 0x8008
    PHX_WARNING_FW_S1_S2_MISMATCH = 0x8009

    # Backwards compatibility
    PHX_ERROR_INVALID_FLASH_PROPERITES = PHX_ERROR_INVALID_FLASH_PROPERTIES
    PHX_WARNING = PHX_WARNING_TIMEOUT

# alias of other enum representation
etStat = eStat


PHX_C_CALL = ctypes.CFUNCTYPE(None, ctypes.c_char_p, ctypes.c_int, ctypes.c_char_p)

# Definition of the tColour structure
class tColour(Structure):
    _fields_ = [
        ("bRed", c_uint8),
        ("bGreen", c_uint8),
        ("bBlue", c_uint8)
    ]

# Definition of the tLutInfo structure
class tLutInfo(Structure):
    _fields_ = [
        ("dwLut", c_uint32),
        ("dwColour", c_uint32),
        ("dwTap", c_uint32),
        ("dwBrightness", c_uint32),
        ("dwContrast", c_uint32),
        ("dwGamma", c_uint32),
        ("dwFloor", c_uint32),
        ("dwCeiling", c_uint32),
        ("pwLutData", POINTER(c_uint16)),
        ("dwSize", c_uint32)
    ]

# Enum for Default settings for the Luts
class eLutCtrl:
    PHX_LUT_DEFAULT_BRIGHTNESS = 100
    PHX_LUT_DEFAULT_CONTRAST = 100
    PHX_LUT_DEFAULT_GAMMA = 100
    PHX_LUT_MAX_LUTS = 256  # Maximum number of LUTs across a line
    PHX_LUT_MAX_COLS = 3    # Maximum number of LUT components
    PHX_LUT_MAX_TAPS = 2    # Maximum number of camera taps

# Enum for Default settings for the FbdLuts
class eFbdLutCtrl:
    FBD_LUT_DEFAULT_BRIGHTNESS = 100
    FBD_LUT_DEFAULT_CONTRAST = 100
    FBD_LUT_BRIGHTNESS_MAX_VALUE = 200
    FBD_LUT_BRIGHTNESS_MIN_VALUE = 0
    FBD_LUT_CONTRAST_MAX_VALUE = 200
    FBD_LUT_CONTRAST_MIN_VALUE = 0
    FBD_LUT_DEFAULT_FLOOR = 0
    FBD_LUT_DEFAULT_CEILING = -1
    FBD_LUT_MAX_TAPS = 4
tBufferHandle = c_void_p

# Define the etFctErrorHandler callback function type
etFctErrorHandler = CFUNCTYPE(None, c_char_p, c_int, c_char_p)