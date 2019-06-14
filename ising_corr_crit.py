import matplotlib.pyplot as plt
import pandas as pd
import os
import numpy as np
plt.ion()
#from matplotlib import cm

df = pd.read_csv('ising_correlacion_Jcrit.txt',header=None,sep=' ',skiprows=2)
datos = np.loadtxt('ising_correlacion_J=0_B=0.txt', skiprows=1)

plt.plot(datos[0,:])

J = np.arange(0.4,0.65,0.05)

datos = np.loadtxt('ising_correlacion_Js_06.txt', skiprows=2)


plt.figure()
for i in range(0,6):
	plt.plot(datos[i,:], label='j = 0.{}'.format(1-i))
plt.legend()
plt.ylim([-1,2])

#m = cm.ScalarMappable(norm=None, cmap='gist_rainbow')
#colores = m.to_rgba(np.arange(0.1,1.4,0.1)/1.4)
#colores = m.to_rgba(J/max(J))

for i in range(df.shape[0]-1):
	plt.plot(df.iloc[i,:],label='J={0:.2f}'.format(J[i]))#,c=colores[i])
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
