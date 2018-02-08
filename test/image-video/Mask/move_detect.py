import numpy as np
import cv2
import matplotlib.pylab as plt


a=[]
y=[]
#print(y[0])
plt.ion()

for i in range(0,500000):
	a.append(i)
	if i==0:
		y.append(0)
	y.append(y[i-1]+i)
	print(i)
	
	print(y[i-1]+i)
	plt.scatter(i,y[i])
	plt.pause(0.0001)



	
	
	
	


