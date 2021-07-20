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
            print("cmd_Serial_IHM=" + str(cmd_Serial_IHM))

            #########################################
            # Obtention de la position actuelle en X#
            #########################################
            
            if cmd_Serial_IHM == b'GCP1 X\r\n':
                #print("ok condition");
                #print(cmd_Serial_IHM)
                ser_Platine.write(cmd_Serial_IHM)

                valeur_position_X = ser_Platine.readline()
                attendreDisponibilite()

                ser_IHM.write(valeur_position_X)
                line = ser_IHM.readline()
                print("line1 = " +str(line))
                line = ser_IHM.readline()
                print("line2 = " +str(line))
                line = ser_IHM.readline()
                print("line3 = " +str(line))

            #########################################
            # Obtention de la position actuelle en Y#
            #########################################
            
            if cmd_Serial_IHM == b'GCP1 Y\r\n':
                #print("ok condition");
                #print(cmd_Serial_IHM)
                ser_Platine.write(cmd_Serial_IHM)

                valeur_position_Y = ser_Platine.readline()
                attendreDisponibilite()

                ser_IHM.write(valeur_position_Y)
                line = ser_IHM.readline()
                print("line1 = " +str(line))
                line = ser_IHM.readline()
                print("line2 = " +str(line))
                line = ser_IHM.readline()
                print("line3 = " +str(line))

            ##############################################
            # Obtention de la position actuelle en X et Y#
            ##############################################
            
            if cmd_Serial_IHM == b'GCP2 X Y\r\n':
                ser_Platine.write(cmd_Serial_IHM)
                valeur_position_X = ser_Platine.readline()
                valeur_position_Y = ser_Platine.readline()
                attendreDisponibilite()

                ser_IHM.write(valeur_position_X)
                ser_IHM.write(valeur_position_Y)
                
            #############################################
            # Mouvement de type relatif sur l'axe X ou Y#
            #############################################
            print(cmd_Serial_IHM[0:3].decode("utf-8"))
            if cmd_Serial_IHM[0:3].decode("utf-8") == "MR1":
                print("commande MR1")
                ser_Platine.write(cmd_Serial_IHM)
                line = ""

                while line != b'> \r\n':
                    line = ser_Platine.readline()
                    print("line="+str(line))
                    ser_IHM.write(line)
                    
                #ser_IHM.write(b'>')
                ser_IHM.write(b'> \r\n');

            ##########################################
            # Mouvement de type relatif sur deux axes#
            ##########################################
            if cmd_Serial_IHM[0:3] == b'MR2':
                ser_Platine.write(cmd_Serial_IHM)
                valeur_position = ser_Platine.readline()
                ser_IHM.write(valeur_position)

                line = ""
                while line != b'> \r\n':
                    line = ser_Platine.readline()
                    valeur_position = line
                    ser_IHM.write(valeur_position)
                    
            ##############################################
            # Mouvement de type linéaire sur l'axe X ou Y#
            ##############################################

            if cmd_Serial_IHM[0:3] == b'MA1':
                ser_Platine.write(cmd_Serial_IHM)
                valeur_position = ser_Platine.readline()
                ser_IHM.write(valeur_position)

            #####################################################
            # Fonction calibration avec capteur de fin de course#
            #####################################################

            if cmd_Serial_IHM == b'CAL1\r\n':
                ser_Platine.write(cmd_Serial_IHM);
                line = ""
                attendreDisponibilite()
                ser_IHM.write(b'> \r\n')
            
                
        #time.sleep(2);

attendreDisponibilite()
#time.sleep(2)
envoieTest()
