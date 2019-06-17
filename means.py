import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import os
path = os.getcwd()

# sirve para graficar los resultados de means.txt de ising_means.c

datos = pd.read_csv(path+"/means.txt",sep="\t",skiprows=1,header=0)
#metadata = pd.read_csv(path+"/means.txt",nrows=1,sep=" ",header=None)

plt.figure(1)
plt.plot(1/datos["beta"],datos["Mag"],label='mag')
plt.plot(1/datos["beta"],datos["En"],label='en')
plt.xlabel(r'temp $1/\beta$')
plt.title('Energia y magnetizacion por sitio')
plt.legend()
#plt.ylim([0,1])
plt.grid()
#plt.savefig('terma_mag_sitio.png',dpi=150)
plt.show()
