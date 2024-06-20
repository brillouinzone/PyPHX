import ctypes
from ctypes import c_int, c_char_p, c_void_p
import os

localpath = os.getcwd()
print(localpath)
# Load the DLL
dll_path = os.path.abspath(os.path.join(localpath,'..','c','PHXGrabberLib','x64','Release', 'phx.dll'))

# Load the shared library
lib = ctypes.CDLL(dll_path)

# Define the argument and return types of the functions
lib.create_phxgrabber.restype = c_void_p
lib.create_phxgrabber.argtypes = []

lib.destroy_phxgrabber.restype = None
lib.destroy_phxgrabber.argtypes = [c_void_p]

lib.phxgrabber_open.restype = None
lib.phxgrabber_open.argtypes = [c_void_p, c_char_p]

lib.phxgrabber_close.restype = None
lib.phxgrabber_close.argtypes = [c_void_p]

lib.phxgrabber_is_opened.restype = c_int
lib.phxgrabber_is_opened.argtypes = [c_void_p]

lib.phxgrabber_last_error.restype = c_char_p
lib.phxgrabber_last_error.argtypes = [c_void_p]

lib.phxgrabber_set_event_counter_usage.restype = None
lib.phxgrabber_set_event_counter_usage.argtypes = [c_void_p, c_int]

# Create a wrapper class in Python
class PHXGrabberInterface:
    def __init__(self):
        self.obj = lib.create_phxgrabber()

    def __del__(self):
        lib.destroy_phxgrabber(self.obj)

    def open(self, camera_config_file):
        lib.phxgrabber_open(self.obj, camera_config_file.encode('utf-8'))

    def close(self):
        lib.phxgrabber_close(self.obj)

    def is_opened(self):
        return lib.phxgrabber_is_opened(self.obj) != 0

    def last_error(self):
        return lib.phxgrabber_last_error(self.obj).decode('utf-8')

    def set_event_counter_usage(self, use_event_counter):
        lib.phxgrabber_set_event_counter_usage(self.obj, use_event_counter)

# Example usage
if __name__ == '__main__':
    grabber = PHXGrabberInterface()
    print("initialized ok")
    grabber.open("Sens128016bit.pcf")
    if grabber.is_opened():
        print("Grabber is opened")
    else:
        print("Error:", grabber.last_error())
    grabber.close()