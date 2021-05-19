"""
Travail à faire pour que le programme fonctionne :

Créer la commande CAL1 dans l'arduino

"""

from serial import *

#Définition des ports série :
serIHM = Serial("", baudrate=250000, timeout=1)
serPlatine = Serial("",baudrate=250000, timeout=1)

"""
La variable typeDeMouvementX permet de définir pour un mouvement relatif
0 : mouvement par défaut -> très fin
1 : fin
2 : moyen
3 : large 
"""
typeDeMouvementX = 1
typeDeMouvementY = 1

"""
La variable automatique est une variable globale qui permet de définir si
la platine est commandée de façon automatique ou manuelle
"""
automatique = True

"""
Les variable Point_Reference sont utilisés pour faire la calibration du
placement du wafer sur la platine XY
"""
Point_Reference_X = (0,0)
Point_Reference_Y = (0,0)
Point_Reference_O = (0,0)

"""
Liste des fonctions qui vont envoyées des commandes vers la platine
"""

#La fonction permet d'attendre la fin de l'exécution de la commande
def attendreDisponibilite():
    line=""
    while line != b'> \r\n':
        line = ser.readline()
    #print("line = " + str(line))

#La fonction permet de recevoir la position actuelle sur les deux axes
def getPosition():
    ser.write(b'GCP2 X Y\r\n')
    log("GCP2 X Y")

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

#La commande permet de connaitre le zéro et la valeur max en X et Y    
def calibration1():
    cmd = "CAL1\r\n"
    cmd = cmd.encode("utf-8")
    ser.write(cmd)
    log("CAL1")
    attendreDisponibilite()

#La commande en lien avec l'alignement du wafer manuel sans reconnaissance de forme
def calibration2():
    #Afficher sur ecran IHM : Trouver le 3ème repère qui est l'origine O
    displayIHM("Indiquer le repère ORIGIN");
    #Faire mesure de la position actuelle
    Point_Reference_O = getPosition()
    #Afficher sur ecran IHM : Trouver le 1er repère sur l'axe X
    displayIHM("Indiquer 1er repère (X)");
    #Faire mesure de la position actuelle
    Point_Reference_X = getPosition()
    #Afficher sur ecran IHM : Trouver le 2ème repère sur l'axe Y
    displayIHM("Indiquer 2ème repère (Y)");
    #Faire mesure de la position actuelle
    Point_Reference_Y = getPosition()

def log(cmd):
    #La fonction log permet de lister les actions qui ont été réalisées
    
    
def start():
    #Faire calibration :
        #Calibration avec capteur fin de course
        calibration1()
        #Calibration en fonction du wafer
        calibration2()

def main():
    start()
    #Ecoute IHM
    # Réfléchir à savoir comment l'on démarre un programme de scan 
    #Mise en parallèle
