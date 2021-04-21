import csv
from serial import *

with open('names.csv', 'w', newline='') as csvfile:
    fieldnames = ['start', 'stop']
    writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
    spamwriter = csv.writer(csvfile, delimiter=' ',
                            quotechar='|', quoting=csv.QUOTE_MINIMAL)

    writer.writeheader()
    
    start = 0
    stop = 1020

    cent = '100'

    with Serial(port="/dev/cu.wchusbserial1420", baudrate=115200, timeout=1, writeTimeout=1) as port_serie:
       if port_serie.isOpen():
           while True:
               #writer.writerow({'start':'100', 'stop':'200'})
               spamwriter.writerow([cent, cent])
               ligne = port_serie.readline().decode("utf-8")
               if(ligne!=""):
                   resultat = ligne.split(';')
                   #print(resultat)
                   print(ligne)


