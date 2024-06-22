import ctypes
from ctypes import c_int, c_char_p, c_void_p, c_uint16, c_uint32, POINTER
import os
import threading
import numpy as np
import matplotlib.pyplot as plt
import time

localpath = os.getcwd()
print(localpath)
# Load the DLL
dll_path = os.path.abspath(os.path.join(localpath, '..', 'c', 'PHXGrabberLib', 'x64', 'Release', 'PHXGrabberLib.dll'))

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

lib.get_buffer_address.restype = ctypes.POINTER(ctypes.c_uint16)  # Change to 16-bit
# lib.get_buffer_address.argtypes = [c_void_p]

lib.get_buffer_width.restype = c_uint32
# lib.get_buffer_width.argtypes = [c_void_p]

lib.get_buffer_height.restype = c_uint32
# lib.get_buffer_height.argtypes = [c_void_p]

lib.stop_looping.argtypes = []
lib.stop_looping.restype = None

lib.access_buffer.argtypes = []
lib.access_buffer.restype = None
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

    def get_buffer_address(self):
        #macro function prototypes

        return lib.get_buffer_address()

    def get_buffer_width(self):
        return lib.get_buffer_width()

    def get_buffer_height(self):
        return lib.get_buffer_height()

    def stop_loop(self):
        lib.stop_looping()
    def grab(self):
        print("waiting for capture trigger")

        # Wait for the loop thread to exit
        t0 = time.perf_counter()
        lib.access_buffer()

        # Get the buffer details
        buffer_address = lib.get_buffer_address()
        print(buffer_address)
        buffer_width = lib.get_buffer_width()
        buffer_height = lib.get_buffer_height()

        if buffer_address:
            # Convert the buffer to a NumPy array
            buffer_size = buffer_width * buffer_height
            buffer_array = np.ctypeslib.as_array(buffer_address, shape=(buffer_size,))
            self.buffer_image = buffer_array.reshape((buffer_height, buffer_width))

        else:
            print("Buffer address is NULL")

    def show_image(self):
        plt.figure()
        plt.imshow(self.buffer_image)
        print(f"npmax = {np.max(self.buffer_image)}")
        plt.show()
# Example usage
if __name__ == '__main__':
    grabber = PHXGrabberInterface()
    print("initialized ok")

    # Run grabber.open() in a separate thread
    def open_grabber():
        grabber.open("Sens128016bit.pcf")

    open_thread = threading.Thread(target=open_grabber)
    open_thread.start()

    # Wait for some time before setting stop_loop to true
    import time
    time.sleep(5)  # Wait for 5 seconds before stopping the loop
    grabber.grab()
    grabber.show_image()
    # Set stop_loop to true
    grabber.stop_loop()

    # Wait for the thread to finish
    open_thread.join()

    if grabber.is_opened():
        print("Grabber is opened")
    else:
        print("Error:", grabber.last_error())

    # Access buffer information
    buffer_address = grabber.get_buffer_address()
    buffer_width = grabber.get_buffer_width()
    buffer_height = grabber.get_buffer_height()

    print("Buffer Address:", buffer_address)
    print("Buffer Width:", buffer_width)
    print("Buffer Height:", buffer_height)

    grabber.close()

