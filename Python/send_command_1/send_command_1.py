from serial import *

import numpy as np
import matplotlib.pyplot as plt
import time

import csv

ser = Serial("/dev/cu.wchusbserial1410",baudrate=250000,timeout=1)
line = ""
index = 0

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

def mouvementRelatifDeuxAxes(x, y, dir_x, dir_y):
    if(x<10) :
        x_str = "0000"+str(x)
    else:
        if(x<100):
            x_str = "000"+str(x)
        else:
            if(x<1000):
                x_str = "00"+str(x)
            else:
                if(x<10000):
                    x_str = "0"+str(x)
                else:
                    x_str = str(x)

    if(y<10) :
        y_str = "0000"+str(y)
    else:
        if(y<100):
            y_str = "000"+str(y)
        else:
            if(y<1000):
                y_str = "00"+str(y)
            else:
                if(y<10000):
                    y_str = "0"+str(y)
                else:
                    y_str = str(y)
                        
    if((dir_x == 0) and (dir_y == 0)):
        cmd = "MR2 X- "+x_str+" Y- "+y_str+"\r\n"
    
    if((dir_x == 0) and (dir_y == 1)):
        cmd = "MR2 X- "+x_str+" Y+ "+y_str+"\r\n"
    
    if((dir_x == 1) and (dir_y == 0)):
        cmd = "MR2 X+ "+x_str+" Y- "+y_str+"\r\n"
    
    if((dir_x == 1) and (dir_y == 1)):
        cmd = "MR2 X+ "+x_str+" Y+ "+y_str+"\r\n"
    
    
    print(cmd);
    cmd = cmd.encode("utf-8")
    ser.write(cmd)
    attendreDisponibilite()

def mouvementMAXrelatif(direction):
    if(direction == True):
        cmd = "MR1 X+ 30000\r\n"
    if(direction == False):
        cmd = "MR1 X- 30000\r\n"
    print(cmd);
    cmd = cmd.encode("utf-8")
    ser.write(cmd)
    attendreDisponibilite()

def newTypeMouvementAbsolu1(targetX, timeON, timeOFF):
    nbPasX = 0;
    direction = False;

    #On calcul le nombre de pas qu'il faut pour atteindre la position
    realValue1 = getPosition()
    print(realValue1)
    if(realValue1[0]>targetX):
        print(realValue1[0]-targetX)
        print("(1) nb pas = ")
        #nbPasX = (realValue1[1]-targetX)/0.29
        nbPasX = (realValue1[0]-targetX)/0.315
        print(nbPasX)
        direction = True;
        
    if(realValue1[0]<targetX):
        print(targetX-realValue1[0])
        print("(2) nb pas = ")
        #nbPasX = (targetX-realValue1[1])/0.29
        nbPasX = (targetX-realValue1[0])/0.315
        print(nbPasX)
        direction = False;

    #On décompose le nombre de pas pour qu'il soit valide
    while(nbPasX > 30000):
        mouvementMAXrelatif(direction)
        print(getPosition())
        nbPasX -= 30000
    
    mouvementRelatifDeuxAxes(int(nbPasX),0,direction,0)
    print(getPosition())

    #On ralenti la vitesse du déplacement
    setVitesse(timeON,timeOFF)
    mouvementAbsoluDeuxAxes(targetX,50000)
    
def getPosition():
    ser.write(b'GCP2 X Y\r\n')
    print('GCP2 X Y')

    line = ser.readline()
    line = line.decode("utf-8")
    #print("line=" + str(line))
    x = float(line[2:10])
    #print("value float =" + str(x))

    line = ser.readline()
    line = line.decode("utf-8")
    #print("line=" + str(line))
    y = float(line[2:10])
    #print("value float =" + str(y))
    attendreDisponibilite()

    return x, y

def setVitesse(timeON, timeOFF):
    if(timeON<1000) :
        time_on_str = "0"+str(timeON)
    else:
        time_on_str = str(timeON)
    cmd = "SCTA 1 ON "+str(timeON)+"\r\n"
    print(cmd);
    cmd = cmd.encode("utf-8")
    ser.write(cmd)
    attendreDisponibilite()
    
    if(timeON<1000) :
        time_on_str = "0"+str(timeON)
    else:
        time_on_str = str(timeON)
    cmd = "SCTA 1 OFF "+str(timeON)+"\r\n"
    print(cmd);
    cmd = cmd.encode("utf-8")
    ser.write(cmd)
    attendreDisponibilite()

def affichageGrapheTest1(theorical, real, x):
    x = np.append(x,(real[0]-theorical[0]))
    #print(x)
    return x

def affichageGrapheTest2(real, x):
    x = np.append(x,real[0])
    #print(x)
    return x   

def test1():
    x = np.empty(0)
    
    for i in range(100):
        theoricalValue = 20000, 50000
        mouvementAbsoluDeuxAxes(theoricalValue[0], theoricalValue[1])
        realValue = getPosition()
        x = affichageGrapheTest1(theoricalValue,realValue, x)
        plt.plot(x)
        plt.draw()
        plt.pause(0.005)

        theoricalValue = 30000, 40000
        mouvementAbsoluDeuxAxes(theoricalValue[0], theoricalValue[1])
        realValue = getPosition()
        x = affichageGrapheTest1(theoricalValue,realValue, x)
        plt.plot(x)
        plt.draw()
        plt.pause(0.005)

    plt.show()

def test2():
    x = np.empty(0)
    y = np.empty(0)
    dif = np.empty(0)

    dif_calcul = 0;

    fig, axs = plt.subplots(3)
    fig.suptitle('2500 step (micro-step 11) XY deviation')
    
    for i in range(100):
        mouvementRelatifDeuxAxes(2500,2500,1,1)
        realValue1 = getPosition()
        x = affichageGrapheTest2(realValue1,x)
        axs[0].plot(x)
        plt.draw()
        plt.pause(0.005)
        
        mouvementRelatifDeuxAxes(2500,2500,0,0)
        realValue2 = getPosition()
        y = affichageGrapheTest2(realValue2,y)
        axs[1].plot(y)
        plt.draw()
        plt.pause(0.005)

        dif_calcul = realValue2[0]-realValue1[0]
        dif = np.append(dif,dif_calcul)
        print(dif)
        axs[2].plot(dif)
        plt.draw()
        plt.pause(0.005)

        #print(x)
        
    plt.show()

def test3():
    x = np.empty(0)
    y = np.empty(0)
    dif = np.empty(0)

    dif_calcul = 0;

    fig, axs = plt.subplots(3)

    file = open("test3.csv", "w")
     
    try:

        for i in range(100):
            newTypeMouvementAbsolu1(10000, 500, 500)
            
            realValue1 = getPosition()
            file.write(str(realValue1[0])+";")
            
            x = affichageGrapheTest2(realValue1,x)
            axs[0].plot(x)
            plt.draw()
            plt.pause(0.005)
            
            newTypeMouvementAbsolu1(80000, 500, 500)
            
            realValue2 = getPosition()
            file.write(str(realValue2[0])+"\n")
    
            y = affichageGrapheTest2(realValue2,y)
            axs[1].plot(y)
            plt.draw()
            plt.pause(0.005)

            dif_calcul = realValue2[0]-realValue1[0]
            dif = np.append(dif,dif_calcul)
            print(dif)
            axs[2].plot(dif)
            plt.draw()
            plt.pause(0.005)
    finally:
        file.close()
    
    
attendreDisponibilite()
plt.ion()
#test1()
#test2()
test3()

