import ctypes

#define the data structures
etParamValue = ctypes.c_int  # Assuming etParamValue is an enum represented as an int
tFlag = ctypes.c_int  # Assuming tFlag is represented as an int
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
