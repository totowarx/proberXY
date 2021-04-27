from serial import *

import numpy as np
import matplotlib.pyplot as plt
import time

ser = Serial("/dev/cu.wchusbserial1410",baudrate=250000,timeout=1)
line = ""
index = 0
x = np.empty(0)

time.sleep(1)
"""
while line != b'> \r\n':
    line = ser.readline()
    #line = line.decode("utf-8")
    print("line=" + str(line))
"""


def attendreDisponibilite():
    line=""
    while line != b'> \r\n':
        line = ser.readline()
    print("line = " + str(line))

def mouvementAbsoluDeuxAxes(x, y):
    if(x<10) :
        x_str = "00000"+str(x)
    else:
        if(x<100):
            x_str = "0000"+str(x)
        else:
            if(x<1000):
                x_str = "000"+str(x)
            else:
                if(x<10000):
                    x_str = "00"+str(x)
                else:
                    if(x<100000):
                        x_str = "0"+str(x)
                    else:
                        x_str = str(x)

    if(y<10) :
        y_str = "00000"+str(y)
    else:
        if(y<100):
            y_str = "0000"+str(y)
        else:
            if(y<1000):
                y_str = "000"+str(y)
            else:
                if(y<10000):
                    y_str = "00"+str(y)
                else:
                    if(y<100000):
                        y_str = "0"+str(y)
                    else:
                        y_str = str(y)
                        
    cmd = "MA2 X"+x_str+" Y"+y_str+"\r\n"
    print(cmd);
    cmd = cmd.encode("utf-8")
    ser.write(cmd)
    attendreDisponibilite()

def getPosition():
    ser.write(b'GCP2 X Y\r\n')
    print('GCP2 X Y')

    line = ser.readline()
    line = line.decode("utf-8")
    #print("line=" + str(line))
    x = float(line[2:8])
    #print("value float =" + str(x))

    line = ser.readline()
    line = line.decode("utf-8")
    #print("line=" + str(line))
    y = float(line[2:8])
    #print("value float =" + str(y))
    attendreDisponibilite()

    return x, y

def affichageGraphe(theorical, real, x):
    x = np.append(x,(real[0]-theorical[0]))
    #print(x)
    return x

attendreDisponibilite()
plt.ion()

for i in range(100):
    theoricalValue = 20000, 50000
    mouvementAbsoluDeuxAxes(theoricalValue[0], theoricalValue[1])
    realValue = getPosition()
    x = affichageGraphe(theoricalValue,realValue, x)
    plt.plot(x)
    plt.draw()
    plt.pause(0.005)

    theoricalValue = 30000, 40000
    mouvementAbsoluDeuxAxes(theoricalValue[0], theoricalValue[1])
    realValue = getPosition()
    x = affichageGraphe(theoricalValue,realValue, x)
    plt.plot(x)
    plt.draw()
    plt.pause(0.005)


plt.show()

