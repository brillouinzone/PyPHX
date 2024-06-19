from PyPHX import *
import os

# localpath= os.path.dirname(os.path.realpath(__file__))
localpath = os.getcwd()
print(localpath)
# Load the DLL
dll_path = os.path.abspath(os.path.join(localpath,'..','c','PHXGrabberLib','x64','Release', 'phx.dll'))
phxdll=PyPHX(libpath=dll_path)
#variables
hCamera = tHandle()
sPhxLive = tPhxLive()
#
# # Define an error handler function in Python
# def error_handler_function(error_msg, status, additional_msg):
#     print(f"Error: {error_msg.decode()}, Status: {status}, Additional: {additional_msg.decode()}")
# error_handler_c = PHX_C_CALL(error_handler_function)
#
#
# try:
#     phxdll.InitLive(sPhxLive)
# except Exception as e:
#     print("exception in Init")
#     print(e)
#
# try:
#     status, hCamera = phxdll.Create(error_handler_c)
#     print(f"create status = {status}")
#     print(f"hCamera value = {hCamera}")
#     if status == etStat.PHX_OK:
#         print(f"Create status: {status} (PHX_OK)")
#     else:
#         print(f"Create status: {status} (Error)")
# except Exception as e:
#     print("error")
#     print(e)
#
# try:
#     status = phxdll.Open(hCamera)
#     if status == etStat.PHX_OK:
#         print(f"Open status: {status} (PHX_OK)")
#     else:
#         print(f"Open status: {status} (Error)")
# except Exception as e:
#     print(e)
#
# try:
#     width = ctypes.c_uint32(1280)
#     status, value = phxdll.ParameterSet(hCamera,etParam.PHX_ROI_XLENGTH,width)
#     if status == etStat.PHX_OK:
#         print(f"Get roi xlen status: {status} (PHX_OK)")
#         print(f"roi_xlength = {value}")
#
#     else:
#         print(f"Get roi xlen status: {status} (Error)")
# except Exception as e:
#     print(e)
#
#
# try:
#     status, value = phxdll.ParameterGet(hCamera,etParam.PHX_ROI_XLENGTH)
#     if status == etStat.PHX_OK:
#         print(f"Get roi xlen status: {status} (PHX_OK)")
#         print(f"roi_xlength = {value}")
#
#     else:
#         print(f"Get roi xlen status: {status} (Error)")
# except Exception as e:
#     print(e)
#

try:
    phxdll.live_run()

except Exception as e:
    print(e)
