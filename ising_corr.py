import matplotlib.pyplot as plt
import pandas as pd
import os
path = os.getcwd()

df = pd.read_csv(path+'\\ising_correlacion.txt',header=None,sep=' ',skiprows=1)

plt.plot(df.iloc[0,:],label='mag')
plt.plot(df.iloc[1,:],label='en')
plt.xlabel('k')
plt.ylabel('C')
plt.legend()
plt.show()
