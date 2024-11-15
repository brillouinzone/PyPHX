/****************************************************************************
 *
 * ACTIVE SILICON LIMITED
 *
 * File name   : phx_api.h
 * Function    : User API for all Phoenix products
 * Project     : AP06
 * Authors     : Martin Bone, Richard Brown
 * Systems     : ANSI C
 * Version     : 08.78.02
 * Release Date: 1-Apr-23
 *
 * Copyright (c) 2000-2023 Active Silicon Ltd.
 ****************************************************************************
 * Comments:
 * --------
 * This file is the only include file a user (or higher level library) needs 
 * to include in their application in order to use the PHX library.
 * Define _PHX_NO_BACKWARDS_COMPATIBILITY_ prior to including this header to check
 * that code does not rely on deprecated features.
 *
 ****************************************************************************
 */
#ifndef _PHX_API_H_
#define _PHX_API_H_

/* Include all the OS dependent definitions */
/* Need to use #import if using native MacOS X with frameworks */
#if defined _ASL_MACOSX_FMWK
#import <ASLphoenix/phx_os.h>

#else
#include <phx_os.h>

#endif

/* API & Library function parameter definitions
 * ============================================
_FnTypes()
*/
typedef const enum {
   FNTYPE_EMASK         = (int) 0xF0000000,

   FNTYPE_PHA_API       = (int) 0x00000000,
   FNTYPE_PHA_LIB       = (int) 0x10000000,
   FNTYPE_PHC_API       = (int) 0x20000000,
   FNTYPE_PHC_LIB       = (int) 0x30000000,
   FNTYPE_PHD_API       = (int) 0x40000000,
   FNTYPE_PHD_LIB       = (int) 0x50000000,
   FNTYPE_PDL_API       = (int) 0x60000000,
   FNTYPE_PDL_LIB       = (int) 0x70000000,
   FNTYPE_PCC_API       = (int) 0x80000000,
   FNTYPE_PCC_LIB       = (int) 0x90000000,

   FNTYPE_PHX_API       = (int) 0xC0000000,
   FNTYPE_PHX_LIB       = (int) 0xD0000000
} etFnTypes;


/* PHX_Function Definitions
 * ========================
 * These enums are used as magic numbers for all parameters passed to the specific functions,
 * ie any parameter passed to the Acquisition functions will have the top 8 bits set to 3.
 * This is used to confirm that the parameter passed is valid for the function.
 */
typedef const enum {
   PHX_EMASK_FN          = (int) ((ui32) FNTYPE_EMASK   | (ui32) 0x000F0000),
   PHX_CAMERACONFIG_LOAD = (int) ((ui32) FNTYPE_PHX_API | (ui32) 0x00010000),
   PHX_SETANDGET         = (int) ((ui32) FNTYPE_PHX_API | (ui32) 0x00020000), /* For parameters after PHX_Open has been called */
   PHX_ACQUIRE           = (int) ((ui32) FNTYPE_PHX_API | (ui32) 0x00030000),
   PHX_ACTION            = (int) ((ui32) FNTYPE_PHX_API | (ui32) 0x00040000),
   PHX_TEST              = (int) ((ui32) FNTYPE_PHX_API | (ui32) 0x00050000),
   PHX_COM               = (int) ((ui32) FNTYPE_PHX_API | (ui32) 0x00060000),
   PHX_PRE_OPEN          = (int) ((ui32) FNTYPE_PHX_API | (ui32) 0x00070000), /* For parameters before PHX_Open has been called */
   PHX_CONTROL           = (int) ((ui32) FNTYPE_PHX_API | (ui32) 0x00080000),
   PHX_FBD_SETANDGET     = (int) ((ui32) FNTYPE_PHX_API | (ui32) 0x00090000), /* For FireBird specific parameters */
   PHX_CAN_SETANDGET     = (int) ((ui32) FNTYPE_PHX_API | (ui32) 0x000A0000), /* For CANBus specific parameters */
   PHX_PRIVATE_SETANDGET = (int) ((ui32) FNTYPE_PHX_API | (ui32) 0x000B0000), /* For Private specific parameters */

#if !defined _PHX_NO_BACKWARDS_COMPATIBILITY_
   /* Retained for backwards compatibility with previous software releases.
    * Do not use for new applications.
    */
   PHX_CAMERACONFIG_SAVE = PHX_ACTION
#endif
} etPhxFn ;


/* PHX_CameraConfigLoad Definitions
 * ================================
 */
typedef enum {
   PHX_EMASK_BOARD    = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x0007),
   PHX_BOARD_AUTO     = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x0000),   /* Auto detect the first available board */
   PHX_BOARD1         = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x0001),   /* 1st board in the scan order */
   PHX_BOARD2         = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x0002),   /* 2nd board in the scan order */
   PHX_BOARD3         = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x0003),   /* 3rd board in the scan order */
   PHX_BOARD4         = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x0004),   /* 4th board in the scan order */
   PHX_BOARD5         = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x0005),   /* 5th board in the scan order */
   PHX_BOARD6         = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x0006),   /* 6th board in the scan order */
   PHX_BOARD7         = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x0007),   /* 7th board in the scan order */
   PHX_BOARD_MAX      = (int) 7,                                                /* Maximum board number supported */

   PHX_EMASK_CHANNEL  = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x0030),
   PHX_CHANNEL_AUTO   = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x0000),   /* Auto detect the first available channel */
   PHX_CHANNEL_A      = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x0010),   /* 1st available channel */
   PHX_CHANNEL_B      = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x0020),   /* 2nd available channel */
   PHX_CHANNEL_MAX    = (int) 2,                                                /* Maximum channel number supported */
   PHX_CHANNEL_ONLY   = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x0040),   /* i.e. don't use other channel's resources   */

   PHX_EMASK_MODE     = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x0280),
   PHX_MODE_NORMAL    = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x0000),   /* i.e. both acquisition engine and com ports */
   PHX_COMMS_ONLY     = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x0080),   /* i.e. no control of acquisition engine      */
   PHX_ACQ_ONLY       = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x0200),   /* i.e. no control of com ports               */

   PHX_EMASK_UPDATE   = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x0100),
   PHX_UPDATE_FIRMWARE= (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x0100),   /* Reprogram all firmware (to flash) */
   PHX_NO_RECONFIGURE = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x0400),   /* Don't automatically reconfigure the board
                                                                                   if the firmware is already up to date */

   PHX_EMASK_TYPE     = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0xC000),
   PHX_EMASK_VARIANT  = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x3808),
   PHX_TYPE_AUTO      = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x0000),
   PHX_DIGITAL        = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x4000),   /* Any digital interface board  */

   PHX_D24CL_PCI32    = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x4800),   /* AS-PHX-D24CL-PCI32   variants */
   PHX_D24CL_PE1      = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x4808),   /* AS-PHX-D24CL-PE1     variants */
   PHX_D24AVDS_PE1    = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x5008),   /* AS-PHX-D24AVDS-PE1   variants */

   PHX_D32_PCI32      = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x5000),   /* AS-PHX-D32-PCI32     variants */

   PHX_D36_PCI32      = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x5800),   /* AS-PHX-D36-PCI32     variants */
   PHX_D36_PCI64      = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x6000),   /* AS-PHX-D36-PCI64     variants */
   PHX_D36_PCI64U     = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x6800),   /* AS-PHX-D36-PCI64U    variants */
   PHX_D36_PE1        = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x5808),   /* AS-PHX-D36-PE1       variants */

   PHX_D10HDSDI_PE1   = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x6808),   /* AS-PHX-D10HDSDI-PE1  variants */
   PHX_D20HDSDI_PE1   = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x6008),   /* AS-PHX-D20HDSDI-PE1  variants */

   PHX_D48CL_PCI32    = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x4008),   /* AS-PHX-D48CL-PCI32   variants */
   PHX_D48CL_PCI64    = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x7000),   /* AS-PHX-D48CL-PCI64   variants */
   PHX_D24CL_PE1_MIR  = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x7008),   /* AS-PHX-D24CL-PE1-MIR variants */
   PHX_D48CL_PCI64U   = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x7800),   /* AS-PHX-D48CL-PCI64U  variants */
   PHX_D48CL_PE1      = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x7808),   /* AS-PHX-D48CL-PE1     variants */

   PHX_ANALOGUE       = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0x8000),   /* Any analogue interface board */

   PHX_DIGITAL2       = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0xC000),   /* Any other digital interface board */
   PHX_D36_PE4        = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0xD808),   /* AS-PHX-D36-PE4       variants */
   PHX_D10HDSDI_PE4   = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0xE808),   /* AS-PHX-D10HDSDI-PE4  variants */
   PHX_D20HDSDI_PE4   = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0xE008),   /* AS-PHX-D20HDSDI-PE4  variants */
   PHX_D48CL_PE4      = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0xF008),   /* AS-PHX-D48CL-PE4     variants */
   PHX_D64CL_PE4      = (int) ((ui32) PHX_CAMERACONFIG_LOAD | (ui32) 0xD008),   /* AS-PHX-D64CL-PE4     variants */

#if !defined _PHX_NO_BACKWARDS_COMPATIBILITY_
   /* Retained for backwards compatibility with previous software releases.
    * Do not use for new applications.
    */
   PHX_CHANNEL1       = PHX_CHANNEL_A,
   PHX_CHANNEL2       = PHX_CHANNEL_B
#endif
} etCamConfigLoad ;


/* PHX_Action Definitions
 * ======================
 */
typedef const enum {
   PHX_EMASK_SAVE     = (int) ((ui32) PHX_ACTION | (ui32) 0x0007),
   PHX_SAVE_CAM       = (int) ((ui32) PHX_ACTION | (ui32) 0x0001),   /* Camera specific parameters      */
   PHX_SAVE_SYS       = (int) ((ui32) PHX_ACTION | (ui32) 0x0002),   /* system specific parameters      */
   PHX_SAVE_APP       = (int) ((ui32) PHX_ACTION | (ui32) 0x0004),   /* Application specific parameters */
   PHX_SAVE_ALL       = (int) ((ui32) PHX_ACTION | (ui32) 0x0007),   /* Save all parameters             */
} etActionParam ;

#if !defined _PHX_NO_BACKWARDS_COMPATIBILITY_
   /* Retained for backwards compatibility with previous software releases.
    * Do not use for new applications.
    */
typedef etActionParam etCamConfigSave;
#endif


/* Expansion Macros
 * ===============================
 */
#define _PHX_ENUM(_e,_x,_v)   _x = (int) ((ui32) _e | ((ui32) _v << 8 ))


/* GetAndSet Parameter Definitions
 * ===============================
 * These define the parameters that can be read or modified via the PHX_ParameterGet & PHX_ParameterSet
 * interface.  Each value is a unique number coded as follows:
 * bits 31 to 24 are the GetAndSet function identifier,
 * bits 23 to 16 are the specific parameter identifier
 * They are implemented as enums to make them visible within the debugger.
 */


/* Note - Because we have changed the API on a couple of occasions, we have tried to preserve the
 * numbering of the unaffected enums. Therefore there are a number of gaps in the following list where
 * some parameters have been removed.
 */
