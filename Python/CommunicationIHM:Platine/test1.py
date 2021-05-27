from serial import *

import time

ser_Platine = Serial("/dev/cu.usbserial-1410",baudrate=250000,timeout=1)
ser_IHM = Serial("/dev/cu.usbmodem14201",baudrate=250000,timeout=1)

line = ""
cmd_Serial_IHM = ""
cmd_Serial_Platine = ""

"""
    L'interface IHM peut envoyer des commandes et recevoir des résultats
    La platine peut recevoir des commandes et revoyer des résultats
"""



def attendreDisponibilite():
    line=""
    while line != b'> \r\n':
        line = ser_Platine.readline()
    #print("line = " + str(line))

def envoieTest():
    while(1):
        line = ser_IHM.readline()
        #print(line)
        if line == b'TO:PLATINE\r\n':
            cmd_Serial_IHM = ser_IHM.readline()
            #print("cmd_Serial_IHM=" + str(cmd_Serial_IHM))

            if cmd_Serial_IHM == b'GCP1 X\r\n':
                #print("ok condition");
                #print(cmd_Serial_IHM)
                ser_Platine.write(cmd_Serial_IHM)

                valeur_position_X = ser_Platine.readline();
                attendreDisponibilite()

                ser_IHM.write(valeur_position_X);
                #line = ser_IHM.readline()
                #print("line1 = " +str(line))
                #line = ser_IHM.readline()
                #print("line2 = " +str(line))
                #line = ser_IHM.readline()
                #print("line3 = " +str(line))

                
        #time.sleep(2);

attendreDisponibilite()
time.sleep(5)
envoieTest()
