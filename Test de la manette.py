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
print("Test manette: appuyez sur Back pour quitter")
while not joy.Back():
    show("Connecté!!!")
    showIf(joy.connected(), "Y", "N")
    # Right analog stick
    show("  Right X/Y:", fmtFloat(joy.rightX()), "/", fmtFloat(joy.rightY()))
    # Left analog stick
    #show("  Left X/Y:", fmtFloat(joy.leftX()), "/", fmtFloat(joy.leftY()))
    # Right trigger
    show("  RightTrg:", fmtFloat(joy.rightTrigger()))
    # Left trigger
    #show("  LeftTrg:", fmtFloat(joy.leftTrigger()))
    # A/B/X/Y buttons
    show("  Buttons:")
    showIf(joy.A(), "A")
    showIf(joy.B(), "B")
    showIf(joy.X(), "X")
    showIf(joy.Y(), "Y")
    # Dpad U/D/L/R
    show("  Dpad:")
    showIf(joy.dpadUp(),    "U")
    showIf(joy.dpadDown(),  "D")
    showIf(joy.dpadLeft(),  "L")
    showIf(joy.dpadRight(), "R")
    # Curseur en début de ligne
    show(chr(13))
# Fin
joy.close()