typedef enum {
   PHX_PARAM_MASK                = (int) 0xffffff00,
   PHX_INVALID_PARAM             = 0,
   PHX_CACHE_FLUSH               = 1,
   PHX_FORCE_REWRITE             = 2,

   _PHX_ENUM( PHX_SETANDGET, PHX_ACQ_CONTINUOUS,                     1 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_ACQ_NUM_BUFFERS,                    2 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_ACQ_SKIP,                           3 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_FGTRIG_SRC,                         4 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_FGTRIG_MODE,                        5 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_ACQ_FIELD_MODE,                     6 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_ACQ_XSUB,                           7 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_ACQ_YSUB,                           8 ),

   _PHX_ENUM( PHX_SETANDGET, PHX_CAM_ACTIVE_XLENGTH,                10 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_CAM_ACTIVE_YLENGTH,                11 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_CAM_ACTIVE_XOFFSET,                12 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_CAM_ACTIVE_YOFFSET,                13 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_CAM_CLOCK_POLARITY,                14 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_CAM_FORMAT,                        15 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_CAM_NUM_TAPS,                      16 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_CAM_SRC_DEPTH,                     17 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_CAM_SRC_COL,                       18 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_CAM_HTAP_DIR,                      19 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_CAM_HTAP_TYPE,                     20 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_CAM_HTAP_NUM,                      21 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_CAM_VTAP_DIR,                      22 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_CAM_VTAP_TYPE,                     23 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_CAM_VTAP_NUM,                      24 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_CAM_TYPE,                          25 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_CAM_XBINNING,                      26 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_CAM_YBINNING,                      27 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_COMMS_DATA,                        28 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_COMMS_FLOW,                        29 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_COMMS_INCOMING,                    30 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_COMMS_OUTGOING,                    31 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_COMMS_PARITY,                      32 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_COMMS_SPEED,                       33 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_COMMS_STANDARD,                    34 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_COMMS_STOP,                        35 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_DATASRC,                           36 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_DATASTREAM_VALID,                  37 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_BUS_FORMAT,                        38 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_DST_PTR_TYPE,                      39 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_DST_PTRS_VIRT,                     40 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_DUMMY_PARAM,                       41 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_ERROR_FIRST_ERRNUM,                42 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_ERROR_FIRST_ERRSTRING,             43 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_ERROR_HANDLER,                     44 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_ERROR_LAST_ERRNUM,                 45 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_ERROR_LAST_ERRSTRING,              46 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_COUNTE1_VALUE_NOW,                 47 ),

   _PHX_ENUM( PHX_SETANDGET, PHX_NUM_BOARDS,                        61 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_IO_CCIO_A,                         62 ),  /* Absolute */
   _PHX_ENUM( PHX_SETANDGET, PHX_IO_CCIO_A_OUT,                     63 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_IO_CCIO_B,                         64 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_IO_CCIO_B_OUT,                     65 ),

   _PHX_ENUM( PHX_SETANDGET, PHX_IO_OPTO_SET,                       68 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_IO_OPTO_CLR,                       69 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_IO_OPTO,                           70 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_IO_TTL_A,                          71 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_IO_TTL_A_OUT,                      72 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_IO_TTL_B,                          73 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_IO_TTL_B_OUT,                      74 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_TIMEOUT_CAPTURE,                   75 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_TIMEOUT_DMA,                       76 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_TIMEOUT_TRIGGER,                   77 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_INTRPT_SET,                        78 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_INTRPT_CLR,                        79 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_REV_HW,                            80 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_REV_HW_MAJOR,                      81 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_REV_HW_MINOR,                      82 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_REV_SW,                            83 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_REV_SW_MAJOR,                      84 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_REV_SW_MINOR,                      85 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_REV_SW_SUBMINOR,                   86 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_ROI_DST_XOFFSET,                   87 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_ROI_DST_YOFFSET,                   88 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_ROI_SRC_XOFFSET,                   89 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_ROI_SRC_YOFFSET,                   90 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_ROI_XLENGTH,                       91 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_ROI_YLENGTH,                       92 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_BUF_DST_XLENGTH,                   93 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_BUF_DST_YLENGTH,                   94 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_ACQ_STATUS,                        95 ),

   _PHX_ENUM( PHX_SETANDGET, PHX_BOARD_PROPERTIES,                 102 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_ROI_XLENGTH_SCALED,               103 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_ROI_YLENGTH_SCALED,               104 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_BUF_SET,                          105 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_BUF_SET_COLOUR,                   106 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_LUT_COUNT,                        107 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_LUT_INFO,                         108 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_REV_HW_SUBMINOR,                  109 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_LUT_CORRECT,                      110 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_LINETRIG_SRC,                     111 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_LINETRIG_TIMER_CTRL,              112 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_TIMERA1_PERIOD,                   113 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_CAMTRIG_SRC,                      114 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_EXP_CTRLIO_1,                     115 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_TIMERM1_WIDTH,                    116 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_EXP_CTRLIO_2,                     117 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_TIMERM2_WIDTH,                    118 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_EXP_LINESTART,                    119 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_FGTRIG_DELAY_MODE,                120 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_TIMERD1_VALUE,                    121 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_EVENTCOUNT_SRC,                   122 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_EVENTGATE_SRC,                    123 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_CAM_HTAP_ORDER,                   124 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_CAM_VTAP_ORDER,                   125 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_EVENT_CONTEXT,                    126 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_CAM_DATA_VALID,                   127 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_COUNT_BUFFER_READY,               128 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_COUNT_BUFFER_READY_NOW,           129 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_BIT_SHIFT,                        130 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_IO_CCIO,                          131 ),  /* Relative */
   _PHX_ENUM( PHX_SETANDGET, PHX_IO_CCIO_OUT,                      132 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_IO_TTL,                           133 ),  /* Relative */
   _PHX_ENUM( PHX_SETANDGET, PHX_IO_TTL_OUT,                       134 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_IO_OPTO_A,                        135 ),  /* Absolute */
   _PHX_ENUM( PHX_SETANDGET, PHX_IO_OPTO_B,                        136 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_IO_TIMER_A1_PERIOD,               137 ),  /* Absolute */
   _PHX_ENUM( PHX_SETANDGET, PHX_IO_TIMER_A2_PERIOD,               138 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_IO_TIMER_B1_PERIOD,               139 ),  /* Absolute */
   _PHX_ENUM( PHX_SETANDGET, PHX_IO_TIMER_B2_PERIOD,               140 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_IO_OPTO_OUT,                      141 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_IO_OPTO_A_OUT,                    142 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_IO_OPTO_B_OUT,                    143 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_FGTRIG_ALIGN,                     144 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_DST_ENDIAN,                       145 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_ACQ_CHAIN,                        146 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_ACQ_BLOCKING,                     147 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_DST_PTRS_PHYS32,                  148 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_BOARD_INFO,                       149 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_DATARATE_TEST,                    150 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_CAM_CLOCK_MAX,                    151 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_COUNTE1_VALUE_DB,                 152 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_CHAN_SYNC_MODE,                   153 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_ACQ_BUFFER_START,                 154 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_LUT_BYPASS,                       155 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_COMMS_PORT_NAME,                  156 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_CVB_PARAM,                        157 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_USR_FORMAT,                       158 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_ACQ_AUTO_RESTART,                 159 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_ACQ_HSCALE,                       160 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_MERGE_CHAN,                       161 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_MERGE_INTRPT_SET,                 162 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_MERGE_INTRPT_CLR,                 163 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_CLSER_INDEX,                      164 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_FIFO_BUFFER_COUNT,                165 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_REV_FW_FLASH,                     166 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_REV_FW_LIB,                       167 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_PCIE_INFO,                        168 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_TAP_MODE,                         169 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_ACQ_IMAGES_PER_BUFFER,            170 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_DST_PTRS_PHYS64,                  171 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_CAM_VTAP_INTERLEAVE_NUM,          172 ),
   _PHX_ENUM( PHX_SETANDGET, PHX_REV_DRV,                          173 ),


   PHX_IO_CCOUT_A                = PHX_IO_CCIO_A,
   PHX_IO_CCOUT_B                = PHX_IO_CCIO_B,
   PHX_IO_CCOUT                  = PHX_IO_CCIO,
   PHX_MASK_CCIO                 = (int) 0x00000003,
   PHX_MASK_CCOUT                = (int) 0x0000000f,
   PHX_MASK_OPTO                 = (int) 0x0000000f,


   _PHX_ENUM( PHX_PRE_OPEN, PHX_CONFIG_FILE,                         1 ),
   _PHX_ENUM( PHX_PRE_OPEN, PHX_BOARD_VARIANT,                       2 ),
   _PHX_ENUM( PHX_PRE_OPEN, PHX_BOARD_NUMBER,                        3 ),
   _PHX_ENUM( PHX_PRE_OPEN, PHX_CHANNEL_NUMBER,                      4 ),
   _PHX_ENUM( PHX_PRE_OPEN, PHX_CONFIG_MODE,                         5 ),
   _PHX_ENUM( PHX_PRE_OPEN, PHX_CONFIG_FLUSH,                        6 ),
   _PHX_ENUM( PHX_PRE_OPEN, PHX_BOARD_BUS_ADDRESS,                   7 ),


   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CXP_INFO,                       1 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CXP_BITRATE,                    2 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CXP_POCXP,                      3 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CXP_DISCOVERY,                  4 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CXP_LINKTEST_UPLINK_OK,         5 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CXP_LINKTEST_UPLINK_ERR,        6 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CXP_LINKTEST_DOWNLINK_OK,       7 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CXP_LINKTEST_DOWNLINK_ERR,      8 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CXP_ERRCOUNT_LINK_LOSS,         9 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CXP_ERRCOUNT_8B10B,            10 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CXP_ERRCOUNT_CRC_STREAM,       11 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CXP_ERRCOUNT_CRC_CTRL,         12 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CXP_ERRCOUNT_CRC_EVENT,        13 ),
   /* Note 14 is not used to maintain backward compatibility */

   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CAMERA_POWER,                  15 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CXP_BITRATE_MODE,              16 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CXP_POCXP_MODE,                17 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CXP_DISCOVERY_MODE,            18 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CXP_UPLINK_TEST,               19 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CXP_DOWNLINK_TEST,             20 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_422IN_CHX,                  21 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_422IN_CH1,                  22 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_422IN_CH2,                  23 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_422IN_CH3,                  24 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_422IN_CH4,                  25 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_OPTOIN_CHX,                 26 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_OPTOIN_CH1,                 27 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_OPTOIN_CH2,                 28 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_OPTOIN_CH3,                 29 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_OPTOIN_CH4,                 30 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_TTLIN_CHX,                  31 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_TTLIN_CH1,                  32 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_TTLIN_CH2,                  33 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_TTLIN_CH3,                  34 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_TTLIN_CH4,                  35 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_422OUT_CHX,                 36 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_422OUT_CH1,                 37 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_422OUT_CH2,                 38 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_422OUT_CH3,                 39 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_422OUT_CH4,                 40 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_OPTOOUT_CHX,                41 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_OPTOOUT_CH1,                42 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_OPTOOUT_CH2,                43 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_OPTOOUT_CH3,                44 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_OPTOOUT_CH4,                45 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_TTLOUT_CHX,                 46 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_TTLOUT_CH1,                 47 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_TTLOUT_CH2,                 48 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_TTLOUT_CH3,                 49 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_TTLOUT_CH4,                 50 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_CCOUT_CHX,                  51 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_CCOUT_CH1,                  52 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_CCOUT_CH2,                  53 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_CCOUT_CH3,                  54 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_CCOUT_CH4,                  55 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CAMTRIG_ENCODER_MODE,          56 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CAMTRIG_ENCODER_SRC,           57 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CAMTRIG_MULTIPLIER,            58 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CAMTRIG_DIVIDER,               59 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CAMTRIG_CXPTRIG_SRC,           60 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CAMTRIG_CXPTRIG_MODE,          61 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CAMTRIG_DELAY_MODE,            62 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_TIMERD2_VALUE,                 63 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_FW_NUM_DESIGNS,                64 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_FW_DESIGN_LIB,                 65 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_FW_DESIGN_PROG,                66 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_FW_DESIGN_FLASH,               67 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_STR_DESIGN_LIB,                68 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_STR_DESIGN_FLASH,              69 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_FGTRIG_FILTER_NS,              70 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CAMTRIG_FILTER_NS,             71 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CAMTRIG_ENCODER_FILTER_NS,     72 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_TIMERA1_PERIOD_NS,             73 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_TIMERD1FG_VALUE_NS,            74 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_TIMERM1_WIDTH_NS,              75 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_TIMERM2_WIDTH_NS,              76 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_TIMERD2_VALUE_NS,              77 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_FPGA_CORE_TEMP,                78 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CAMTRIG_DELAY_MODE_D1CAM,      79 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CAMTRIG_DELAY_MODE_D2CAM,      80 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_TIMERD1FG_COUNT,               81 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_TIMERA1_MODE,                  82 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_TIMERA1_PULSE_COUNT,           83 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_TIMERA1_SWTRIG,                84 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_TIMERD1CAM_VALUE_NS,           85 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_TIMERD2CAM_VALUE_NS,           86 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IMAGE_TIMESTAMP_MODE,          87 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_DST_ALIGNMENT,                 88 ),

#if 0
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CXP_POCXP_TRIP_RESET,          89 ),
#endif

   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CXP_HEADER,                    90 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IRIG_TRIG_COUNT,               91 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_24VOUT_CHX,                 92 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_24VOUT_CH1,                 93 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_24VOUT_CH2,                 94 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_24VOUT_CH3,                 95 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_24VOUT_CH4,                 96 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_24VIN_CHX,                  97 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_24VIN_CH1,                  98 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_24VIN_CH2,                  99 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_24VIN_CH3,                 100 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_24VIN_CH4,                 101 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_24VIN_CH5,                 102 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_24VIN_CH6,                 103 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_24VIN_CH7,                 104 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_IO_24VIN_CH8,                 105 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_SERIAL_NUMBER,                106 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_RETAIN_CXP_DISCOVERY,         107 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CL_LVAL_FILTER,               108 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CXP_ERRCOUNT_DUP_FIXED,       109 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CXP_ERRCOUNT_DUP_NFIXED,      110 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CXP_ERRCOUNT_FEC_FIXED,       111 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CXP_ERRCOUNT_FEC_NFIXED,      112 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_ACQ_BUFFER_MEMSET_MODE,       113 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_ACQ_BUFFER_MEMSET_VALUE,      114 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_CL_POCL_MODE,                 115 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_TIMERA1_TRIG_SRC,             116 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_TIMERA1_TRIG_FILTER_NS,       117 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_MEZZANINE_GUID,               118 ),
   _PHX_ENUM( PHX_FBD_SETANDGET, PHX_USER_ID,                      119 ),


   PHX_CL_INFO                   = PHX_CXP_INFO,


#if !defined _PHX_NO_BACKWARDS_COMPATIBILITY_
   /* Retained for backwards compatibility with previous software releases.
    * Do not use for new applications
    */
   PHX_ACQ_NUM_IMAGES            = PHX_ACQ_NUM_BUFFERS,        /*   2 << 8 */
   PHX_ACQTRIG_SRC               = PHX_FGTRIG_SRC,             /*   4 << 8 */
   PHX_ACQTRIG_TYPE              = PHX_FGTRIG_MODE,            /*   5 << 8 */
   PHX_ACQ_TYPE                  = PHX_ACQ_FIELD_MODE,         /*   6 << 8 */
   PHX_CAM_SRC_BANDS             = PHX_CAM_SRC_COL,            /*  18 << 8 */
   PHX_CAPTURE_FORMAT            = PHX_BUS_FORMAT,             /*  38 << 8 */
   PHX_DST_FORMAT                = PHX_BUS_FORMAT,
   PHX_DST_PTRS                  = PHX_DST_PTRS_VIRT,          /*  40 << 8 */
   PHX_EVENTCOUNT                = PHX_COUNTE1_VALUE_NOW,      /*  47 << 8 */
   PHX_EVENT_COUNT               = PHX_COUNTE1_VALUE_NOW,
   PHX_IO_LVDS_A                 = PHX_IO_CCIO_A,              /*  62 << 8 */
   PHX_IO_LVDS_A_OUT             = PHX_IO_CCIO_A_OUT,          /*  63 << 8 */
   PHX_IO_LVDS_B                 = PHX_IO_CCIO_B,              /*  64 << 8 */
   PHX_IO_LVDS_B_OUT             = PHX_IO_CCIO_B_OUT,          /*  65 << 8 */
   PHX_LINETRIG_TIMER_PERIOD     = PHX_TIMERA1_PERIOD,         /* 113 << 8 */
   PHX_EXPTRIG_SRC               = PHX_CAMTRIG_SRC,            /* 114 << 8 */
   PHX_EXP_SRC                   = PHX_CAMTRIG_SRC,
   PHX_IO_TIMER_1_PERIOD         = PHX_TIMERM1_WIDTH,          /* 116 << 8 */
   PHX_EXP_CTRLIO_1_DELAY        = PHX_TIMERM1_WIDTH,
   PHX_IO_TIMER_2_PERIOD         = PHX_TIMERM2_WIDTH,          /* 118 << 8 */
   PHX_EXP_CTRLIO_2_DELAY        = PHX_TIMERM2_WIDTH,
   PHX_ACQTRIG_DELAY_TYPE        = PHX_FGTRIG_DELAY_MODE,      /* 120 << 8 */
   PHX_ACQTRIG_DELAY             = PHX_TIMERD1_VALUE,          /* 121 << 8 */
   PHX_EVENTCOUNT_TYPE           = PHX_EVENTCOUNT_SRC,         /* 122 << 8 */
   PHX_EVENTGATE_TYPE            = PHX_EVENTGATE_SRC,          /* 123 << 8 */
   PHX_BUFFER_READY_COUNT        = PHX_COUNT_BUFFER_READY,     /* 128 << 8 */
   PHX_BUFFER_READY_COUNTER      = PHX_COUNT_BUFFER_READY_NOW, /* 129 << 8 */
   PHX_LUT_SHIFT                 = PHX_BIT_SHIFT,              /* 130 << 8 */
   PHX_IO_422_OUT                = PHX_IO_CCIO_OUT,            /* 132 << 8 */
   PHX_ACQTRIG_ALIGN             = PHX_FGTRIG_ALIGN,           /* 144 << 8 */
   PHX_EVENTCOUNT_AT_GATE        = PHX_COUNTE1_VALUE_DB,       /* 152 << 8 */
   PHX_USER_FORMAT               = PHX_USR_FORMAT,             /* 158 << 8 */
   PHX_REV_FLASH                 = PHX_REV_FW_FLASH,           /* 166 << 8 */
   PHX_TIMERD1_VALUE_NS          = PHX_TIMERD1FG_VALUE_NS,     /*  74 << 8 */
   PHX_STATUS                    = PHX_ACQ_STATUS,
   PHX_DST_PTRS_PHYS             = PHX_DST_PTRS_PHYS32
#endif
} etParam;


typedef enum {
   PHX_CONFIG_SAVE               = (int) ((ui32) PHX_ACTION | ((ui32)  1 << 8 )),
   PHX_FIRMWARE_PROG             = (int) ((ui32) PHX_ACTION | ((ui32)  2 << 8 ))
} etAction;


/* PHX_ControlRead/Write Definitions
 * =================================
 */
typedef enum {
   PHX_COMMS_PORT                = (int) ((ui32) PHX_CONTROL | (ui32) 0x0000),
   PHX_REGISTER_HOST             = (int) ((ui32) PHX_CONTROL | (ui32) 0x0001),
   PHX_REGISTER_DEVICE           = (int) ((ui32) PHX_CONTROL | (ui32) 0x0002)
} etControlPort;   


/* PHX_ComParameterGet Definitions
 * ===============================
 */
typedef enum {
   PHX_COM_NUM_PORTS             = (int) ((ui32) PHX_COM | (ui32) 0x0000),   /* return the number of Phoenix com ports available  */
   PHX_COM_CLSER_CONFIG          = (int) ((ui32) PHX_COM | (ui32) 0x0001),   /* LEGACY : convert clser index to config magic number        */
   PHX_COM_WIN_CONFIG            = (int) ((ui32) PHX_COM | (ui32) 0x0002),   /* LEGACY : convert windows comN index to config magic number */
   PHX_COM_GET_HANDLE            = (int) ((ui32) PHX_COM | (ui32) 0x0003),   /* Get Phoenix handle for specified Com Port Index */
   PHX_COM_GET_BOARD_CHAN_INDEX  = (int) ((ui32) PHX_COM | (ui32) 0x0004)    /* Get Board/Channel Indices */
} etComParam;   


/* GetAndSet Parameter Values   
 * ===========================
 * These define the values passed for a given parameter within the PHX_ParameterGet & PHX_ParameterSet
 * interface.  Each value consists of a unique incremental number.  However each value contains
 * the parameter value coded within the top 8 bits, which is used as an error check that the correct
 * value is being used for the parameter.
 * They are implemented as enums to make them visible within the debugger.
 */
