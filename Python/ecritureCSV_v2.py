from serial import *

fichier = open("data.csv", "a")

with Serial(port="/dev/cu.wchusbserial1420", baudrate=115200, timeout=1, writeTimeout=1) as port_serie:
    if port_serie.isOpen():
        i = 0
        while i<50:
            ligne = port_serie.readline().decode("utf-8")
            if(ligne!=""):
                fichier.write(ligne)
                print(ligne)
                i+=1
        fichier.close()
