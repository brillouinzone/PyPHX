from PyPHX import *
import os
import time

# localpath= os.path.dirname(os.path.realpath(__file__))
localpath = os.getcwd()
print(localpath)
# Load the DLL
dll_path = os.path.abspath(os.path.join(localpath,'..','c','PHX','x64','Release', 'phx.dll'))
phxdll=PyPHX(libpath=dll_path)
#variables
hCamera = tHandle()
sPhxLive = tPhxLive()

try:
    phxdll.live_run()

except Exception as e:
    print(e)

# time.sleep(1)
# phxdll.grab()
# phxdll.show_image()
# phxdll.Close()



