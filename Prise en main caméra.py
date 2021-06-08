
#Import
from picamera import PiCamera
from time import sleep

camera = PiCamera()

#Initialisation
camera = picamera.PiCamera()
camera.resolution = (1024, 768)

#Prendre des photos
camera.start_preview()
sleep(2)
camera.capture('/home/pi/Desktop/image0.jpg') 
camera.stop_preview()

#Prendre des phtots en rafales
camera.start_preview()
for i in range(10):
    sleep(10)
    camera.capture('/home/pi/Desktop/image%s.jpg' % i)
camera.stop_preview()

#Filmer durant un temps défini
camera.start_preview()
camera.start_recording('/home/pi/Desktop/video0.h264')
sleep(10)
camera.stop_recording()
camera.stop_preview()


