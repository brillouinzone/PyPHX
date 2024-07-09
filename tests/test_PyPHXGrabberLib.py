from PyPHX import PHXGrabberInterface
import time
import numpy as np
import matplotlib.pyplot as plt
# Example usage
if __name__ == '__main__':
    grabber = PHXGrabberInterface("Sens128016bit.pcf")
    print("initialized ok")
    if grabber.is_opened():
        print("Grabber is opened")
    else:
        print("Error:", grabber.last_error())
    print("Initial Error Status:", grabber.last_error())
    grabber.stream()

    # Run grabber.open() in a separate thread

    images = np.zeros((grabber.im_height, grabber.im_width, 2))
    tic = time.perf_counter()
    tdiff = 0
    for i in range(2):
        grabber.grab()
        # 20 milliseconds of overhead here
        images[:, :, i] = grabber.buffer_image
        tdiff = time.perf_counter() - tic
        #adjust to frame rate
        time.sleep(.01)


    print(f"access in {1000 * tdiff:0.2f}ms \n")
    grabber.show_image()  # shows the latest buffer image
    plt.figure()
    plt.imshow(images[:, :, 0] - images[:, :, 1])
    plt.title(f"imd_diff, t(im1)-t(im2)={tdiff:0.2f}ms ")
    plt.colorbar()
    plt.show()



    if grabber.is_opened():
        print("Grabber is opened")
    else:
        print("Error:", grabber.last_error())
    # Set stop_loop to true

    print("stopping stream")
    grabber.stop_stream()

    if grabber.is_opened():
        print("Grabber is opened")
    else:
        print("Error:", grabber.last_error())
    grabber.close()