typedef enum {
   PHX_INVALID_PARAMVALUE = 0,

   /* Boolean flags */
   PHX_ENABLE = 1,
   PHX_DISABLE,

   /* PHX_COMMS_DATA */
   PHX_COMMS_DATA_5 = (int) ((ui32) PHX_COMMS_DATA + 1),
   PHX_COMMS_DATA_6,
   PHX_COMMS_DATA_7,
   PHX_COMMS_DATA_8,

   /* PHX_COMMS_STOP */
   PHX_COMMS_STOP_1 = (int) ((ui32) PHX_COMMS_STOP + 1),
   PHX_COMMS_STOP_1_5,
   PHX_COMMS_STOP_2,

   /* PHX_COMMS_PARITY */
   PHX_COMMS_PARITY_NONE = (int) ((ui32) PHX_COMMS_PARITY + 1),
   PHX_COMMS_PARITY_EVEN,
   PHX_COMMS_PARITY_ODD,

   /* PHX_COMMS_FLOW */
   PHX_COMMS_FLOW_NONE = (int) ((ui32) PHX_COMMS_FLOW + 1),
   PHX_COMMS_FLOW_HW,
   PHX_COMMS_FLOW_SW,

   /* PHX_COMMS_STANDARD */
   PHX_COMMS_STANDARD_RS232 = (int) ((ui32) PHX_COMMS_STANDARD + 1),
   PHX_COMMS_STANDARD_LVDS,

   /* PHX_IO_OPTO_OUT_SET/CLR */
   PHX_IO_OPTO_OUT1                 = 0x00000001,
   PHX_IO_OPTO_OUT2                 = 0x00000002,
   PHX_IO_OPTO_OUT3                 = 0x00000004,
   PHX_IO_OPTO_OUT4                 = 0x00000008,

   /* PHX_IO_OPTO_SET/CLR */
   PHX_IO_OPTO1                     = 0x00000001,
   PHX_IO_OPTO2                     = 0x00000002,
   PHX_IO_OPTO3                     = 0x00000004,
   PHX_IO_OPTO4                     = 0x00000008,

   /* PHX_ACQ_STATUS */
   PHX_ACQ_STATUS_IDLE = (int) ((ui32) PHX_ACQ_STATUS + 1),
   PHX_ACQ_STATUS_IN_PROGRESS,
   PHX_ACQ_STATUS_WAITING_FOR_TRIGGER,

   /* PHX_CAM_TYPE */
   PHX_CAM_LINESCAN_ROI = (int) ((ui32) PHX_CAM_TYPE + 1),
   PHX_CAM_LINESCAN_NO_ROI,
   PHX_CAM_AREASCAN_ROI,
   PHX_CAM_AREASCAN_NO_ROI,
   PHX_CAM_MULTILINESCAN_ROI,

   /* PHX_CAM_FORMAT */
   PHX_CAM_INTERLACED = (int) ((ui32) PHX_CAM_FORMAT + 1),
   PHX_CAM_NON_INTERLACED,

   /* PHX_CAM_SRC_COL */
   PHX_CAM_SRC_MONO                 = 0x00000001,
   PHX_CAM_SRC_RGB                  = 0x00000003,
   PHX_CAM_SRC_BAYER_RG = (int) ((ui32) PHX_CAM_SRC_COL + 1),
   PHX_CAM_SRC_BAYER_GR,
   PHX_CAM_SRC_BAYER_GB,
   PHX_CAM_SRC_BAYER_BG,
   PHX_CAM_SRC_YUV422,
   PHX_CAM_SRC_RGBA,
   PHX_CAM_SRC_RGBM,
 
   /* PHX_CAM_HTAP_DIR */
   PHX_CAM_HTAP_LEFT = (int) ((ui32) PHX_CAM_HTAP_DIR + 1),
   PHX_CAM_HTAP_RIGHT,
   PHX_CAM_HTAP_CONVERGE,
   PHX_CAM_HTAP_DIVERGE,

   /* PHX_CAM_HTAP_TYPE */
   PHX_CAM_HTAP_LINEAR = (int) ((ui32) PHX_CAM_HTAP_TYPE + 1),
   PHX_CAM_HTAP_OFFSET_1,
   PHX_CAM_HTAP_ALTERNATE,
   PHX_CAM_HTAP_OFFSET_2,
   PHX_CAM_HTAP_SPAN,

   /* PHX_CAM_HTAP_ORDER */
   PHX_CAM_HTAP_ASCENDING = (int) ((ui32) PHX_CAM_HTAP_ORDER + 1),
   PHX_CAM_HTAP_DESCENDING,

   /* PHX_CAM_VTAP_DIR */
   PHX_CAM_VTAP_TOP = (int) ((ui32) PHX_CAM_VTAP_DIR + 1),
   PHX_CAM_VTAP_BOTTOM,
   PHX_CAM_VTAP_CONVERGE,
   PHX_CAM_VTAP_DIVERGE,

   /* PHX_CAM_VTAP_TYPE */
   PHX_CAM_VTAP_LINEAR = (int) ((ui32) PHX_CAM_VTAP_TYPE + 1),
   PHX_CAM_VTAP_OFFSET,
   PHX_CAM_VTAP_ALTERNATE,

   /* PHX_CAM_VTAP_ORDER */
   PHX_CAM_VTAP_ASCENDING = (int) ((ui32) PHX_CAM_VTAP_ORDER + 1),
   PHX_CAM_VTAP_DESCENDING,

   /* PHX_CAM_CLOCK_POLARITY */
   PHX_CAM_CLOCK_POS = (int) ((ui32) PHX_CAM_CLOCK_POLARITY + 1),
   PHX_CAM_CLOCK_NEG,

   /* PHX_CAM_CLOCK_MAX */
   PHX_CAM_CLOCK_MAX_DEFAULT = (int) ((ui32) PHX_CAM_CLOCK_MAX + 1),
   PHX_CAM_CLOCK_MAX_85MHZ,

   /* PHX_ACQ_TYPE */
   PHX_ACQ_FRAME_12 = (int) ((ui32) PHX_ACQ_FIELD_MODE + 1),
   PHX_ACQ_FRAME_21,
   PHX_ACQ_FIELD_12,
   PHX_ACQ_FIELD_21,
   PHX_ACQ_FIELD_1,
   PHX_ACQ_FIELD_2,
   PHX_ACQ_FIELD_NEXT,
   PHX_ACQ_LINE_DOUBLE_12,
   PHX_ACQ_LINE_DOUBLE_21,
   PHX_ACQ_LINE_DOUBLE_NEXT,
   PHX_ACQ_LINE_DOUBLE_1,
   PHX_ACQ_LINE_DOUBLE_2,

   /* PHX_ACQ_XSUB */
   PHX_ACQ_X1 = (int) ((ui32) PHX_ACQ_XSUB + 1),
   PHX_ACQ_X2,
   PHX_ACQ_X4,
   PHX_ACQ_X8,

   /* PHX_DST_PTR_TYPE */
   PHX_DST_PTR_INTERNAL = (int) ((ui32) PHX_DST_PTR_TYPE + 1),
   PHX_DST_PTR_USER_VIRT,
   PHX_DST_PTR_USER_PHYS,
   PHX_DST_PTR_USER_CUDA,

   /* PHX_DATASTREAM_VALID */
   PHX_DATASTREAM_ALWAYS = (int) ((ui32) PHX_DATASTREAM_VALID + 1),
   PHX_DATASTREAM_LINE_ONLY,
   PHX_DATASTREAM_FRAME_ONLY,
   PHX_DATASTREAM_FRAME_AND_LINE,

   /* PHX_DATASRC */
   PHX_DATASRC_CAMERA = (int) ((ui32) PHX_DATASRC + 1),
   PHX_DATASRC_SIMULATOR_STATIC,
   PHX_DATASRC_SIMULATOR_ROLL,
   PHX_DATASRC_SIMULATOR_PRBS,

   /* PHX_BUS_FORMAT */
   PHX_BUS_FORMAT_MONO8  = (int) ((ui32) PHX_BUS_FORMAT + 1),
   PHX_BUS_FORMAT_MONO16,
   PHX_BUS_FORMAT_MONO32,
   PHX_BUS_FORMAT_MONO36,     /* No longer supported in version 2 release */
   PHX_BUS_FORMAT_BGR5,
   PHX_BUS_FORMAT_BGR565,
   PHX_BUS_FORMAT_XBGR8,         /* PHX_DST_FORMAT_RGBX32, */
   PHX_BUS_FORMAT_BGRX8,         /* PHX_DST_FORMAT_XRGB32, */
   PHX_BUS_FORMAT_BGR16,
   PHX_BUS_FORMAT_RGB5,
   PHX_BUS_FORMAT_RGB565,
   PHX_BUS_FORMAT_XRGB8,         /* PHX_DST_FORMAT_BGRX32, */
   PHX_BUS_FORMAT_RGBX8,         /* PHX_DST_FORMAT_XBGR32, */
   PHX_BUS_FORMAT_RGB16,
   PHX_BUS_FORMAT_BGR101210,   /* Added in version 2 release */
   PHX_BUS_FORMAT_RGB101210,
   PHX_BUS_FORMAT_BGR8,        /* Added in version 2.25 release */
   PHX_BUS_FORMAT_RGB8,
   PHX_BUS_FORMAT_MONO10,
   PHX_BUS_FORMAT_MONO12,
   PHX_BUS_FORMAT_MONO14,
   PHX_DST_FORMAT_BAY8,
   PHX_DST_FORMAT_BAY10,
   PHX_DST_FORMAT_BAY12,
   PHX_DST_FORMAT_BAY14,
   PHX_DST_FORMAT_BAY16,
   PHX_DST_FORMAT_2Y12,
   PHX_BUS_FORMAT_BGR12,
   PHX_BUS_FORMAT_RGB12,
   PHX_BUS_FORMAT_YUV422_8,
   PHX_DST_FORMAT_Y12B,
   PHX_BUS_FORMAT_RGB8_PLANAR,
   PHX_BUS_FORMAT_MONO10P,
   PHX_BUS_FORMAT_MONO14P,
   PHX_BUS_FORMAT_RGBA8,
   PHX_BUS_FORMAT_RGBA10,
   PHX_BUS_FORMAT_RGBA12,
   PHX_BUS_FORMAT_RGBA14,
   PHX_BUS_FORMAT_RGBA16,
   PHX_BUS_FORMAT_BAYER_GR8,
   PHX_BUS_FORMAT_BAYER_RG8,
   PHX_BUS_FORMAT_BAYER_GB8,
   PHX_BUS_FORMAT_BAYER_BG8,
   PHX_BUS_FORMAT_BAYER_GR10,
   PHX_BUS_FORMAT_BAYER_RG10,
   PHX_BUS_FORMAT_BAYER_GB10,
   PHX_BUS_FORMAT_BAYER_BG10,
   PHX_BUS_FORMAT_BAYER_GR12,
   PHX_BUS_FORMAT_BAYER_RG12,
   PHX_BUS_FORMAT_BAYER_GB12,
   PHX_BUS_FORMAT_BAYER_BG12,
   PHX_BUS_FORMAT_BAYER_GR14,
   PHX_BUS_FORMAT_BAYER_RG14,
   PHX_BUS_FORMAT_BAYER_GB14,
   PHX_BUS_FORMAT_BAYER_BG14,
   PHX_BUS_FORMAT_BAYER_GR16,
   PHX_BUS_FORMAT_BAYER_RG16,
   PHX_BUS_FORMAT_BAYER_GB16,
   PHX_BUS_FORMAT_BAYER_BG16,
   PHX_BUS_FORMAT_BGR10,
   PHX_BUS_FORMAT_RGB10,
   PHX_BUS_FORMAT_BGR14,
   PHX_BUS_FORMAT_RGB14,
   PHX_BUS_FORMAT_MONO12P,
   PHX_BUS_FORMAT_BAYER_GR10P,
   PHX_BUS_FORMAT_BAYER_RG10P,
   PHX_BUS_FORMAT_BAYER_GB10P,
   PHX_BUS_FORMAT_BAYER_BG10P,
   PHX_BUS_FORMAT_BAYER_GR12P,
   PHX_BUS_FORMAT_BAYER_RG12P,
   PHX_BUS_FORMAT_BAYER_GB12P,
   PHX_BUS_FORMAT_BAYER_BG12P,
   PHX_BUS_FORMAT_BAYER_GR14P,
   PHX_BUS_FORMAT_BAYER_RG14P,
   PHX_BUS_FORMAT_BAYER_GB14P,
   PHX_BUS_FORMAT_BAYER_BG14P,

   /* PHX_USR_FORMAT */
   PHX_USR_FORMAT_MONO8  = (int) ((ui32) PHX_USR_FORMAT + 1),
   PHX_USR_FORMAT_MONO16,
   PHX_USR_FORMAT_MONO32,
   PHX_USR_FORMAT_MONO36,     /* No longer supported in version 2 release */
   PHX_USR_FORMAT_BGR5,
   PHX_USR_FORMAT_BGR565,
   PHX_USR_FORMAT_XBGR8,        /* PHX_USER_FORMAT_RGBX32, */
   PHX_USR_FORMAT_BGRX8,        /* PHX_USER_FORMAT_XRGB32, */
   PHX_USR_FORMAT_BGR16,
   PHX_USR_FORMAT_RGB5,
   PHX_USR_FORMAT_RGB565,
   PHX_USR_FORMAT_XRGB8,        /* PHX_USER_FORMAT_BGRX32, */
   PHX_USR_FORMAT_RGBX8,        /* PHX_USER_FORMAT_XBGR32, */
   PHX_USR_FORMAT_RGB16,
   PHX_USR_FORMAT_BGR101210,  /* Added in version 2 release */
   PHX_USR_FORMAT_RGB101210,
   PHX_USR_FORMAT_BGR8,       /* Added in version 2.25 release */
   PHX_USR_FORMAT_RGB8,
   PHX_USR_FORMAT_MONO10,
   PHX_USR_FORMAT_MONO12,
   PHX_USR_FORMAT_MONO14,
   PHX_USER_FORMAT_BAY8,
   PHX_USER_FORMAT_BAY10,
   PHX_USER_FORMAT_BAY12,
   PHX_USER_FORMAT_BAY14,
   PHX_USER_FORMAT_BAY16,
   PHX_USER_FORMAT_2Y12,
   PHX_USR_FORMAT_BGR12,
   PHX_USR_FORMAT_RGB12,
   PHX_USR_FORMAT_YUV422_8,
   PHX_USER_FORMAT_Y12B,
   PHX_USR_FORMAT_RGB8_PLANAR,
   PHX_USR_FORMAT_MONO10P,
   PHX_USR_FORMAT_MONO14P,
   PHX_USR_FORMAT_RGBA8,
   PHX_USR_FORMAT_RGBA10,
   PHX_USR_FORMAT_RGBA12,
   PHX_USR_FORMAT_RGBA14,
   PHX_USR_FORMAT_RGBA16,
   PHX_USR_FORMAT_BAYER_GR8,
   PHX_USR_FORMAT_BAYER_RG8,
   PHX_USR_FORMAT_BAYER_GB8,
   PHX_USR_FORMAT_BAYER_BG8,
   PHX_USR_FORMAT_BAYER_GR10,
   PHX_USR_FORMAT_BAYER_RG10,
   PHX_USR_FORMAT_BAYER_GB10,
   PHX_USR_FORMAT_BAYER_BG10,
   PHX_USR_FORMAT_BAYER_GR12,
   PHX_USR_FORMAT_BAYER_RG12,
   PHX_USR_FORMAT_BAYER_GB12,
   PHX_USR_FORMAT_BAYER_BG12,
   PHX_USR_FORMAT_BAYER_GR14,
   PHX_USR_FORMAT_BAYER_RG14,
   PHX_USR_FORMAT_BAYER_GB14,
   PHX_USR_FORMAT_BAYER_BG14,
   PHX_USR_FORMAT_BAYER_GR16,
   PHX_USR_FORMAT_BAYER_RG16,
   PHX_USR_FORMAT_BAYER_GB16,
   PHX_USR_FORMAT_BAYER_BG16,
   PHX_USR_FORMAT_BGR10,
   PHX_USR_FORMAT_RGB10,
   PHX_USR_FORMAT_BGR14,
   PHX_USR_FORMAT_RGB14,
   PHX_USR_FORMAT_MONO12P,
   PHX_USR_FORMAT_BAYER_GR10P,
   PHX_USR_FORMAT_BAYER_RG10P,
   PHX_USR_FORMAT_BAYER_GB10P,
   PHX_USR_FORMAT_BAYER_BG10P,
   PHX_USR_FORMAT_BAYER_GR12P,
   PHX_USR_FORMAT_BAYER_RG12P,
   PHX_USR_FORMAT_BAYER_GB12P,
   PHX_USR_FORMAT_BAYER_BG12P,
   PHX_USR_FORMAT_BAYER_GR14P,
   PHX_USR_FORMAT_BAYER_RG14P,
   PHX_USR_FORMAT_BAYER_GB14P,
   PHX_USR_FORMAT_BAYER_BG14P,
#if 0
   PHX_USR_FORMAT_RGB10P,
   PHX_USR_FORMAT_RGB12P,
   PHX_USR_FORMAT_RGB14P,
#endif

   /* PHX_LINETRIG_SRC */
   PHX_LINETRIG_NONE  = (int) ((ui32) PHX_LINETRIG_SRC + 1),
   PHX_LINETRIG_AUXIN_1_RISING,        /* Relative */
   PHX_LINETRIG_AUXIN_1_FALLING,
   PHX_LINETRIG_CTRLIN_2_RISING,       /* No longer supported; retained for enum backward compatibility */
   PHX_LINETRIG_CTRLIN_2_FALLING,      /* No longer supported; retained for enum backward compatibility */
   PHX_LINETRIG_AUXIN_2_RISING,
   PHX_LINETRIG_AUXIN_2_FALLING,
   PHX_LINETRIG_TIMER,
   PHX_LINETRIG_AUXIN_A1_RISING,       /* Absolute */
   PHX_LINETRIG_AUXIN_A1_FALLING,
   PHX_LINETRIG_AUXIN_A2_RISING,
   PHX_LINETRIG_AUXIN_A2_FALLING,
   PHX_LINETRIG_AUXIN_B1_RISING,
   PHX_LINETRIG_AUXIN_B1_FALLING,
   PHX_LINETRIG_AUXIN_B2_RISING,
   PHX_LINETRIG_AUXIN_B2_FALLING,

   /* PHX_LINETRIG_TIMER_CTRL */
   PHX_LINETRIG_TIMER_TIME = (int) ((ui32) PHX_LINETRIG_TIMER_CTRL + 1),
   PHX_LINETRIG_TIMER_DISABLE,
   PHX_LINETRIG_TIMER_LINES,

   /* PHX_CAMTRIG_SRC */
   PHX_CAMTRIG_SRC_LINETRIG = (int) ((ui32) PHX_CAMTRIG_SRC + 1),
   PHX_CAMTRIG_SRC_FGTRIGA_CHX,
   PHX_CAMTRIG_SRC_NONE,
   PHX_CAMTRIG_SRC_SWTRIG_CHX,
   PHX_CAMTRIG_SRC_AUXIN_1_RISE,        /* Relative */
   PHX_CAMTRIG_SRC_AUXIN_1_FALL,
   PHX_CAMTRIG_SRC_AUXIN_2_RISE,
   PHX_CAMTRIG_SRC_AUXIN_2_FALL,
   PHX_CAMTRIG_SRC_TIMERA1_CHX,
   PHX_CAMTRIG_SRC_AUXIN_A1_RISE,       /* Absolute */
   PHX_CAMTRIG_SRC_AUXIN_A1_FALL,
   PHX_CAMTRIG_SRC_AUXIN_A2_RISE,
   PHX_CAMTRIG_SRC_AUXIN_A2_FALL,
   PHX_CAMTRIG_SRC_AUXIN_B1_RISE,
   PHX_CAMTRIG_SRC_AUXIN_B1_FALL,
   PHX_CAMTRIG_SRC_AUXIN_B2_RISE,
   PHX_CAMTRIG_SRC_AUXIN_B2_FALL,
   PHX_CAMTRIG_SRC_FGTRIGA_CH1,
   PHX_CAMTRIG_SRC_FGTRIGA_CH2,
   PHX_CAMTRIG_SRC_FGTRIGA_CH3,
   PHX_CAMTRIG_SRC_FGTRIGA_CH4,
   PHX_CAMTRIG_SRC_SWTRIG_CH1,
   PHX_CAMTRIG_SRC_SWTRIG_CH2,
   PHX_CAMTRIG_SRC_SWTRIG_CH3,
   PHX_CAMTRIG_SRC_SWTRIG_CH4,
   PHX_CAMTRIG_SRC_TIMERA1_CH1,
   PHX_CAMTRIG_SRC_TIMERA1_CH2,
   PHX_CAMTRIG_SRC_TIMERA1_CH3,
   PHX_CAMTRIG_SRC_TIMERA1_CH4,
   PHX_CAMTRIG_SRC_422IN_CHX_0_RISE,
   PHX_CAMTRIG_SRC_422IN_CH1_0_RISE,
   PHX_CAMTRIG_SRC_422IN_CH2_0_RISE,
   PHX_CAMTRIG_SRC_422IN_CH3_0_RISE,
   PHX_CAMTRIG_SRC_422IN_CH4_0_RISE,
   PHX_CAMTRIG_SRC_422IN_CHX_0_FALL,
   PHX_CAMTRIG_SRC_422IN_CH1_0_FALL,
   PHX_CAMTRIG_SRC_422IN_CH2_0_FALL,
   PHX_CAMTRIG_SRC_422IN_CH3_0_FALL,
   PHX_CAMTRIG_SRC_422IN_CH4_0_FALL,
   PHX_CAMTRIG_SRC_OPTOIN_CHX_0_RISE,
   PHX_CAMTRIG_SRC_OPTOIN_CH1_0_RISE,
   PHX_CAMTRIG_SRC_OPTOIN_CH2_0_RISE,
   PHX_CAMTRIG_SRC_OPTOIN_CH3_0_RISE,
   PHX_CAMTRIG_SRC_OPTOIN_CH4_0_RISE,
   PHX_CAMTRIG_SRC_OPTOIN_CHX_0_FALL,
   PHX_CAMTRIG_SRC_OPTOIN_CH1_0_FALL,
   PHX_CAMTRIG_SRC_OPTOIN_CH2_0_FALL,
   PHX_CAMTRIG_SRC_OPTOIN_CH3_0_FALL,
   PHX_CAMTRIG_SRC_OPTOIN_CH4_0_FALL,
   PHX_CAMTRIG_SRC_TTLIN_CHX_0_RISE,
   PHX_CAMTRIG_SRC_TTLIN_CH1_0_RISE,
   PHX_CAMTRIG_SRC_TTLIN_CH2_0_RISE,
   PHX_CAMTRIG_SRC_TTLIN_CH3_0_RISE,
   PHX_CAMTRIG_SRC_TTLIN_CH4_0_RISE,
   PHX_CAMTRIG_SRC_TTLIN_CHX_0_FALL,
   PHX_CAMTRIG_SRC_TTLIN_CH1_0_FALL,
   PHX_CAMTRIG_SRC_TTLIN_CH2_0_FALL,
   PHX_CAMTRIG_SRC_TTLIN_CH3_0_FALL,
   PHX_CAMTRIG_SRC_TTLIN_CH4_0_FALL,
   PHX_CAMTRIG_SRC_ENCODER_CHX,
   PHX_CAMTRIG_SRC_ENCODER_CH1,
   PHX_CAMTRIG_SRC_ENCODER_CH2,
   PHX_CAMTRIG_SRC_ENCODER_CH3,
   PHX_CAMTRIG_SRC_ENCODER_CH4,
   PHX_CAMTRIG_SRC_24VIN_CHX_0_RISE,
   PHX_CAMTRIG_SRC_24VIN_CHX_0_FALL,
   PHX_CAMTRIG_SRC_24VIN_CH1_0_RISE,
   PHX_CAMTRIG_SRC_24VIN_CH1_0_FALL,
   PHX_CAMTRIG_SRC_24VIN_CH2_0_RISE,
   PHX_CAMTRIG_SRC_24VIN_CH2_0_FALL,
   PHX_CAMTRIG_SRC_24VIN_CH3_0_RISE,
   PHX_CAMTRIG_SRC_24VIN_CH3_0_FALL,
   PHX_CAMTRIG_SRC_24VIN_CH4_0_RISE,
   PHX_CAMTRIG_SRC_24VIN_CH4_0_FALL,
   PHX_CAMTRIG_SRC_24VIN_CH5_0_RISE,
   PHX_CAMTRIG_SRC_24VIN_CH5_0_FALL,
   PHX_CAMTRIG_SRC_24VIN_CH6_0_RISE,
   PHX_CAMTRIG_SRC_24VIN_CH6_0_FALL,
   PHX_CAMTRIG_SRC_24VIN_CH7_0_RISE,
   PHX_CAMTRIG_SRC_24VIN_CH7_0_FALL,
   PHX_CAMTRIG_SRC_24VIN_CH8_0_RISE,
   PHX_CAMTRIG_SRC_24VIN_CH8_0_FALL,

   /* PHX_CAMTRIG_ENCODER_MODE */
   PHX_CAMTRIG_ENCODER_MODE1 = (int) ((ui32) PHX_CAMTRIG_ENCODER_MODE + 1),
   PHX_CAMTRIG_ENCODER_MODE2,
   PHX_CAMTRIG_ENCODER_MODE3,
   PHX_CAMTRIG_ENCODER_MODE4,
   PHX_CAMTRIG_ENCODER_MODE5,

   /* PHX_CAMTRIG_ENCODER_SRC */
   PHX_CAMTRIG_SRC_422IN_CHX_0 = (int) ((ui32) PHX_CAMTRIG_ENCODER_SRC + 1),
   PHX_CAMTRIG_SRC_422IN_CH1_0,
   PHX_CAMTRIG_SRC_422IN_CH2_0,
   PHX_CAMTRIG_SRC_422IN_CH3_0,
   PHX_CAMTRIG_SRC_422IN_CH4_0,
   PHX_CAMTRIG_SRC_OPTOIN_CHX_0,
   PHX_CAMTRIG_SRC_OPTOIN_CH1_0,
   PHX_CAMTRIG_SRC_OPTOIN_CH2_0,
   PHX_CAMTRIG_SRC_OPTOIN_CH3_0,
   PHX_CAMTRIG_SRC_OPTOIN_CH4_0,
   PHX_CAMTRIG_SRC_TTLIN_CHX_0,
   PHX_CAMTRIG_SRC_TTLIN_CH1_0,
   PHX_CAMTRIG_SRC_TTLIN_CH2_0,
   PHX_CAMTRIG_SRC_TTLIN_CH3_0,
   PHX_CAMTRIG_SRC_TTLIN_CH4_0,

   /* PHX_CAMTRIG_DELAY_MODE */
   PHX_CAMTRIG_DELAY_NONE  = (int) ((ui32) PHX_CAMTRIG_DELAY_MODE + 1),
   PHX_CAMTRIG_DELAY_TIME,
   PHX_CAMTRIG_DELAY_LINE,

   /* PHX_CAMTRIG_CXPTRIG_SRC */
   PHX_CAMTRIG_CXPTRIG_NONE = (int) ((ui32) PHX_CAMTRIG_CXPTRIG_SRC + 1),
   PHX_CAMTRIG_CXPTRIG_TIMERM1_CHX,
   PHX_CAMTRIG_CXPTRIG_TIMERM1_CH1,
   PHX_CAMTRIG_CXPTRIG_TIMERM1_CH2,
   PHX_CAMTRIG_CXPTRIG_TIMERM1_CH3,
   PHX_CAMTRIG_CXPTRIG_TIMERM1_CH4,
   PHX_CAMTRIG_CXPTRIG_TIMERM2_CHX,
   PHX_CAMTRIG_CXPTRIG_TIMERM2_CH1,
   PHX_CAMTRIG_CXPTRIG_TIMERM2_CH2,
   PHX_CAMTRIG_CXPTRIG_TIMERM2_CH3,
   PHX_CAMTRIG_CXPTRIG_TIMERM2_CH4,
   PHX_CAMTRIG_CXPTRIG_SW_RISE,
   PHX_CAMTRIG_CXPTRIG_SW_FALL,
   PHX_CAMTRIG_CXPTRIG_FGTRIGD_CHX,
   PHX_CAMTRIG_CXPTRIG_FGTRIGD_CH1,
   PHX_CAMTRIG_CXPTRIG_FGTRIGD_CH2,
   PHX_CAMTRIG_CXPTRIG_FGTRIGD_CH3,
   PHX_CAMTRIG_CXPTRIG_FGTRIGD_CH4,

   /* PHX_CAMTRIG_CXPTRIG_MODE */
   PHX_CAMTRIG_CXPTRIG_RISEFALL = (int) ((ui32) PHX_CAMTRIG_CXPTRIG_MODE + 1),
   PHX_CAMTRIG_CXPTRIG_RISEFALL_INV,
   PHX_CAMTRIG_CXPTRIG_RISE,
   PHX_CAMTRIG_CXPTRIG_RISE_INV,
   PHX_CAMTRIG_CXPTRIG_FALL,
   PHX_CAMTRIG_CXPTRIG_FALL_INV,

   /* PHX_EXP_CTRLIO_1 */
   PHX_EXP_CTRLIO_1_HW_POS = (int) ((ui32) PHX_EXP_CTRLIO_1 + 1),
   PHX_EXP_CTRLIO_1_HW_NEG,
   PHX_EXP_CTRLIO_1_SW_POS,
   PHX_EXP_CTRLIO_1_SW_NEG,

   /* PHX_EXP_CTRLIO_2 */
   PHX_EXP_CTRLIO_2_HW_POS = (int) ((ui32) PHX_EXP_CTRLIO_2 + 1),
   PHX_EXP_CTRLIO_2_HW_NEG,
   PHX_EXP_CTRLIO_2_SW_POS,
   PHX_EXP_CTRLIO_2_SW_NEG,

   /* PHX_EXP_LINESTART */
   PHX_EXP_LINESTART_LINE = (int) ((ui32) PHX_EXP_LINESTART + 1),
   PHX_EXP_LINESTART_CCIO_2,     /* Relative */
   PHX_EXP_LINESTART_CCIO_A2,    /* Absolute */
   PHX_EXP_LINESTART_CCIO_B2,

   /* PHX_FGTRIG_SRC */
   PHX_FGTRIG_SRC_OPTO_A1 = (int) ((ui32) PHX_FGTRIG_SRC + 1), /* Absolute */
   PHX_FGTRIG_SRC_OPTO_A2,
   PHX_FGTRIG_SRC_OPTO_B1,
   PHX_FGTRIG_SRC_OPTO_B2,
   PHX_FGTRIG_SRC_CTRLIN_A1,
   PHX_FGTRIG_SRC_CTRLIN_A2,
   PHX_FGTRIG_SRC_CTRLIN_A3,
   PHX_FGTRIG_SRC_CTRLIN_B1,
   PHX_FGTRIG_SRC_CTRLIN_B2,
   PHX_FGTRIG_SRC_CTRLIN_B3,
   PHX_FGTRIG_SRC_CCIO_A1,
   PHX_FGTRIG_SRC_CCIO_A2,
   PHX_FGTRIG_SRC_CCIO_B1,
   PHX_FGTRIG_SRC_CCIO_B2,
   PHX_FGTRIG_SRC_AUXIN_A1,
   PHX_FGTRIG_SRC_AUXIN_A2,
   PHX_FGTRIG_SRC_AUXIN_B1,
   PHX_FGTRIG_SRC_AUXIN_B2,
   PHX_FGTRIG_SRC_OPTO_1,
   PHX_FGTRIG_SRC_OPTO_2,
   PHX_FGTRIG_SRC_AUXIN_1,
   PHX_FGTRIG_SRC_AUXIN_2,
   PHX_FGTRIG_SRC_CTRLIN_1,
   PHX_FGTRIG_SRC_CTRLIN_2,
   PHX_FGTRIG_SRC_CTRLIN_3,
   PHX_FGTRIG_SRC_CCIO_1,
   PHX_FGTRIG_SRC_CCIO_2,
   PHX_FGTRIG_SRC_TIMERA1_CHX,
   PHX_FGTRIG_SRC_TIMERA1_CH1,
   PHX_FGTRIG_SRC_TIMERA1_CH2,
   PHX_FGTRIG_SRC_TIMERA1_CH3,
   PHX_FGTRIG_SRC_TIMERA1_CH4,
   PHX_FGTRIG_SRC_422IN_CHX_0,
   PHX_FGTRIG_SRC_422IN_CH1_0,
   PHX_FGTRIG_SRC_422IN_CH2_0,
   PHX_FGTRIG_SRC_422IN_CH3_0,
   PHX_FGTRIG_SRC_422IN_CH4_0,
   PHX_FGTRIG_SRC_OPTOIN_CHX_0,
   PHX_FGTRIG_SRC_OPTOIN_CH1_0,
   PHX_FGTRIG_SRC_OPTOIN_CH2_0,
   PHX_FGTRIG_SRC_OPTOIN_CH3_0,
   PHX_FGTRIG_SRC_OPTOIN_CH4_0,
   PHX_FGTRIG_SRC_TTLIN_CHX_0,
   PHX_FGTRIG_SRC_TTLIN_CH1_0,
   PHX_FGTRIG_SRC_TTLIN_CH2_0,
   PHX_FGTRIG_SRC_TTLIN_CH3_0,
   PHX_FGTRIG_SRC_TTLIN_CH4_0,
   PHX_FGTRIG_SRC_24VIN_CHX_0,
   PHX_FGTRIG_SRC_24VIN_CH1_0,
   PHX_FGTRIG_SRC_24VIN_CH2_0,
   PHX_FGTRIG_SRC_24VIN_CH3_0,
   PHX_FGTRIG_SRC_24VIN_CH4_0,
   PHX_FGTRIG_SRC_24VIN_CH5_0,
   PHX_FGTRIG_SRC_24VIN_CH6_0,
   PHX_FGTRIG_SRC_24VIN_CH7_0,
   PHX_FGTRIG_SRC_24VIN_CH8_0,

   /* PHX_FGTRIG_MODE */
   PHX_FGTRIG_FREERUN = (int) ((ui32) PHX_FGTRIG_MODE + 1),
   PHX_FGTRIG_FIRST_POS_EDGE,
   PHX_FGTRIG_FIRST_NEG_EDGE,
   PHX_FGTRIG_EACH_POS_EDGE,
   PHX_FGTRIG_EACH_NEG_EDGE,
   PHX_FGTRIG_FIRST_POS_LEVEL,
   PHX_FGTRIG_FIRST_NEG_LEVEL,
   PHX_FGTRIG_EACH_POS_LEVEL,
   PHX_FGTRIG_EACH_NEG_LEVEL,
   PHX_FGTRIG_GATED_POS_LEVEL,
   PHX_FGTRIG_GATED_NEG_LEVEL,
   PHX_FGTRIG_EACH_POS_EDGE_NR,
   PHX_FGTRIG_EACH_NEG_EDGE_NR,

   /* PHX_FGTRIG_ALIGN */
   PHX_FGTRIG_ALIGN_NONE = (int) ((ui32) PHX_FGTRIG_ALIGN + 1),
   PHX_FGTRIG_ALIGN_TO_CLK,
   PHX_FGTRIG_ALIGN_TO_LINE,
   PHX_FGTRIG_ALIGN_TO_FRAME,

   /* PHX_FGTRIG_DELAY_MODE */
   PHX_FGTRIG_DELAY_NONE  = (int) ((ui32) PHX_FGTRIG_DELAY_MODE + 1),
   PHX_FGTRIG_DELAY_LINE,
   PHX_FGTRIG_DELAY_TIME,
   PHX_FGTRIG_DELAY_TIMERM1_CHX,
   PHX_FGTRIG_DELAY_TIMERM1_CH1,
   PHX_FGTRIG_DELAY_TIMERM1_CH2,
   PHX_FGTRIG_DELAY_TIMERM1_CH3,
   PHX_FGTRIG_DELAY_TIMERM1_CH4,

   /* PHX_TIMERA1_MODE */
   PHX_TIMERA1_MODE_RUN = (int) ((ui32) PHX_TIMERA1_MODE + 1),
   PHX_TIMERA1_MODE_N_PULSES,
   PHX_TIMERA1_MODE_STOP,
   PHX_TIMERA1_MODE_ABORT,

   /* PHX_TIMERA1_TRIG_SRC */
   PHX_TIMERA1_TRIG_SRC_NONE = (int) ((ui32) PHX_TIMERA1_TRIG_SRC + 1),
   PHX_TIMERA1_TRIG_SRC_SWTRIG_CHX,
   PHX_TIMERA1_TRIG_SRC_SWTRIG_CH1,
   PHX_TIMERA1_TRIG_SRC_SWTRIG_CH2,
   PHX_TIMERA1_TRIG_SRC_SWTRIG_CH3,
   PHX_TIMERA1_TRIG_SRC_SWTRIG_CH4,
   PHX_TIMERA1_TRIG_SRC_TIMERA1_CH1,
   PHX_TIMERA1_TRIG_SRC_TIMERA1_CH2,
   PHX_TIMERA1_TRIG_SRC_TIMERA1_CH3,
   PHX_TIMERA1_TRIG_SRC_TIMERA1_CH4,
   PHX_TIMERA1_TRIG_SRC_422IN_CHX_0_RISE,
   PHX_TIMERA1_TRIG_SRC_422IN_CH1_0_RISE,
   PHX_TIMERA1_TRIG_SRC_422IN_CH2_0_RISE,
   PHX_TIMERA1_TRIG_SRC_422IN_CH3_0_RISE,
   PHX_TIMERA1_TRIG_SRC_422IN_CH4_0_RISE,
   PHX_TIMERA1_TRIG_SRC_422IN_CHX_0_FALL,
   PHX_TIMERA1_TRIG_SRC_422IN_CH1_0_FALL,
   PHX_TIMERA1_TRIG_SRC_422IN_CH2_0_FALL,
   PHX_TIMERA1_TRIG_SRC_422IN_CH3_0_FALL,
   PHX_TIMERA1_TRIG_SRC_422IN_CH4_0_FALL,
   PHX_TIMERA1_TRIG_SRC_OPTOIN_CHX_0_RISE,
   PHX_TIMERA1_TRIG_SRC_OPTOIN_CH1_0_RISE,
   PHX_TIMERA1_TRIG_SRC_OPTOIN_CH2_0_RISE,
   PHX_TIMERA1_TRIG_SRC_OPTOIN_CH3_0_RISE,
   PHX_TIMERA1_TRIG_SRC_OPTOIN_CH4_0_RISE,
   PHX_TIMERA1_TRIG_SRC_OPTOIN_CHX_0_FALL,
   PHX_TIMERA1_TRIG_SRC_OPTOIN_CH1_0_FALL,
   PHX_TIMERA1_TRIG_SRC_OPTOIN_CH2_0_FALL,
   PHX_TIMERA1_TRIG_SRC_OPTOIN_CH3_0_FALL,
   PHX_TIMERA1_TRIG_SRC_OPTOIN_CH4_0_FALL,
   PHX_TIMERA1_TRIG_SRC_TTLIN_CHX_0_RISE,
   PHX_TIMERA1_TRIG_SRC_TTLIN_CH1_0_RISE,
   PHX_TIMERA1_TRIG_SRC_TTLIN_CH2_0_RISE,
   PHX_TIMERA1_TRIG_SRC_TTLIN_CH3_0_RISE,
   PHX_TIMERA1_TRIG_SRC_TTLIN_CH4_0_RISE,
   PHX_TIMERA1_TRIG_SRC_TTLIN_CHX_0_FALL,
   PHX_TIMERA1_TRIG_SRC_TTLIN_CH1_0_FALL,
   PHX_TIMERA1_TRIG_SRC_TTLIN_CH2_0_FALL,
   PHX_TIMERA1_TRIG_SRC_TTLIN_CH3_0_FALL,
   PHX_TIMERA1_TRIG_SRC_TTLIN_CH4_0_FALL,
   PHX_TIMERA1_TRIG_SRC_24VIN_CHX_0_RISE,
   PHX_TIMERA1_TRIG_SRC_24VIN_CHX_0_FALL,
   PHX_TIMERA1_TRIG_SRC_24VIN_CH1_0_RISE,
   PHX_TIMERA1_TRIG_SRC_24VIN_CH1_0_FALL,
   PHX_TIMERA1_TRIG_SRC_24VIN_CH2_0_RISE,
   PHX_TIMERA1_TRIG_SRC_24VIN_CH2_0_FALL,
   PHX_TIMERA1_TRIG_SRC_24VIN_CH3_0_RISE,
   PHX_TIMERA1_TRIG_SRC_24VIN_CH3_0_FALL,
   PHX_TIMERA1_TRIG_SRC_24VIN_CH4_0_RISE,
   PHX_TIMERA1_TRIG_SRC_24VIN_CH4_0_FALL,
   PHX_TIMERA1_TRIG_SRC_24VIN_CH5_0_RISE,
   PHX_TIMERA1_TRIG_SRC_24VIN_CH5_0_FALL,
   PHX_TIMERA1_TRIG_SRC_24VIN_CH6_0_RISE,
   PHX_TIMERA1_TRIG_SRC_24VIN_CH6_0_FALL,
   PHX_TIMERA1_TRIG_SRC_24VIN_CH7_0_RISE,
   PHX_TIMERA1_TRIG_SRC_24VIN_CH7_0_FALL,
   PHX_TIMERA1_TRIG_SRC_24VIN_CH8_0_RISE,
   PHX_TIMERA1_TRIG_SRC_24VIN_CH8_0_FALL,

   /* PHX_IMAGE_TIMESTAMP_MODE */
   PHX_IMAGE_TIMESTAMP_NONE = (int) ((ui32) PHX_IMAGE_TIMESTAMP_MODE + 1),
   PHX_IMAGE_TIMESTAMP_MODE1,
   PHX_IMAGE_TIMESTAMP_MODE2,

   /* PHX_EVENTCOUNT_SRC */
   PHX_EVENTCOUNT_LINE = (int) ((ui32) PHX_EVENTCOUNT_SRC + 1),
   PHX_EVENTCOUNT_FRAME,
   PHX_EVENTCOUNT_TIME,

   /* PHX_EVENTGATE_SRC */
   PHX_EVENTGATE_ACQTRIG = (int) ((ui32) PHX_EVENTGATE_SRC + 1),
   PHX_EVENTGATE_FRAME,
   PHX_EVENTGATE_ACQ,
   PHX_EVENTGATE_LINE,

   /* PHX_DST_ENDIAN */
   PHX_DST_LITTLE_ENDIAN = (int) ((ui32) PHX_DST_ENDIAN + 1),
   PHX_DST_BIG_ENDIAN,

   /* PHX_CHAN_SYNC_MODE */
   PHX_CHAN_SYNC_NONE = (int) ((ui32) PHX_CHAN_SYNC_MODE + 1),
   PHX_CHAN_SYNC_ACQEXPTRIG,

   /* PHX_CVB_PARAM */
   PHX_CVB_WIDTH = (int) ((ui32) PHX_CVB_PARAM + 1),
   PHX_CVB_HEIGHT,
   PHX_CVB_PLANES,
   PHX_CVB_BIT_DEPTH,   /* per plane */
   PHX_CVB_BYTES_PER_PIXEL,
   PHX_CVB_X_STEP,
   PHX_CVB_Y_STEP,
   PHX_CVB_PLANE_STEP,
   PHX_CVB_MALLOC,

   /* PHX_ACQ_AUTO_RESTART, these parameter values are 'OR'able together */
   PHX_ACQ_AUTO_NONE          = (int) ((ui32) PHX_ACQ_AUTO_RESTART + 0x01),
   PHX_ACQ_AUTO_SYNC_LOST     = (int) ((ui32) PHX_ACQ_AUTO_RESTART + 0x02),
   PHX_ACQ_AUTO_FIFO_OVERFLOW = (int) ((ui32) PHX_ACQ_AUTO_RESTART + 0x04),

   /* PHX_BOARD_VARIANT */
   PHX_BOARD_DIGITAL             = (int) ((ui32) PHX_DIGITAL),
   PHX_BOARD_PHX_D24CL_PE1       = (int) ((ui32) PHX_D24CL_PE1),
   PHX_BOARD_PHX_D24CL_PE1_MIR   = (int) ((ui32) PHX_D24CL_PE1_MIR),
   PHX_BOARD_PHX_D48CL_PE1       = (int) ((ui32) PHX_D48CL_PE1),
   PHX_BOARD_PHX_D48CL_PE4       = (int) ((ui32) PHX_D48CL_PE4),
   PHX_BOARD_PHX_D64CL_PE4       = (int) ((ui32) PHX_D64CL_PE4),
   PHX_BOARD_PHX_D24CL_PCI32     = (int) ((ui32) PHX_D24CL_PCI32),
   PHX_BOARD_PHX_D48CL_PCI32     = (int) ((ui32) PHX_D48CL_PCI32),
   PHX_BOARD_PHX_D48CL_PCI64     = (int) ((ui32) PHX_D48CL_PCI64),
   PHX_BOARD_PHX_D48CL_PCI64U    = (int) ((ui32) PHX_D48CL_PCI64U),
   PHX_BOARD_PHX_D10HDSDI_PE1    = (int) ((ui32) PHX_D10HDSDI_PE1),
   PHX_BOARD_PHX_D20HDSDI_PE1    = (int) ((ui32) PHX_D20HDSDI_PE1),
   PHX_BOARD_PHX_D10HDSDI_PE4    = (int) ((ui32) PHX_D10HDSDI_PE4),
   PHX_BOARD_PHX_D20HDSDI_PE4    = (int) ((ui32) PHX_D20HDSDI_PE4),
   PHX_BOARD_PHX_D36_PE1         = (int) ((ui32) PHX_D36_PE1),
   PHX_BOARD_PHX_D36_PE4         = (int) ((ui32) PHX_D36_PE4),
   PHX_BOARD_PHX_D32_PCI32       = (int) ((ui32) PHX_D32_PCI32),
   PHX_BOARD_PHX_D36_PCI32       = (int) ((ui32) PHX_D36_PCI32),
   PHX_BOARD_PHX_D36_PCI64       = (int) ((ui32) PHX_D36_PCI64),
   PHX_BOARD_PHX_D36_PCI64U      = (int) ((ui32) PHX_D36_PCI64U),
   PHX_BOARD_PHX_D24AVDS_PE1     = (int) ((ui32) PHX_D24AVDS_PE1),
   PHX_BOARD_FBD_1XCLD_2PE8,
   PHX_BOARD_FBD_4XCXP6_2PE8,
   PHX_BOARD_FBD_2XCLD_2PE8,
   PHX_BOARD_FBD_1XCXP6_2PE8,
   PHX_BOARD_FBD_2XCXP6_2PE8,
   PHX_BOARD_FBD_1XCLM_2PE8,
   PHX_BOARD_FBD_2XCLM_2PE8,
   PHX_BOARD_FBD_1XCLD_TTLA_2PE8,
   PHX_BOARD_FBD_4XCXP6_TTLA_2PE8,
   PHX_BOARD_FBD_1XCLM_TTLA_2PE8,
   PHX_BOARD_FBD_1XCLD_2PE8_MIR,
   PHX_BOARD_FBD_4XCXP6_2PE8_MIR,
   PHX_BOARD_FBD_1XCLM_2PE8_MIR,
   PHX_BOARD_FBD_1XCXPLR1_2PE8,
   PHX_BOARD_FBD_1XCLD_2PE4,
   PHX_BOARD_FBD_1XCLD_2PE4_SW,
   PHX_BOARD_FBD_1XCLD_2PE4L,
   PHX_BOARD_FBD_1XCLD_3CPCIS_2PE4,
   PHX_BOARD_FBD_1XCXP6_2PE8_MIR,
   PHX_BOARD_FBD_4XCXP6_3PE4,
   PHX_BOARD_FBD_1XCXP6_2PE4L,
   PHX_BOARD_FBD_4XCXP6_2PE8_MIR_A,
   PHX_BOARD_FBD_4XCXP12_3PE4,
   PHX_BOARD_FBD_4XCXP12_3PE8,
   PHX_BOARD_FBD_4XCXP12_3PE8_MIR,

   /* PHX_BOARD_NUMBER */
   PHX_BOARD_NUMBER_AUTO      = (int) ((ui32) PHX_BOARD_AUTO),
   PHX_BOARD_NUMBER_1         = (int) ((ui32) PHX_BOARD1),
   PHX_BOARD_NUMBER_2         = (int) ((ui32) PHX_BOARD2),
   PHX_BOARD_NUMBER_3         = (int) ((ui32) PHX_BOARD3),
   PHX_BOARD_NUMBER_4         = (int) ((ui32) PHX_BOARD4),
   PHX_BOARD_NUMBER_5         = (int) ((ui32) PHX_BOARD5),
   PHX_BOARD_NUMBER_6         = (int) ((ui32) PHX_BOARD6),
   PHX_BOARD_NUMBER_7         = (int) ((ui32) PHX_BOARD7),  /* Add or reduce these, if PHX_BOARD_MAX changes */

   /* PHX_CHANNEL_NUMBER */
   PHX_CHANNEL_NUMBER_AUTO    = (int) ((ui32) PHX_CHANNEL_AUTO),
   PHX_CHANNEL_NUMBER_1       = (int) ((ui32) PHX_CHANNEL_A),
   PHX_CHANNEL_NUMBER_2       = (int) ((ui32) PHX_CHANNEL_B),
   PHX_CHANNEL_NUMBER_3,
   PHX_CHANNEL_NUMBER_4,
   PHX_CHANNEL_NUMBER_5,
   PHX_CHANNEL_NUMBER_6,
   PHX_CHANNEL_NUMBER_7,
   PHX_CHANNEL_NUMBER_8,

   /* PHX_CONFIG_MODE */
   PHX_CONFIG_NORMAL             = (int) ((ui32) PHX_MODE_NORMAL),
   PHX_CONFIG_COMMS_ONLY         = (int) ((ui32) PHX_COMMS_ONLY),
   PHX_CONFIG_ACQ_ONLY           = (int) ((ui32) PHX_ACQ_ONLY),
   PHX_CONFIG_NO_CHILDREN        = (int) ((ui32) PHX_COMMS_ONLY | (ui32) PHX_ACQ_ONLY ),

   /* PHX_BOARD_BUS_ADDRESS */
   PHX_BOARD_INVALID_BUS_ADDRESS = (int) 0x80000000,

   /* PHX_CXP_BITRATE_MODE */
   PHX_CXP_BITRATE_MODE_AUTO = (int) ((ui32) PHX_CXP_BITRATE_MODE + 1),
   PHX_CXP_BITRATE_MODE_CXP1,
   PHX_CXP_BITRATE_MODE_CXP2,
   PHX_CXP_BITRATE_MODE_CXP3,
   PHX_CXP_BITRATE_MODE_CXP5,
   PHX_CXP_BITRATE_MODE_CXP6,
   PHX_CXP_BITRATE_MODE_CXP10,
   PHX_CXP_BITRATE_MODE_CXP12,

   /* PHX_CXP_BITRATE */
   PHX_CXP_BITRATE_UNKNOWN = (int) ((ui32) PHX_CXP_BITRATE + 1),
   PHX_CXP_BITRATE_CXP1    =               PHX_CXP_BITRATE_MODE_CXP1,
   PHX_CXP_BITRATE_CXP2    =               PHX_CXP_BITRATE_MODE_CXP2,
   PHX_CXP_BITRATE_CXP3    =               PHX_CXP_BITRATE_MODE_CXP3,
   PHX_CXP_BITRATE_CXP5    =               PHX_CXP_BITRATE_MODE_CXP5,
   PHX_CXP_BITRATE_CXP6    =               PHX_CXP_BITRATE_MODE_CXP6,
   PHX_CXP_BITRATE_CXP10   =               PHX_CXP_BITRATE_MODE_CXP10,
   PHX_CXP_BITRATE_CXP12   =               PHX_CXP_BITRATE_MODE_CXP12,

   /* PHX_CXP_POCXP_MODE */
   PHX_CXP_POCXP_MODE_AUTO = (int) ((ui32) PHX_CXP_POCXP_MODE + 1),
   PHX_CXP_POCXP_MODE_OFF,
   PHX_CXP_POCXP_MODE_TRIP_RESET,
   PHX_CXP_POCXP_MODE_FORCEON,

   /* PHX_CL_POCL_MODE */
   PHX_CL_POCL_MODE_AUTO = (int) ((ui32) PHX_CL_POCL_MODE + 1),
   PHX_CL_POCL_MODE_OFF,
   PHX_CL_POCL_MODE_TRIP_RESET,


   /* PHX_CXP_DISCOVERY_MODE */
   PHX_CXP_DISCOVERY_MODE_AUTO = (int) ((ui32) PHX_CXP_DISCOVERY_MODE + 1),
   PHX_CXP_DISCOVERY_MODE_1X,
   PHX_CXP_DISCOVERY_MODE_2X,
   PHX_CXP_DISCOVERY_MODE_4X,

   /* PHX_CXP_DISCOVERY */
   PHX_CXP_DISCOVERY_UNKNOWN = (int) ((ui32) PHX_CXP_DISCOVERY + 1),
   PHX_CXP_DISCOVERY_1X      =               PHX_CXP_DISCOVERY_MODE_1X,
   PHX_CXP_DISCOVERY_2X      =               PHX_CXP_DISCOVERY_MODE_2X,
   PHX_CXP_DISCOVERY_4X      =               PHX_CXP_DISCOVERY_MODE_4X,

   /* PHX_DST_ALIGMNENT */
   PHX_DST_LSB_ALIGNED = (int) ((ui32) PHX_DST_ALIGNMENT + 1),
   PHX_DST_MSB_ALIGNED,

   /* PHX_ACQ_BUFFER_MEMSET_MODE */
   PHX_ACQ_BUFFER_MEMSET_NONE = (int) ((ui32) PHX_ACQ_BUFFER_MEMSET_MODE + 1),
   PHX_ACQ_BUFFER_MEMSET_START,
   PHX_ACQ_BUFFER_MEMSET_ALWAYS,

   /* PHX_INTRPT_CONDITIONS */
   PHX_INTRPT_TEST               = 0x00000001,
   PHX_INTRPT_BUFFER_READY       = 0x00000002,
   PHX_INTRPT_FIFO_OVERFLOW      = 0x00000004,
   PHX_INTRPT_FRAME_LOST         = 0x00000008,
   PHX_INTRPT_CAPTURE_COMPLETE   = 0x00000010,
   PHX_INTRPT_FRAME_START        = 0x00000020,
   PHX_INTRPT_FRAME_END          = 0x00000040,
   PHX_INTRPT_LINE_START         = 0x00000080,
   PHX_INTRPT_LINE_END           = 0x00000100,
   PHX_INTRPT_FGTRIG_START       = 0x00000200,
   PHX_INTRPT_FGTRIG_END         = 0x00000400,
   PHX_INTRPT_TIMEOUT            = 0x00000800,
   PHX_INTRPT_SYNC_LOST          = 0x00001000,
   PHX_INTRPT_TIMERA1            = 0x00002000,
   PHX_INTRPT_CXP_ERROR          = 0x00004000,
   PHX_INTRPT_TIMERM1            = 0x00010000,
   PHX_INTRPT_TIMERM2            = 0x00020000,
   PHX_INTRPT_CAMERA_TRIGGER     = 0x00040000,
   PHX_INTRPT_GLOBAL_ENABLE      = (int) 0x80000000,

#if !defined _PHX_NO_BACKWARDS_COMPATIBILITY_
   /* Retained for backwards compatibility with previous software releases.
    * Do not use for new applications.
    */

   /* PHX_CONFIG_MODE */
   PHX_CONFIG_FW_ONLY   = PHX_CONFIG_NO_CHILDREN,  /* For backwards compatibility from 8.49.0 */

   /* PHX_CAM_SRC_COL */
   PHX_CAM_SRC_BAY_RGGB = PHX_CAM_SRC_BAYER_RG,
   PHX_CAM_SRC_BAY_GRBG = PHX_CAM_SRC_BAYER_GR,
   PHX_CAM_SRC_BAY_GBRG = PHX_CAM_SRC_BAYER_GB,
   PHX_CAM_SRC_BAY_BGGR = PHX_CAM_SRC_BAYER_BG,
   PHX_CAM_SRC_YUV      = PHX_CAM_SRC_YUV422,      /* For backwards compatibility from 7.3.15 */

  /* PHX_CAM_HTAP_DIR */
   PHX_CAM_HTAP_BOTH = PHX_CAM_HTAP_CONVERGE,      /* For Backwards compatibility from 2.10.1 */

   /* PHX_CAM_HTAP_TYPE */
   PHX_CAM_HTAP_OFFSET =    PHX_CAM_HTAP_OFFSET_1, /* Backward Compatibility prior to 4.14 */

   /* PHX_CAM_VTAP_DIR */
   PHX_CAM_VTAP_BOTH = PHX_CAM_VTAP_CONVERGE,      /* Backward compatibility prior to v6.34 */

   /* PHX_ACQ_TYPE */
   PHX_ACQ_FRAME    = PHX_ACQ_FRAME_12,            /* For Backwards compatibility from 2.8.2 */

   /* PHX_DST_FORMAT */
   PHX_DST_FORMAT_Y8    = PHX_BUS_FORMAT_MONO8,
   PHX_DST_FORMAT_Y16   = PHX_BUS_FORMAT_MONO16,
   PHX_DST_FORMAT_Y32   = PHX_BUS_FORMAT_MONO32,
   PHX_DST_FORMAT_Y36   = PHX_BUS_FORMAT_MONO36,     /* No longer supported in version 2 release */
   PHX_DST_FORMAT_RGB15 = PHX_BUS_FORMAT_BGR5,
   PHX_DST_FORMAT_RGB16 = PHX_BUS_FORMAT_BGR565,
   PHX_DST_XBGR32       = PHX_BUS_FORMAT_XBGR8,      /* PHX_DST_FORMAT_RGBX32, */
   PHX_DST_BGRX32       = PHX_BUS_FORMAT_BGRX8,      /* PHX_DST_FORMAT_XRGB32, */
   PHX_DST_FORMAT_RGB48 = PHX_BUS_FORMAT_BGR16,
   PHX_DST_FORMAT_BGR15 = PHX_BUS_FORMAT_RGB5,
   PHX_DST_FORMAT_BGR16 = PHX_BUS_FORMAT_RGB565,
   PHX_DST_XRGB32       = PHX_BUS_FORMAT_XRGB8,      /* PHX_DST_FORMAT_BGRX32, */
   PHX_DST_RGBX32       = PHX_BUS_FORMAT_RGBX8,      /* PHX_DST_FORMAT_XBGR32, */
   PHX_DST_FORMAT_BGR48 = PHX_BUS_FORMAT_RGB16,
   PHX_DST_FORMAT_RGB32 = PHX_BUS_FORMAT_BGR101210,  /* Added in version 2 release */
   PHX_DST_FORMAT_BGR32 = PHX_BUS_FORMAT_RGB101210,
   PHX_DST_FORMAT_RGB24 = PHX_BUS_FORMAT_BGR8,       /* Added in version 2.25 release */
   PHX_DST_FORMAT_BGR24 = PHX_BUS_FORMAT_RGB8,
   PHX_DST_FORMAT_Y10   = PHX_BUS_FORMAT_MONO10,
   PHX_DST_FORMAT_Y12   = PHX_BUS_FORMAT_MONO12,
   PHX_DST_FORMAT_Y14   = PHX_BUS_FORMAT_MONO14,
/* PHX_DST_FORMAT_BAY8,
   PHX_DST_FORMAT_BAY10,
   PHX_DST_FORMAT_BAY12,
   PHX_DST_FORMAT_BAY14,
   PHX_DST_FORMAT_BAY16, */
   PHX_DST_FORMAT_Y12_PACKED = PHX_BUS_FORMAT_MONO12P,
   PHX_DST_FORMAT_RGB36      = PHX_BUS_FORMAT_BGR12,
   PHX_DST_FORMAT_BGR36      = PHX_BUS_FORMAT_RGB12,
   PHX_DST_FORMAT_YUV422     = PHX_BUS_FORMAT_YUV422_8,
/* PHX_DST_FORMAT_Y12B, */
   PHX_DST_FORMAT_RRGGBB8    = PHX_BUS_FORMAT_RGB8_PLANAR,
   PHX_DST_FORMAT_Y10_PACKED = PHX_BUS_FORMAT_MONO10P,
   PHX_DST_FORMAT_Y14_PACKED = PHX_BUS_FORMAT_MONO14P,

   PHX_DST_FORMAT_RGBX32 = PHX_BUS_FORMAT_XBGR8,   /* Backward Compatibility prior to 5.54, New names are consistent with TMG */
   PHX_DST_FORMAT_XRGB32 = PHX_BUS_FORMAT_BGRX8,
   PHX_DST_FORMAT_BGRX32 = PHX_BUS_FORMAT_XRGB8,
   PHX_DST_FORMAT_XBGR32 = PHX_BUS_FORMAT_RGBX8,

   /* PHX_USER_FORMAT */
   PHX_USER_FORMAT_Y8    = PHX_USR_FORMAT_MONO8,
   PHX_USER_FORMAT_Y16   = PHX_USR_FORMAT_MONO16,
   PHX_USER_FORMAT_Y32   = PHX_USR_FORMAT_MONO32,
   PHX_USER_FORMAT_Y36   = PHX_USR_FORMAT_MONO36,     /* No longer supported in version 2 release */
   PHX_USER_FORMAT_RGB15 = PHX_USR_FORMAT_BGR5,
   PHX_USER_FORMAT_RGB16 = PHX_USR_FORMAT_BGR565,
   PHX_USER_XBGR32       = PHX_USR_FORMAT_XBGR8,      /* PHX_DST_FORMAT_RGBX32, */
   PHX_USER_BGRX32       = PHX_USR_FORMAT_BGRX8,      /* PHX_DST_FORMAT_XRGB32, */
   PHX_USER_FORMAT_RGB48 = PHX_USR_FORMAT_BGR16,
   PHX_USER_FORMAT_BGR15 = PHX_USR_FORMAT_RGB5,
   PHX_USER_FORMAT_BGR16 = PHX_USR_FORMAT_RGB565,
   PHX_USER_XRGB32       = PHX_USR_FORMAT_XRGB8,      /* PHX_DST_FORMAT_BGRX32, */
   PHX_USER_RGBX32       = PHX_USR_FORMAT_RGBX8,      /* PHX_DST_FORMAT_XBGR32, */
   PHX_USER_FORMAT_BGR48 = PHX_USR_FORMAT_RGB16,
   PHX_USER_FORMAT_RGB32 = PHX_USR_FORMAT_BGR101210,  /* Added in version 2 release */
   PHX_USER_FORMAT_BGR32 = PHX_USR_FORMAT_RGB101210,
   PHX_USER_FORMAT_RGB24 = PHX_USR_FORMAT_BGR8,       /* Added in version 2.25 release */
   PHX_USER_FORMAT_BGR24 = PHX_USR_FORMAT_RGB8,
   PHX_USER_FORMAT_Y10   = PHX_USR_FORMAT_MONO10,
   PHX_USER_FORMAT_Y12   = PHX_USR_FORMAT_MONO12,
   PHX_USER_FORMAT_Y14   = PHX_USR_FORMAT_MONO14,
/* PHX_USER_FORMAT_BAY8,
   PHX_USER_FORMAT_BAY10,
   PHX_USER_FORMAT_BAY12,
   PHX_USER_FORMAT_BAY14,
   PHX_USER_FORMAT_BAY16, */
   PHX_USER_FORMAT_Y12_PACKED = PHX_USR_FORMAT_MONO12P,
   PHX_USER_FORMAT_RGB36      = PHX_USR_FORMAT_BGR12,
   PHX_USER_FORMAT_BGR36      = PHX_USR_FORMAT_RGB12,
   PHX_USER_FORMAT_YUV422     = PHX_USR_FORMAT_YUV422_8,
/* PHX_USER_FORMAT_Y12B, */
   PHX_USER_FORMAT_RRGGBB8    = PHX_USR_FORMAT_RGB8_PLANAR,
   PHX_USER_FORMAT_Y10_PACKED = PHX_USR_FORMAT_MONO10P,
   PHX_USER_FORMAT_Y14_PACKED = PHX_USR_FORMAT_MONO14P,

   PHX_USER_FORMAT_RGBX32 = PHX_USR_FORMAT_XBGR8,   /* Backward Compatibility prior to 5.54, New names are consistent with TMG */
   PHX_USER_FORMAT_XRGB32 = PHX_USR_FORMAT_BGRX8,
   PHX_USER_FORMAT_BGRX32 = PHX_USR_FORMAT_XRGB8,
   PHX_USER_FORMAT_XBGR32 = PHX_USR_FORMAT_RGBX8,

   /* PHX_LINETRIG_SRC */
   PHX_LINETRIG_CTRLIN_1_RISING  = PHX_LINETRIG_AUXIN_1_RISING,      /* Backward Compatibility prior to 2.4.0 */
   PHX_LINETRIG_CTRLIN_1_FALLING = PHX_LINETRIG_AUXIN_1_FALLING,
   PHX_LINETRIG_CTRLIN_3_RISING  = PHX_LINETRIG_AUXIN_2_RISING,
   PHX_LINETRIG_CTRLIN_3_FALLING = PHX_LINETRIG_AUXIN_2_FALLING,

   /* PHX_LINETRIG_TIMER_CTRL */
   PHX_LINETRIG_TIMER_START = PHX_LINETRIG_TIMER_TIME,      /* Backward Compatibility prior to 3.53.0 */
   PHX_LINETRIG_TIMER_STOP  = PHX_LINETRIG_TIMER_DISABLE,

   /* PHX_CAMTRIG_SRC */
   PHX_EXPTRIG_LINETRIG         = PHX_CAMTRIG_SRC_LINETRIG,    /* Backward compatibility */
   PHX_EXPTRIG_ACQTRIG          = PHX_CAMTRIG_SRC_FGTRIGA_CHX,
   PHX_EXPTRIG_NONE             = PHX_CAMTRIG_SRC_NONE,
   PHX_EXPTRIG_SWTRIG           = PHX_CAMTRIG_SRC_SWTRIG_CHX,
   PHX_EXPTRIG_AUXIN_1_RISING   = PHX_CAMTRIG_SRC_AUXIN_1_RISE,        /* Relative */
   PHX_EXPTRIG_AUXIN_1_FALLING  = PHX_CAMTRIG_SRC_AUXIN_1_FALL,
   PHX_EXPTRIG_AUXIN_2_RISING   = PHX_CAMTRIG_SRC_AUXIN_2_RISE,
   PHX_EXPTRIG_AUXIN_2_FALLING  = PHX_CAMTRIG_SRC_AUXIN_2_FALL,
   PHX_EXPTRIG_TIMER            = PHX_CAMTRIG_SRC_TIMERA1_CHX,
   PHX_EXPTRIG_AUXIN_A1_RISING  = PHX_CAMTRIG_SRC_AUXIN_A1_RISE,       /* Absolute */
   PHX_EXPTRIG_AUXIN_A1_FALLING = PHX_CAMTRIG_SRC_AUXIN_A1_FALL,
   PHX_EXPTRIG_AUXIN_A2_RISING  = PHX_CAMTRIG_SRC_AUXIN_A2_RISE,
   PHX_EXPTRIG_AUXIN_A2_FALLING = PHX_CAMTRIG_SRC_AUXIN_A2_FALL,
   PHX_EXPTRIG_AUXIN_B1_RISING  = PHX_CAMTRIG_SRC_AUXIN_B1_RISE,
   PHX_EXPTRIG_AUXIN_B1_FALLING = PHX_CAMTRIG_SRC_AUXIN_B1_FALL,
   PHX_EXPTRIG_AUXIN_B2_RISING  = PHX_CAMTRIG_SRC_AUXIN_B2_RISE,
   PHX_EXPTRIG_AUXIN_B2_FALLING = PHX_CAMTRIG_SRC_AUXIN_B2_FALL,
   PHX_EXP_LINETRIG = PHX_EXPTRIG_LINETRIG,                       /* Backward compatibility prior to 3.5.0 */
   PHX_EXP_ACQTRIG  = PHX_EXPTRIG_ACQTRIG,

   /* PHX_EXP_LINESTART */
   PHX_EXP_LINESTART_CTRLIO_2 = PHX_EXP_LINESTART_CCIO_2,   /* Backward Compatibility prior to 2.4.0 */

   /* PHX_FGTRIG_SRC */                            /* Backward compatibility */
   PHX_ACQTRIG_OPTO_A1   = PHX_FGTRIG_SRC_OPTO_A1, /* Absolute */
   PHX_ACQTRIG_OPTO_A2   = PHX_FGTRIG_SRC_OPTO_A2,
   PHX_ACQTRIG_OPTO_B1   = PHX_FGTRIG_SRC_OPTO_B1,
   PHX_ACQTRIG_OPTO_B2   = PHX_FGTRIG_SRC_OPTO_B2,
   PHX_ACQTRIG_CTRLIN_A1 = PHX_FGTRIG_SRC_CTRLIN_A1,
   PHX_ACQTRIG_CTRLIN_A2 = PHX_FGTRIG_SRC_CTRLIN_A2,
   PHX_ACQTRIG_CTRLIN_A3 = PHX_FGTRIG_SRC_CTRLIN_A3,
   PHX_ACQTRIG_CTRLIN_B1 = PHX_FGTRIG_SRC_CTRLIN_B1,
   PHX_ACQTRIG_CTRLIN_B2 = PHX_FGTRIG_SRC_CTRLIN_B2,
   PHX_ACQTRIG_CTRLIN_B3 = PHX_FGTRIG_SRC_CTRLIN_B3,
   PHX_ACQTRIG_CCIO_A1   = PHX_FGTRIG_SRC_CCIO_A1,
   PHX_ACQTRIG_CCIO_A2   = PHX_FGTRIG_SRC_CCIO_A2,
   PHX_ACQTRIG_CCIO_B1   = PHX_FGTRIG_SRC_CCIO_B1,
   PHX_ACQTRIG_CCIO_B2   = PHX_FGTRIG_SRC_CCIO_B2,
   PHX_ACQTRIG_AUXIN_A1  = PHX_FGTRIG_SRC_AUXIN_A1,
   PHX_ACQTRIG_AUXIN_A2  = PHX_FGTRIG_SRC_AUXIN_A2,
   PHX_ACQTRIG_AUXIN_B1  = PHX_FGTRIG_SRC_AUXIN_B1,
   PHX_ACQTRIG_AUXIN_B2  = PHX_FGTRIG_SRC_AUXIN_B2,
   PHX_ACQTRIG_OPTO_1    = PHX_FGTRIG_SRC_OPTO_1,        /* Relative */
   PHX_ACQTRIG_OPTO_2    = PHX_FGTRIG_SRC_OPTO_2,
   PHX_ACQTRIG_AUXIN_1   = PHX_FGTRIG_SRC_AUXIN_1,
   PHX_ACQTRIG_AUXIN_2   = PHX_FGTRIG_SRC_AUXIN_2,
   PHX_ACQTRIG_CTRLIN_1  = PHX_FGTRIG_SRC_CTRLIN_1,
   PHX_ACQTRIG_CTRLIN_2  = PHX_FGTRIG_SRC_CTRLIN_2,
   PHX_ACQTRIG_CTRLIN_3  = PHX_FGTRIG_SRC_CTRLIN_3,
   PHX_ACQTRIG_CCIO_1    = PHX_FGTRIG_SRC_CCIO_1,
   PHX_ACQTRIG_CCIO_2    = PHX_FGTRIG_SRC_CCIO_2,
   PHX_ACQTRIG_TIMER     = PHX_FGTRIG_SRC_TIMERA1_CHX,
   PHX_ACQTRIG_OPTO1     = PHX_ACQTRIG_OPTO_A1,          /* Backward Compatibility prior to 2.4.0 */
   PHX_ACQTRIG_OPTO2     = PHX_ACQTRIG_OPTO_A2,
   PHX_ACQTRIG_OPTO3     = PHX_ACQTRIG_OPTO_B1,
   PHX_ACQTRIG_OPTO4     = PHX_ACQTRIG_OPTO_B2,
   PHX_ACQTRIG_CTRL1IN_1 = PHX_ACQTRIG_CTRLIN_A1,
   PHX_ACQTRIG_CTRL1IN_2 = PHX_ACQTRIG_CTRLIN_A2,
   PHX_ACQTRIG_CTRL1IN_3 = PHX_ACQTRIG_CTRLIN_A3,
   PHX_ACQTRIG_CTRL2IN_1 = PHX_ACQTRIG_CTRLIN_B1,
   PHX_ACQTRIG_CTRL2IN_2 = PHX_ACQTRIG_CTRLIN_B2,
   PHX_ACQTRIG_CTRL2IN_3 = PHX_ACQTRIG_CTRLIN_B3,
   PHX_ACQTRIG_CTRLIO_1  = PHX_ACQTRIG_CCIO_A1,
   PHX_ACQTRIG_CTRLIO_2  = PHX_ACQTRIG_CCIO_A2,
   PHX_ACQTRIG_CTRLIO_3  = PHX_ACQTRIG_CCIO_B1,
   PHX_ACQTRIG_CTRLIO_4  = PHX_ACQTRIG_CCIO_B2,

   /* PHX_FGTRIG_MODE */
   PHX_ACQTRIG_NONE            = PHX_FGTRIG_FREERUN,           /* Backward compatibility */
   PHX_ACQTRIG_FIRST_POS_EDGE  = PHX_FGTRIG_FIRST_POS_EDGE,
   PHX_ACQTRIG_FIRST_NEG_EDGE  = PHX_FGTRIG_FIRST_NEG_EDGE,
   PHX_ACQTRIG_EACH_POS_EDGE   = PHX_FGTRIG_EACH_POS_EDGE,
   PHX_ACQTRIG_EACH_NEG_EDGE   = PHX_FGTRIG_EACH_NEG_EDGE,
   PHX_ACQTRIG_FIRST_POS_LEVEL = PHX_FGTRIG_FIRST_POS_LEVEL,
   PHX_ACQTRIG_FIRST_NEG_LEVEL = PHX_FGTRIG_FIRST_NEG_LEVEL,
   PHX_ACQTRIG_EACH_POS_LEVEL  = PHX_FGTRIG_EACH_POS_LEVEL,
   PHX_ACQTRIG_EACH_NEG_LEVEL  = PHX_FGTRIG_EACH_NEG_LEVEL,
   PHX_ACQTRIG_GATED_POS_LEVEL = PHX_FGTRIG_GATED_POS_LEVEL,
   PHX_ACQTRIG_GATED_NEG_LEVEL = PHX_FGTRIG_GATED_NEG_LEVEL,

   /* PHX_FGTRIG_ALIGN */
   PHX_ACQTRIG_ALIGN_NONE     = PHX_FGTRIG_ALIGN_NONE,      /* Backward compatibility */
   PHX_ACQTRIG_ALIGN_TO_CLK   = PHX_FGTRIG_ALIGN_TO_CLK,
   PHX_ACQTRIG_ALIGN_TO_LINE  = PHX_FGTRIG_ALIGN_TO_LINE,
   PHX_ACQTRIG_ALIGN_TO_FRAME = PHX_FGTRIG_ALIGN_TO_FRAME,

   /* PHX_FGTRIG_DELAY_MODE */
   PHX_ACQTRIG_DELAY_NONE  = PHX_FGTRIG_DELAY_NONE,   /* Backward compatibility */
   PHX_ACQTRIG_DELAY_LINE  = PHX_FGTRIG_DELAY_LINE,
   PHX_ACQTRIG_DELAY_TIMER = PHX_FGTRIG_DELAY_TIME,

   /* PHX_EVENTGATE_SRC */
   PHX_EVENTGATE_START = PHX_EVENTGATE_ACQ,     /* Retained for backwards compatibility */

   /* PHX_BOARD_VARIANT */
   PHX_BOARD_FBD              = PHX_BOARD_FBD_1XCLD_2PE8,   /* Backwards compatibility, prior to v6.36 */

   /* PHX_INTRPT_CONDITIONS */
   PHX_INTRPT_DMA                = PHX_INTRPT_BUFFER_READY,    /* Retained for backwards compatibility */
   PHX_INTRPT_FIFO_A_OVERFLOW    = PHX_INTRPT_FIFO_OVERFLOW,   /* Retained for backwards compatibility */
   PHX_INTRPT_ACQ_TRIG_START     = PHX_INTRPT_FGTRIG_START,    /* Retained for backwards compatibility */
   PHX_INTRPT_ACQ_TRIG_END       = PHX_INTRPT_FGTRIG_END,      /* Retained for backwards compatibility */
   PHX_INTRPT_TIMER              = PHX_INTRPT_TIMERA1,         /* Retained for backwards compatibility */
/* PHX_INTRPT_FIFO_B_OVERFLOW    = 0x00000008,   * Removed in v3.0 of SDK */

   /* PHX_ACQ_STATUS */
   PHX_STATUS_IDLE                  = PHX_ACQ_STATUS_IDLE,
   PHX_STATUS_ACQ_IN_PROGRESS       = PHX_ACQ_STATUS_IN_PROGRESS,
   PHX_STATUS_WAITING_FOR_TRIGGER   = PHX_ACQ_STATUS_WAITING_FOR_TRIGGER
#endif
} etParamValue ;


