from __future__ import print_function
import xbox
# from xbox import Joystick
# change le nombre à virgule flottante au format de chaîne -x.xxx
def fmtFloat(n):
    return '{:6.3f}'.format(n)
def show(*args):
    for arg in args:
        print(arg, end="")
def showIf(boolean, ifTrue, ifFalse=" "):
    if boolean:
        show(ifTrue)
    else:
        show(ifFalse)
joy = xbox.Joystick()
# Boutons 
print("Appuyez sur Back pour quitter")
while not joy.Back():
    show("Connecté!!!")
    showIf(joy.connected(), "Y", "N")
    # Joystick droit
    show("  Joystick droit:", fmtFloat(joy.rightX()), "/", fmtFloat(joy.rightY()))
    # Joystick gauche
    show("  Joystick gauche:", fmtFloat(joy.leftX()), "/", fmtFloat(joy.leftY()))
    # Gachette droite
    show("  Gachette droite:", fmtFloat(joy.rightTrigger()))
    # Gauchette gauche 
    show("  Gauchette gauche:", fmtFloat(joy.leftTrigger()))
    # A/B/X/Y
    show("  Boutons:")
    showIf(joy.A(), "A")
    showIf(joy.B(), "B")
    showIf(joy.X(), "X")
    showIf(joy.Y(), "Y")
    # Dpad U/D/L/R
    show("  Croix directionnel:")
    showIf(joy.dpadUp(),    "U")
    showIf(joy.dpadDown(),  "D")
    showIf(joy.dpadLeft(),  "L")
    showIf(joy.dpadRight(), "R")
    # Bumper gauche
    show("  Bumper gauche:")
    showIf(joy.leftBumper(), "LB")
    # Bumper gauche
    show("  Bumper droit:")
    showIf(joy.rightBumper(), "RB")
    # Curseur en debut de ligne : affichage unique
    show(chr(13))
# Fin
joy.close()
