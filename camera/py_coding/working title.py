import RPi.GPIO as GPIO
from time import sleep
import os
#software for switching off button
GPIO.setmode(GPIO.BOARD)
GPIO.setwarnings(False)  # Turn off warnings output
GPIO.setup(38, GPIO.OUT) # Set pin #38 (GPIO20) to output
GPIO.setup(40, GPIO.IN)  # Set pin #40 (GPIO21) to input

while True:
    buttonIn = GPIO.input(40)
    if buttonIn == True:
        print('Button is pressed')
        #GPIO.cleanup()
        #os.system("sudo shutdown -h now")
        #break
    sleep(1)       