/*
PHX_IO_METHOD()
*/
typedef enum {
   PHX_EMASK_IO_METHOD               = 0x3F000000,
   PHX_IO_METHOD_WRITE               = 0x00000000,
   PHX_IO_METHOD_READ                = 0x00000000,
   PHX_IO_METHOD_BIT_SET             = 0x01000000,
   PHX_IO_METHOD_BIT_CLR             = 0x02000000,
   PHX_IO_METHOD_BIT_TIMERMX_POS     = 0x04000000,
   PHX_IO_METHOD_BIT_FGTRIGD_POS     = 0x05000000,
   PHX_IO_METHOD_BIT_FIFO_WARN_POS   = 0x06000000,
   PHX_IO_METHOD_BIT_TIMERMX_NEG     = 0x08000000,
   PHX_IO_METHOD_BIT_FGTRIGD_NEG     = 0x09000000,
   PHX_IO_METHOD_BIT_FIFO_WARN_NEG   = 0x0A000000,
   PHX_IO_METHOD_BIT_TIMERM1_POS_CHX = 0x10000000,
   PHX_IO_METHOD_BIT_TIMERM1_NEG_CHX = 0x11000000,
   PHX_IO_METHOD_BIT_TIMERM1_POS_CH1 = 0x12000000,
   PHX_IO_METHOD_BIT_TIMERM1_NEG_CH1 = 0x13000000,
   PHX_IO_METHOD_BIT_TIMERM1_POS_CH2 = 0x14000000,
   PHX_IO_METHOD_BIT_TIMERM1_NEG_CH2 = 0x15000000,
   PHX_IO_METHOD_BIT_TIMERM1_POS_CH3 = 0x16000000,
   PHX_IO_METHOD_BIT_TIMERM1_NEG_CH3 = 0x17000000,
   PHX_IO_METHOD_BIT_TIMERM1_POS_CH4 = 0x18000000,
   PHX_IO_METHOD_BIT_TIMERM1_NEG_CH4 = 0x19000000,
   PHX_IO_METHOD_BIT_TIMERM2_POS_CHX = 0x1A000000,
   PHX_IO_METHOD_BIT_TIMERM2_NEG_CHX = 0x1B000000,
   PHX_IO_METHOD_BIT_TIMERM2_POS_CH1 = 0x1C000000,
   PHX_IO_METHOD_BIT_TIMERM2_NEG_CH1 = 0x1D000000,
   PHX_IO_METHOD_BIT_TIMERM2_POS_CH2 = 0x1E000000,
   PHX_IO_METHOD_BIT_TIMERM2_NEG_CH2 = 0x1F000000,
   PHX_IO_METHOD_BIT_TIMERM2_POS_CH3 = 0x20000000,
   PHX_IO_METHOD_BIT_TIMERM2_NEG_CH3 = 0x21000000,
   PHX_IO_METHOD_BIT_TIMERM2_POS_CH4 = 0x22000000,
   PHX_IO_METHOD_BIT_TIMERM2_NEG_CH4 = 0x23000000,
   PHX_IO_METHOD_BIT_FGTRIGD_POS_CHX = 0x24000000,
   PHX_IO_METHOD_BIT_FGTRIGD_NEG_CHX = 0x25000000,
   PHX_IO_METHOD_BIT_FGTRIGD_POS_CH1 = 0x26000000,
   PHX_IO_METHOD_BIT_FGTRIGD_NEG_CH1 = 0x27000000,
   PHX_IO_METHOD_BIT_FGTRIGD_POS_CH2 = 0x28000000,
   PHX_IO_METHOD_BIT_FGTRIGD_NEG_CH2 = 0x29000000,
   PHX_IO_METHOD_BIT_FGTRIGD_POS_CH3 = 0x2A000000,
   PHX_IO_METHOD_BIT_FGTRIGD_NEG_CH3 = 0x2B000000,
   PHX_IO_METHOD_BIT_FGTRIGD_POS_CH4 = 0x2C000000,
   PHX_IO_METHOD_BIT_FGTRIGD_NEG_CH4 = 0x2D000000,

#if !defined _PHX_NO_BACKWARDS_COMPATIBILITY_
   /* Retained for backwards compatibility with previous software releases.
    * Do not use for new applications.
    */
   PHX_IO_METHOD_BIT_TIMER_POS       = 0x04000000,     /* Retained for backwards compatibility */
   PHX_IO_METHOD_BIT_HW_POS          = 0x04000000,     /* Retained for backwards compatibility */
   PHX_IO_METHOD_BIT_ACQTRIG_POS     = 0x05000000,     /* Retained for backwards compatibility */
   PHX_IO_METHOD_BIT_TIMER_NEG       = 0x08000000,     /* Retained for backwards compatibility */
   PHX_IO_METHOD_BIT_HW_NEG          = 0x08000000,     /* Retained for backwards compatibility */
   PHX_IO_METHOD_BIT_ACQTRIG_NEG     = 0x09000000      /* Retained for backwards compatibility */
#endif
} etPhxIoMethod;   

