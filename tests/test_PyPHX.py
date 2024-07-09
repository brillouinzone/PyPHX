from PyPHX import *
import os
import time

phxdll=PyPHXLive()
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



