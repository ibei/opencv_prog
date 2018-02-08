import cv2
import numpy as np


import matplotlib.pylab as plt

cap = cv2.VideoCapture("detect_amout.avi")
frameCount=int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
fps=int(cap.get(cv2.CAP_PROP_FPS))
print ('zhen: ',frameCount)
print ('FPS: ',fps)



es = cv2.getStructuringElement(cv2.MORPH_RECT, (10,10))


background = None
x=[]
y=[]
plt.ion()

for index in range(0,frameCount):
  ret, frame_ori = cap.read()
  frame=frame_ori[300:400,200:400]
  area=0
  y_ori=0
  i=0

  if background is None:

		background = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
		background = cv2.GaussianBlur(background, (21, 21), 0)
		continue
  gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
  gray_frame = cv2.GaussianBlur(gray_frame, (11, 11), 0)

  diff = cv2.absdiff(background, gray_frame)

  diff = cv2.threshold(diff, 25, 255, cv2.THRESH_BINARY)[1]
  diff = cv2.dilate(diff, es, iterations = 2)

  image, cnts, hierarchy = cv2.findContours(diff, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
  
  for c in cnts:
    if cv2.contourArea(c) < 1000:
      continue    
    cv2.drawContours(frame,[c],-1,(0,255,0),2)
    area+=cv2.contourArea(c)
    print ('area is :   ',area)
    i+=1
  print('contours :  ',i)
  if i==0:
		y_ori=0
  else :
		y_ori=area
  x.append(index)
  y.append(y_ori)

	
  #print('y equals to :    ',y[index-1])
  plt.scatter(index,y[index-1])
  plt.pause(0.0001)

  cv2.imshow("contours", frame)
  cv2.moveWindow("contours",0,500)
  
  #cv2.imshow("dif", diff)
  #cv2.moveWindow("dif",0,0)
  if cv2.waitKey(1000 / fps) ==27 :
		break

cv2.destroyAllWindows()
cap.release()
		
	
	
