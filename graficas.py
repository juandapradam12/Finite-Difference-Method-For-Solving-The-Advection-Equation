import numpy as np 
import matplotlib.pyplot as plt 

x = np.loadtxt('x_x.dat')
y_0 = np.loadtxt('y_0x.dat')
y_350 = np.loadtxt('y_350x.dat')
y_600 = np.loadtxt('y_600x.dat')

x_y = plt.figure()
y = [y_0, y_350, y_600]
leyenda = ['Nt=0', 'Nt=350', 'Nt=600']

for y_i, ley in zip(y, leyenda):
	plt.plot(x, y_i, label = ley)

plt.legend()
#plt.show()

plt.savefig('x_y.pdf')
