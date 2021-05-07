import numpy as np
import matplotlib.pyplot as plt

x = np.array([1, 3, 4, 6])

def add4(x):
    print(x);
    x = np.append(x,4);

add4(x) 
print(x)

plt.plot(x)
plt.show() # affiche la figure a l'ecran
