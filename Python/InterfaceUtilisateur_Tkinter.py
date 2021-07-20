
from tkinter import *

window = Tk()

#FRAME : CENTRE

##

#FRAME : DROITE

##LabelFrame : Déplacement Relatif

###FRAME : Partie HAUTE

####Bouton HAUT
####Bouton BAS
####Bouton GAUCHE
####Bouton DROITE

###FRAME : Partie BASSE
####Listbox : valeur déplacement

##LabelFrame : Déplacement Absolu

##LabelFrame : Position actuelle
###Label : Position x
###Label : Position y


#---------------Panneau centrale---------------#
panneauCentre = Frame(window)
panneauCentre.grid(row=1,column=1)

labelframe_Relatif = LabelFrame(panneauCentre,text = "Log")
labelframe_Relatif.grid(row=1,column=1)
#-----------------Panneau droit----------------#
panneauDroit = Frame(window)
panneauDroit.grid(row=1,column=2)

labelframe_Relatif = LabelFrame(panneauDroit,text = "Déplacement relatif")
labelframe_Relatif.grid(row=1,column=1)

lfr_h = Frame(labelframe_Relatif)
lfr_h.grid(row=1,column=1)

buttonLeft = Button(lfr_h, text="Left", height=1, width=8)
buttonLeft.grid(row=2,column=0)

buttonRight = Button(lfr_h, text="Right", height = 1, width = 8)
buttonRight.grid(row=2, column=3)

buttonUp = Button(lfr_h, text="Up", height = 1, width = 8)
buttonUp.grid(row=0, column=2)

buttonDown = Button(lfr_h, text="Down", height = 1, width = 8)
buttonDown.grid(row=3, column=2)

lfr_b = Frame(labelframe_Relatif)
lfr_b.grid(row=2, column=1)

liste = Listbox(lfr_b)
liste.insert(1, "10 step")
liste.insert(2, "50 step")
liste.insert(3, "100 step")
liste.insert(4, "500 step")
liste.insert(5, "1000 step")
liste.insert(6, "5000 step")
liste.insert(7, "10000 step")
liste.selection_set(3)

liste.pack()


labelframe_position_actuelle = LabelFrame(panneauDroit, text="Position absolu")
labelframe_position_actuelle.grid(row=2,column=1)

label_position_X = Label(labelframe_position_actuelle, text="undefined")
label_position_X.pack()

label_position_Y = Label(labelframe_position_actuelle, text="undefined")
label_position_Y.pack()


labelframe_Absolu = LabelFrame(panneauDroit, text="Déplacement absolu")
labelframe_Absolu.grid(row=3,column=1)


"""
labelframe_Relatif = LabelFrame(window, text = "Déplacement relatif")
labelframe_Relatif.pack(fill="both", expand = "yes")

buttonLeft = Button(labelframe_Relatif, text="Left", height = 1, width = 8)
buttonLeft.grid(row=2, column=0)

buttonRight = Button(labelframe_Relatif, text="Right", height = 1, width = 8)
buttonRight.grid(row=2, column=3)

buttonUp = Button(labelframe_Relatif, text="Up", height = 1, width = 8)
buttonUp.grid(row=0, column=2)

buttonDown = Button(labelframe_Relatif, text="Down", height = 1, width = 8)
buttonDown.grid(row=3, column=2)

labelframe_Position_Actuelle = LabelFrame(window, text = "Position actuelle")
labelframe_Position_Actuelle.pack(fill = "both", expand = "yes")


liste = Listbox(window)
liste.insert(1, "10 step")
liste.insert(2, "50 step")
liste.insert(3, "100 step")
liste.insert(4, "500 step")
liste.insert(5, "1000 step")
liste.insert(6, "5000 step")
liste.insert(7, "10000 step")

liste.pack()

label_position_X = Label(labelframe_Position_Actuelle, text="undefined")
label_position_X.pack()
"""

"""
myFrame = Frame(window,text="Frame1")
myFrame.grid(row=1,column=1)
MyLabel = Label(myFrame,text="Label inside Frame1")
MyLabel.pack()
"""

window.title("PROBER XY Vmicro")
#window.geometry("400x300")
window.mainloop()