/*
PHX_BOARD_INFO()
*/
typedef enum {
   PHX_BOARD_INFO_PCI_3V            = 0x00000001,     /* 3V PCI Interface      */
   PHX_BOARD_INFO_PCI_5V            = 0x00000002,     /* 5V PCI Interface      */
   PHX_BOARD_INFO_PCI_33M           = 0x00000004,     /* 33MHz PCI Interface   */
   PHX_BOARD_INFO_PCI_66M           = 0x00000008,     /* 66MHz PCI Interface   */
   PHX_BOARD_INFO_PCI_32B           = 0x00000010,     /* 32bit PCI Interface   */
   PHX_BOARD_INFO_PCI_64B           = 0x00000020,     /* 64bit PCI Interface   */
   PHX_BOARD_INFO_LVDS              = 0x00000040,     /* LVDS Camera Interface */
   PHX_BOARD_INFO_CL                = 0x00000080,     /* Camera Link Interface */
   PHX_BOARD_INFO_CHAIN_MASTER      = 0x00000100,     /* Master Chaining */
   PHX_BOARD_INFO_CHAIN_SLAVE       = 0x00000200,     /* Slave Chaining  */
   PHX_BOARD_INFO_PCI_EXPRESS       = 0x00000400,     /* PCI Express interface  */
   PHX_BOARD_INFO_CL_BASE           = 0x00000800,     /* Camera Link Base only  */
   PHX_BOARD_INFO_CL_MEDIUM         = 0x00001000,     /* Camera Link Medium     */
   PHX_BOARD_INFO_CL_FULL           = 0x00002000,     /* Camera Link Full       */
   PHX_BOARD_INFO_BOARD_3V          = 0x00010000,     /* Board is 3V compatible */
   PHX_BOARD_INFO_BOARD_5V          = 0x00020000,     /* Board is 5V compatible */
   PHX_BOARD_INFO_BOARD_33M         = 0x00040000,     /* Board is 33MHz compatible */
   PHX_BOARD_INFO_BOARD_66M         = 0x00080000,     /* Board is 66MHz compatible */
   PHX_BOARD_INFO_BOARD_32B         = 0x00100000,     /* Board is 32bit compatible */
   PHX_BOARD_INFO_BOARD_64B         = 0x00200000      /* Board is 64bit compatible */
} etBoardInfo;

