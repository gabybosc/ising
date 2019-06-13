import matplotlib.pyplot as plt
import pandas as pd
import os
path = os.getcwd()
import numpy as np
#from matplotlib import cm

df = pd.read_csv(path+'\\ising_correlacion_Jcrit.txt',header=None,sep=' ',skiprows=2)

J = np.arange(0.45,0.6,0.05)

#m = cm.ScalarMappable(norm=None, cmap='gist_rainbow')
#colores = m.to_rgba(np.arange(0.1,1.4,0.1)/1.4)
#colores = m.to_rgba(J/max(J))

for i in range(df.shape[0]):
	plt.plot(df.iloc[i,:]),label='J={0:.2f}'.format(J[i]))#,c=colores[i])
plt.xlabel('Pasos por sitio ($k/L^2$)')
plt.ylabel('C')
plt.title('Correlacion para magnetizacion')
#plt.ylim([-1,1])
plt.legend()
plt.grid()

plt.show()

datos = np.loadtxt('magnetizacion_vs_B.txt', skiprows=2)
B = datos[:,0]
m = datos[:,1]
u = datos[:,2]

plt.plot(B,m/(-20), label='mag')
plt.plot(B,u/100, label='energia')
plt.legend()
