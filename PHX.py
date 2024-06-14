from ctypes import c_int

# /* API & Library function parameter definitions
#  * ============================================
# _FnTypes()
# */
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
