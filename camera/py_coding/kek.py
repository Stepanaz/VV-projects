import datetime as dt
from time import sleep
kek = open("/home/pi/Desktop/backup/py_coding/kek.txt", 'a')

kek.write(str(dt.datetime.now()) + " Hello world!\n")

kek.close()