import ctypes
from ctypes import c_int, c_char_p, c_void_p, c_uint16, c_uint32, POINTER
import os
import threading
import numpy as np
import matplotlib.pyplot as plt
import time

# Load the DLL
# Determine the absolute path to the DLL file
module_path = os.path.dirname(__file__)
dll_path = os.path.abspath(os.path.join(module_path, '..', 'c', 'PHXGrabberLib', 'x64', 'Release', 'PHXGrabberLib.dll'))

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

lib.lock_buffer.argtypes = [ctypes.c_bool]

lib.access_buffer.argtypes = []
lib.access_buffer.restype = None


# Create a wrapper class in Python
class PHXGrabberInterface:
    def __init__(self, camfile):
        self.obj = lib.create_phxgrabber()
        self.camfile = camfile
        self.im_height = lib.get_buffer_height()
        self.im_width = lib.get_buffer_width()
        self.buffer_image = np.zeros((self.im_height, self.im_width))
        self.capture_thread = threading.Thread(target=self.config_and_stream)
    def __del__(self):
        lib.destroy_phxgrabber(self.obj)

    def config_and_stream(self):
        lib.phxgrabber_open(self.obj, self.camfile.encode('utf-8'))
        time.sleep(0.24)

    def open(self, camera_config_file):
        lib.phxgrabber_open(self.obj, camera_config_file.encode('utf-8'))
        # wait a moment for all to initialize
        # test the buffer access speed
        time.sleep(1)

    def phx_close(self):
        lib.phxgrabber_close(self.obj)

    def is_opened(self):
        return lib.phxgrabber_is_opened(self.obj) != 0

    def last_error(self):
        #todo:needs debugging
        return lib.phxgrabber_last_error(self.obj).decode('utf-8')

    def set_event_counter_usage(self, use_event_counter):
        lib.phxgrabber_set_event_counter_usage(self.obj, use_event_counter)

    def get_buffer_address(self):
        # macro function prototypes

        return lib.get_buffer_address()

    def get_buffer_width(self):
        return lib.get_buffer_width()

    def get_buffer_height(self):
        return lib.get_buffer_height()

    def stop_stream(self):
        lib.stop_looping()
        # Wait for the thread to finish
        self.capture_thread.join()

    def stream(self):
        self.capture_thread.start()
        time.sleep(0.1)

    def phx_grab(self):
        # print("locking image buffer")
        # lib.lock_buffer(True)

        # Wait for the loop thread to exit
        t0 = time.perf_counter()
        lib.access_buffer()

        # Get the buffer details
        buffer_address = lib.get_buffer_address()
        buffer_width = lib.get_buffer_width()
        buffer_height = lib.get_buffer_height()

        if buffer_address:
            # Convert the buffer to a NumPy array
            buffer_size = buffer_width * buffer_height
            buffer_array = np.ctypeslib.as_array(buffer_address, shape=(buffer_size,))
            self.buffer_image = buffer_array.reshape((buffer_height, buffer_width))

            #overhead for data transfer
            time.sleep(.01)
        else:
            print("Buffer address is NULL")



        # print("unlocking image buffer")
        # lib.lock_buffer(False)

    def phx_grab_N_images(self,N):
        images = np.zeros((self.im_height, self.im_width, N))
        for i in range(N):
            self.phx_grab()
            # 20 milliseconds of overhead here
            images[:, :, i] = self.buffer_image
            # adjust to frame rate
            time.sleep(.01)
        return images
    def show_image(self):
        plt.figure()
        plt.imshow(self.buffer_image)
        plt.title(f"latest img, npmax = {np.max(self.buffer_image)}")
        plt.show()