/*
 * PHX_PCIE_INFO()
 */
typedef enum {
   PHX_CXP_LINKS_MAX                = 4,
   PHX_EMASK_PCIE_INFO_LINK_GEN     = 0x00000003,
   PHX_PCIE_INFO_UNKNOWN            = 0x00000000,     /* Unknown               */
   PHX_PCIE_INFO_LINK_GEN1          = 0x00000001,     /* Link status Gen1      */
   PHX_PCIE_INFO_LINK_GEN2          = 0x00000002,     /* Link status Gen2      */
   PHX_PCIE_INFO_LINK_GEN3          = 0x00000003,     /* Link status Gen3      */
   PHX_EMASK_PCIE_INFO_LINK_X       = 0x0000001C,
   PHX_PCIE_INFO_LINK_X1            = 0x00000004,     /* Link status x1        */
   PHX_PCIE_INFO_LINK_X2            = 0x00000008,     /* Link status x2        */
   PHX_PCIE_INFO_LINK_X4            = 0x0000000C,     /* Link status x4        */
   PHX_PCIE_INFO_LINK_X8            = 0x00000010,     /* Link status x8        */
   PHX_PCIE_INFO_LINK_X12           = 0x00000014,     /* Link status x12       */
   PHX_PCIE_INFO_LINK_X16           = 0x00000018,     /* Link status x16       */
   PHX_PCIE_INFO_LINK_X32           = 0x0000001C,     /* Link status x32       */
   PHX_EMASK_PCIE_INFO_FG_GEN       = 0x00000300,
   PHX_PCIE_INFO_FG_GEN1            = 0x00000100,     /* Frame grabber Gen1    */
   PHX_PCIE_INFO_FG_GEN2            = 0x00000200,     /* Frame grabber Gen2    */
   PHX_PCIE_INFO_FG_GEN3            = 0x00000300,     /* Frame grabber Gen3    */
   PHX_EMASK_PCIE_INFO_FG_X         = 0x00001C00,
   PHX_PCIE_INFO_FG_X1              = 0x00000400,     /* Frame grabber x1      */
   PHX_PCIE_INFO_FG_X2              = 0x00000800,     /* Frame grabber x2      */
   PHX_PCIE_INFO_FG_X4              = 0x00000C00,     /* Frame grabber x4      */
   PHX_PCIE_INFO_FG_X8              = 0x00001000,     /* Frame grabber x8      */
   PHX_PCIE_INFO_FG_X12             = 0x00001400,     /* Frame grabber x12     */
   PHX_PCIE_INFO_FG_X16             = 0x00001800,     /* Frame grabber x16     */
   PHX_PCIE_INFO_FG_X32             = 0x00001C00,     /* Frame grabber x32     */
   PHX_EMASK_PCIE_INFO_SLOT_GEN     = 0x00030000,
   PHX_PCIE_INFO_SLOT_GEN1          = 0x00010000,     /* Slot Gen1      */
   PHX_PCIE_INFO_SLOT_GEN2          = 0x00020000,     /* Slot Gen2      */
   PHX_PCIE_INFO_SLOT_GEN3          = 0x00030000,     /* Slot Gen3      */
   PHX_EMASK_PCIE_INFO_SLOT_X       = 0x001C0000,
   PHX_PCIE_INFO_SLOT_X1            = 0x00040000,     /* Slot x1        */
   PHX_PCIE_INFO_SLOT_X2            = 0x00080000,     /* Slot x2        */
   PHX_PCIE_INFO_SLOT_X4            = 0x000C0000,     /* Slot x4        */
   PHX_PCIE_INFO_SLOT_X8            = 0x00100000,     /* Slot x8        */
   PHX_PCIE_INFO_SLOT_X12           = 0x00140000,     /* Slot x12       */
   PHX_PCIE_INFO_SLOT_X16           = 0x00180000,     /* Slot x16       */
   PHX_PCIE_INFO_SLOT_X32           = 0x001C0000,     /* Slot x32       */
} etPcieInfo;


