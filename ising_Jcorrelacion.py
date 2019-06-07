import matplotlib.pyplot as plt
import pandas as pd
import os
path = os.getcwd()

df = pd.read_csv(path+'\\ising_Jcorrelacion.txt',header=0,sep='\t',skiprows=1)

plt.plot(df['k'],df['c_k'])
plt.xlabel('k')
plt.ylabel('C')
plt.show()
