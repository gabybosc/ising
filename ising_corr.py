import matplotlib.pyplot as plt
import pandas as pd
import os
path = os.getcwd()
import numpy as np
from matplotlib import cm

df = pd.read_csv(path+'\\ising_correlacion_Js.txt',header=None,sep=' ',skiprows=2)

J = np.arange(0.1,0.35,0.1)

#m = cm.ScalarMappable(norm=None, cmap='gist_rainbow')
#colores = m.to_rgba(np.arange(0.1,1.4,0.1)/1.4)
#colores = m.to_rgba(J/max(J))

for i in range(len(J)):
	plt.plot(df.iloc[i,:],label='J={0:.1f}'.format(J[i]))#,c=colores[i])
plt.xlabel('Pasos por sitio ($k/L^2$)')
plt.ylabel('C')
plt.title('Correlacion para magnetizacion')
#plt.ylim([-1,1])
plt.legend()
plt.grid()

plt.show()