/*
 * PHX_CXP_INFO()
 */
typedef enum {
   PHX_CXP_CAMERA_DISCOVERED        = 0x00000001,     /*                       */
   PHX_CXP_CAMERA_IS_POCXP          = 0x00000002,     /*                       */
   PHX_CXP_POCXP_UNAVAILABLE        = 0x00000004,     /*                       */
   PHX_CXP_POCXP_TRIPPED            = 0x00000008,     /*                       */
   PHX_CXP_LINK1_USED               = 0x00000100,     /*                       */
   PHX_CXP_LINK2_USED               = 0x00000200,     /*                       */
   PHX_CXP_LINK3_USED               = 0x00000400,     /*                       */
   PHX_CXP_LINK4_USED               = 0x00000800,     /*                       */
   PHX_CXP_LINK1_MASTER             = 0x00010000,     /*                       */
   PHX_CXP_LINK2_MASTER             = 0x00020000,     /*                       */
   PHX_CXP_LINK3_MASTER             = 0x00040000,     /*                       */
   PHX_CXP_LINK4_MASTER             = 0x00080000,     /*                       */
} etCxpInfo;


/*
 * PHX_CL_INFO()
 */
typedef enum {
   PHX_CL_CAMERA_CONNECTED          = 0x00000001,     /*                       */
   PHX_CL_CAMERA_IS_POCL            = 0x00000002,     /*                       */
   PHX_CL_POCL_UNAVAILABLE          = 0x00000004,     /*                       */
   PHX_CL_POCL_TRIPPED              = 0x00000008,     /*                       */
} etClInfo;


/* Acquire Function Definitions
 * ============================
 * These define the operations that can be performed by the 
 * PHX_Acquire() function.
 * They are implemented as enums to make them visible within the debugger.
 */
