import crontab

cron = crontab.CronTab(user='pi')
job = cron.new(command='python3 /home/pi/Desktop/backup/py_coding/camera_2.py')
job.every_reboot()

cron.write()
