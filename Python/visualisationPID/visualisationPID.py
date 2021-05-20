from serial import *

import numpy as np
import matplotlib.pyplot as plt
import time

ser = Serial("/dev/cu.wchusbserial1410",baudrate=250000,timeout=1)
line = ""
index = 0

time.sleep(1)

def releveDeplacement():

    consigne = np.empty(0)

    while line != b'> \r\n':
        line = ser.readline()

        x = float(line[0
        plt.plot(x)
        plt.draw()
        plt.pause(0.005)

    plt.show()