typedef enum {
   _PHX_ENUM( PHX_ACQUIRE, PHX_START,                           1 ),
   _PHX_ENUM( PHX_ACQUIRE, PHX_CHECK_AND_WAIT,                  2 ),
   _PHX_ENUM( PHX_ACQUIRE, PHX_CHECK_AND_RETURN,                3 ),
   _PHX_ENUM( PHX_ACQUIRE, PHX_STOP,                            4 ),
   _PHX_ENUM( PHX_ACQUIRE, PHX_ABORT,                           5 ),
   _PHX_ENUM( PHX_ACQUIRE, PHX_BUFFER_GET,                      6 ),
   _PHX_ENUM( PHX_ACQUIRE, PHX_BUFFER_RELEASE,                  7 ),
   _PHX_ENUM( PHX_ACQUIRE, PHX_BUFFER_ABORT,                    8 ),
   _PHX_ENUM( PHX_ACQUIRE, PHX_EVENT_HANDLER,                   9 ),
   _PHX_ENUM( PHX_ACQUIRE, PHX_START_IMMEDIATELY,              10 ),
   _PHX_ENUM( PHX_ACQUIRE, PHX_SWTRIG,                         11 ),
   _PHX_ENUM( PHX_ACQUIRE, PHX_UNLOCK,                         12 ),
   _PHX_ENUM( PHX_ACQUIRE, PHX_AUTO_WHITE_BALANCE,             13 ),
   _PHX_ENUM( PHX_ACQUIRE, PHX_AUTO_RESTART,                   14 ),
   _PHX_ENUM( PHX_ACQUIRE, PHX_BUFFER_GET_MERGED,              15 ),
   _PHX_ENUM( PHX_ACQUIRE, PHX_USER_LOCK,                      16 ),
   _PHX_ENUM( PHX_ACQUIRE, PHX_USER_UNLOCK,                    17 ),
   _PHX_ENUM( PHX_ACQUIRE, PHX_BUFFER_OBJECT_GET,              18 ),
   _PHX_ENUM( PHX_ACQUIRE, PHX_BUFFER_MAKE_READY,              19 ),

   _PHX_ENUM( PHX_ACQUIRE, PHX_CXP_LINKTEST_UPLINK_OK_RST,    128 ),
   _PHX_ENUM( PHX_ACQUIRE, PHX_CXP_LINKTEST_UPLINK_ERR_RST,   129 ),
   _PHX_ENUM( PHX_ACQUIRE, PHX_CXP_LINKTEST_DOWNLINK_OK_RST,  130 ),
   _PHX_ENUM( PHX_ACQUIRE, PHX_CXP_LINKTEST_DOWNLINK_ERR_RST, 131 ),
   _PHX_ENUM( PHX_ACQUIRE, PHX_CXP_ERRCOUNT_LINK_LOSS_RST,    132 ),
   _PHX_ENUM( PHX_ACQUIRE, PHX_CXP_ERRCOUNT_8B10B_RST,        133 ),
   _PHX_ENUM( PHX_ACQUIRE, PHX_CXP_ERRCOUNT_CRC_STREAM_RST,   134 ),
   _PHX_ENUM( PHX_ACQUIRE, PHX_CXP_ERRCOUNT_CRC_CTRL_RST,     135 ),
   _PHX_ENUM( PHX_ACQUIRE, PHX_CXP_ERRCOUNT_CRC_EVENT_RST,    136 ),
   _PHX_ENUM( PHX_ACQUIRE, PHX_CXP_ERRCOUNT_DUP_FIXED_RST,    137 ),
   _PHX_ENUM( PHX_ACQUIRE, PHX_CXP_ERRCOUNT_DUP_NFIXED_RST,   138 ),
   _PHX_ENUM( PHX_ACQUIRE, PHX_CXP_ERRCOUNT_FEC_FIXED_RST,    139 ),
   _PHX_ENUM( PHX_ACQUIRE, PHX_CXP_ERRCOUNT_FEC_NFIXED_RST,   140 ),

#if !defined _PHX_NO_BACKWARDS_COMPATIBILITY_
   /* Retained for backwards compatibility with previous software releases.
    * Do not use for new applications.
    */
   PHX_EXPOSE                        = PHX_SWTRIG
#endif
} etAcq;
 
 
/* This structure is used to specify the address and context of a block of
 * memory (used to specify user allocated image buffers). It can be used
 * either for virtual or physical addresses.
 */
typedef struct {
   void *pvAddress;
   void *pvContext;
} stImageBuff;

/* This structure is used to specify the address, size and context of a
 * block of memory (used to specify user locked image buffers).
 */
typedef struct {
   void *pvAddress;
   ui64  qwSizeBytes;
   void *pvContext;
} stUserBuff;

/* This structure is used to specify address and lngth of physical blocks of memory.
 */
typedef struct {
   tPhxPhysAddr   uiAddress;
   tPhxPhysLen    uiLength;
} stPhysBuff;

typedef struct {
   ui64  qwTime;
   ui64  qwEvent;
} stPhxTimeStamp;


typedef struct {
   ui32 uiHeaderIndication;
   ui32 uiStreamID;
   ui32 uiSrcTag;
   ui32 uiSizeX;
   ui32 uiSizeY;
   ui32 uiOffsetX;
   ui32 uiOffsetY;
   ui32 uiDWordCount;
   ui32 uiPixelFormat;
   ui32 uiTapGeometry;
   ui32 uiFlags;
} stPhxCxpHeader;


typedef enum {
   PHX_BUFFER_VIRTUAL_ADDR,
   PHX_BUFFER_CONTEXT,
   PHX_BUFFER_SEQUENCE_TAG,
   PHX_BUFFER_TIMESTAMP,
   PHX_BUFFER_NUM_TIMESTAMPS,
   PHX_BUFFER_TIMESTAMP_ADDR
} etBufferParam;

/*
 * NOTE
 * If you change the following values you ***MUST*** change the
 * appropriate number of initializers for the global array.
 */
/* The maximum allowable number of image buffers. */
#define PHX_MAX_BUFFS         0x00007FFF
#define PHX_BUFFS_MASK        0x00007FFF


#define PHX_MAX_UINT64 (ui64(-1))

/* Timeout Definitions
 * ===================
 */
enum eTimeouts {
   PHX_TIMEOUT_IMMEDIATE = 0,
   PHX_TIMEOUT_INFINITE  = -1,
   PHX_TIMEOUT_PROTOCOL  = -2 /* 999 to be confirmed */
};

/* Status Definitions
 * ==================
 */
enum eStat {
   PHX_OK = 0,
   PHX_ERROR_BAD_HANDLE,
   PHX_ERROR_BAD_PARAM,
   PHX_ERROR_BAD_PARAM_VALUE,
   PHX_ERROR_READ_ONLY_PARAM,
   PHX_ERROR_OPEN_FAILED,
   PHX_ERROR_INCOMPATIBLE,
   PHX_ERROR_HANDSHAKE,
   PHX_ERROR_INTERNAL_ERROR,
   PHX_ERROR_OVERFLOW,
   PHX_ERROR_NOT_IMPLEMENTED,
   PHX_ERROR_HW_PROBLEM,
   PHX_ERROR_NOT_SUPPORTED,
   PHX_ERROR_OUT_OF_RANGE,
   PHX_ERROR_MALLOC_FAILED,
   PHX_ERROR_SYSTEM_CALL_FAILED,
   PHX_ERROR_FILE_OPEN_FAILED,
   PHX_ERROR_FILE_CLOSE_FAILED,
   PHX_ERROR_FILE_INVALID,
   PHX_ERROR_BAD_MEMBER,
   PHX_ERROR_HW_NOT_CONFIGURED,
   PHX_ERROR_INVALID_FLASH_PROPERTIES,
   PHX_ERROR_ACQUISITION_STARTED,
   PHX_ERROR_INVALID_POINTER,
   PHX_ERROR_LIB_INCOMPATIBLE,
   PHX_ERROR_SLAVE_MODE,

   /* Phoenix display library errors */
   PHX_ERROR_DISPLAY_CREATE_FAILED,
   PHX_ERROR_DISPLAY_DESTROY_FAILED,
   PHX_ERROR_DDRAW_INIT_FAILED,
   PHX_ERROR_DISPLAY_BUFF_CREATE_FAILED,
   PHX_ERROR_DISPLAY_BUFF_DESTROY_FAILED,
   PHX_ERROR_DDRAW_OPERATION_FAILED,

   /* Registry errors */
   PHX_ERROR_WIN32_REGISTRY_ERROR,

   PHX_ERROR_PROTOCOL_FAILURE,
   PHX_ERROR_CXP_INVALID_ADDRESS,  /* Protocol errors reported by the camera */
   PHX_ERROR_CXP_INVALID_DATA,
   PHX_ERROR_CXP_INVALID_CONTROL,
   PHX_ERROR_CXP_WRITE_TO_READ_ONLY,
   PHX_ERROR_CXP_READ_FROM_WRITE_ONLY,
   PHX_ERROR_CXP_SIZE_TOO_LARGE,
   PHX_ERROR_CXP_INCORRECT_SIZE,
   PHX_ERROR_CXP_MALFORMED_PACKET,
   PHX_ERROR_CXP_FAILED_CRC,
   PHX_ERROR_CXP_COMMAND_MISMATCH, /* Protocol errors detected by the driver */
   PHX_ERROR_CXP_WAIT_ACK_DATA,
   PHX_ERROR_CXP_SIZE_MISMATCH,
   PHX_ERROR_CXP_STATUS_MISMATCH,
   PHX_ERROR_CXP_ACK_TIMEOUT,      /* Timeout errors */
   PHX_ERROR_CXP_WAIT_ACK_TIMEOUT,
   PHX_ERROR_CXP_USER_ACK_TIMEOUT,
   PHX_ERROR_CXP_INITIAL_ACK_TIMEOUT,
   PHX_ERROR_CXP_TENTATIVE_ACK_TIMEOUT,
   PHX_ERROR_CXP_RX_PACKET_INVALID, /* Rx Protocol errors detected by the driver */
   PHX_ERROR_CXP_RX_PACKET_CRC_ERROR,
   PHX_ERROR_CXP_INVALID_READ_ACK,
   PHX_ERROR_CXP_INVALID_WRITE_ACK,
   PHX_ERROR_CXP_INVALID_TENTATIVE_ACK,
   PHX_ERROR_CXP_INVALID_RESET_ACK,
   PHX_ERROR_CXP_INVALID_WAIT_ACK,

   PHX_WARNING_TIMEOUT        = 0x8000,
   PHX_WARNING_FLASH_RECONFIG,
   PHX_WARNING_ZBT_RECONFIG,
   PHX_WARNING_NOT_PHX_COM,
   PHX_WARNING_NO_PHX_BOARD_REGISTERED, 
   PHX_WARNING_TIMEOUT_EXTENDED,
   PHX_WARNING_FW_PARTIALLY_UPDATED,
   PHX_WARNING_FW_STATUS_VERIFY,
   PHX_WARNING_FW_DATA_VERIFY,
   PHX_WARNING_FW_S1_S2_MISMATCH,

#if !defined _PHX_NO_BACKWARDS_COMPATIBILITY_
   /* Retained for backwards compatibility with previous software releases.
    * Do not use for new applications.
    */
   PHX_ERROR_INVALID_FLASH_PROPERITES = PHX_ERROR_INVALID_FLASH_PROPERTIES, /* Backwards compatibility */
   PHX_WARNING                        = PHX_WARNING_TIMEOUT                 /* Backwards compatibility */
#endif
};
typedef enum eStat etStat;

/* This structure is used to specify a colour by its individual components.
 */
typedef struct {
   ui8 bRed;
   ui8 bGreen;
   ui8 bBlue;
} tColour;

/* This structure holds the details of a logical LUT
 */
struct tLutInfo{
   ui32 dwLut;
   ui32 dwColour;
   ui32 dwTap;
   ui32 dwBrightness;
   ui32 dwContrast;
   ui32 dwGamma;
   ui32 dwFloor;
   ui32 dwCeiling;
   ui16 *pwLutData;
   ui32 dwSize;
};

/* Default settings for the Luts */
enum eLutCtrl {
   PHX_LUT_DEFAULT_BRIGHTNESS    = 100,
   PHX_LUT_DEFAULT_CONTRAST      = 100,
   PHX_LUT_DEFAULT_GAMMA         = 100,
   PHX_LUT_MAX_LUTS              = 256,  /* Maximum number of LUTs across a line */
   PHX_LUT_MAX_COLS              =   3,  /* Maximum number of LUT components */
   PHX_LUT_MAX_TAPS              =   2   /* Maximum number of camera taps */
};


/* Default settings for the Luts */
enum eFbdLutCtrl {
   FBD_LUT_DEFAULT_BRIGHTNESS   = 100,
   FBD_LUT_DEFAULT_CONTRAST     = 100,
   FBD_LUT_BRIGHTNESS_MAX_VALUE = 200,
   FBD_LUT_BRIGHTNESS_MIN_VALUE =   0,
   FBD_LUT_CONTRAST_MAX_VALUE   = 200,
   FBD_LUT_CONTRAST_MIN_VALUE   =   0,
   FBD_LUT_DEFAULT_FLOOR        =   0,
   FBD_LUT_DEFAULT_CEILING      =  -1,
   FBD_LUT_MAX_TAPS             =   4
};


/* Prototype Definitions
 * =====================
 */

typedef void * tBufferHandle;

#if defined __cplusplus
extern "C" {
#endif
typedef void (PHX_C_CALL *etFctErrorHandler) (const char*, etStat, const char*);

etStat PHX_EXPORT_FN PHX_Action          ( tHandle, etAction, etActionParam, void* );
etStat PHX_EXPORT_FN PHX_Create          ( tHandle*, etFctErrorHandler );
etStat PHX_EXPORT_FN PHX_Open            ( tHandle );
etStat PHX_EXPORT_FN PHX_Close           ( tHandle* );
etStat PHX_EXPORT_FN PHX_Destroy         ( tHandle* );
etStat PHX_EXPORT_FN PHX_ParameterGet    ( tHandle, etParam, void* );
etStat PHX_EXPORT_FN PHX_ParameterSet    ( tHandle, etParam, void* );
etStat PHX_EXPORT_FN PHX_StreamRead      ( tHandle, etAcq,   void* );
etStat PHX_EXPORT_FN PHX_ControlRead     ( tHandle, etControlPort, void*, ui8*, ui32*, ui32 );
etStat PHX_EXPORT_FN PHX_ControlReset    ( tHandle, etControlPort, void*, ui32 );
etStat PHX_EXPORT_FN PHX_ControlWrite    ( tHandle, etControlPort, void*, ui8*, ui32*, ui32 );

etStat PHX_EXPORT_FN PHX_BufferParameterGet( tHandle, tBufferHandle, etBufferParam, void * );
etStat PHX_EXPORT_FN PHX_BufferParameterSet( tHandle, tBufferHandle, etBufferParam, void * );

void   PHX_EXPORT_FN PHX_ErrHandlerDefault ( const char*, etStat, const char* );
void   PHX_EXPORT_FN PHX_ErrCodeDecode     ( char*,       etStat );
void   PHX_EXPORT_FN PHX_DebugDefaultTraceHandler( ui8 * );

etStat PHX_EXPORT_FN PHX_MutexCreate(  tHandle hCamera, tPHX *phPhxMutex, char *szMutexName );
etStat PHX_EXPORT_FN PHX_MutexDestroy( tHandle hCamera, tPHX *phPhxMutex );
etStat PHX_EXPORT_FN PHX_MutexAcquire( tHandle hCamera, tPHX   hPhxMutex, ui32 dwTimeout );
etStat PHX_EXPORT_FN PHX_MutexRelease( tHandle hCamera, tPHX   hPhxMutex );
etStat PHX_EXPORT_FN PHX_SemaphoreCreate(  tHandle hCamera, tPHX *phPhxSem, ui32 dwInitialCount, ui32 dwMaximumCount );
etStat PHX_EXPORT_FN PHX_SemaphoreDestroy( tHandle hCamera, tPHX *phPhxSem );
etStat PHX_EXPORT_FN PHX_SemaphoreSignal(  tHandle hCamera, tPHX   hPhxSem, ui32 dwCount );
etStat PHX_EXPORT_FN PHX_SemaphoreWaitWithTimeout( tHandle hCamera, tPHX hPhxSem, ui32 dwWait );
etStat PHX_EXPORT_FN PHX_ComParameterGet( ui32, etComParam, void* );
etStat PHX_EXPORT_FN PHX_MemoryAlloc      ( tHandle, void **, ui32, ui32, ui32 );
void   PHX_EXPORT_FN PHX_MemoryFreeAndNull( tHandle, void ** );

/* Legacy Section
 * ===============================
 */
etStat PHX_EXPORT_FN PHX_Acquire         ( tHandle, etAcq,   void* );
etStat PHX_EXPORT_FN PHX_CameraConfigLoad( tHandle*, char*, etCamConfigLoad, etFctErrorHandler );
etStat PHX_EXPORT_FN PHX_CameraConfigSave( tHandle,  char*, etActionParam );
etStat PHX_EXPORT_FN PHX_CameraRelease   ( tHandle* );
etStat PHX_EXPORT_FN PHX_CommsReceive    ( tHandle, ui8*, ui32*, ui32 );
etStat PHX_EXPORT_FN PHX_CommsTransmit   ( tHandle, ui8*, ui32*, ui32 );

#if defined __cplusplus
};
#endif

/* Backwards Compatibility Section
 * ===============================
 */

#define etCamConfig            etCamConfigLoad  /* etCamConfig was used in PHX releases prior to version 2.2.10 */
#define PHX_MAX_IMAGE_BUFFERS  PHX_MAX_BUFFS    /* Prior to v3.04 */

#endif  /* _PHX_API_H_ */
