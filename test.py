import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

if __name__ == '__main__':
	fig = plt.figure()
 
	axes3d = Axes3D(fig)
 
	x = np.linspace(-10,10,100)
	y = np.linspace(-10,10,100)
 
	X,Y = np.meshgrid(x,y)
	Z = np.sqrt(X**2+Y**2)
 
	axes3d.plot_surface(X,Y,Z)
 
	x = np.linspace(10,30)
	y = np.sin(x)
	z = np.cos(x)
	axes3d.plot(x,y,z,color = 'r')
	
	plt.show()