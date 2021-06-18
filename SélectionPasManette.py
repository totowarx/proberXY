import xbox

joy = xbox.Joystick()
Tab = [50, 100, 200, 300, 500, 1000, 5000]
i = 0

while not joy.Back():
    
    if joy.rightBumper():
        while joy.rightBumper():
            s = 1
            
        print(len(Tab))
        if i < len(Tab)-1:
            #print(i)
            #print ("pas = ", Tab[i])
            i += 1
            print(i)
            print ("pas = ", Tab[i])
            
    if joy.leftBumper():
        #print("LB")
        while joy.leftBumper():
            s = 1
        if i > 0:
            #print ("pas = ", Tab[i])
            i -= 1
            print ("pas = ", Tab[i])
            
