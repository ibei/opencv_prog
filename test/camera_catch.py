import cv2
import numpy
import matplotlib.pyplot as plot

cap=cv2.VideoCapture(-1)
while(1):
	
	ret,frame=cap.read()
	cv2.imshow("capture",frame)
	if cv2.waitKey(1)&0xFF==ord('q'):
		break
cap.release()
cv2.destroyAllWindows()
