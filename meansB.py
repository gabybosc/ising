import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import os
path = os.getcwd()

# sirve para graficar los resultados de means.txt de ising_means.c

datos = pd.read_csv(path+"/meansB.txt",sep="\t",skiprows=1,header=0)
#metadata = pd.read_csv(path+"/means.txt",nrows=1,sep=" ",header=None)

plt.figure(1)
plt.plot(1/datos["temp"],datos["mag"])
plt.xlabel(r'temperatura inversa $\beta$')
plt.ylabel(r'magnetizacion por sitio $|m|$')
plt.grid()
#plt.savefig('img/meanmagB.png',dpi=150)
plt.show()

plt.figure(2)
plt.plot(1/datos["temp"],datos["en"])
plt.xlabel(r'temperatura inversa $\beta$')
plt.ylabel(r'energia por sitio $u$')
plt.grid()
#plt.savefig('img/meanenB.png',dpi=150)
plt.show()
