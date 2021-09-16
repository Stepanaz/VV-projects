import picamera
from time import sleep
import os
import datetime as dt
import RPi.GPIO as GPIO
import sys

def debug(line):
    kek = open("/home/pi/Desktop/backup/py_coding/kek2.txt", 'a')
    kek.write(str(dt.datetime.now()) + " " + line + "\n")
    kek.close()

def backup_log(line):
    backup_log_path = "/home/pi/Desktop/backup/backup_videos/" + str(dt.datetime.now().date()) + " backup_log.txt"
    backup_log_file = open(backup_log_path, 'a', encoding = "utf-8")
    backup_log_file.write(dt.datetime.now().strftime('%Y-%m-%d %H:%M:%S') + " " + line + '\n')
    backup_log_file.close()
    
def log(line):
    log_path = "/media/pi/USB_DISK/" + str(dt.datetime.now().date()) + " log.txt" #По хорошему надо бы трай кач для проверки доступности флешки,
    log_file = open(log_path, 'a', encoding = "utf-8")
    log_file.write(dt.datetime.now().strftime('%Y-%m-%d %H:%M:%S') + " " + line +'\n')
    log_file.close()
    

debug("before try")
#sleep(20)# ждём 20 секунд, чтобы дождаться полной загрузки системы
try:
    #Директория для записи файлов
    path = "/media/pi/USB_DISK/videos/" 
    
    debug("opening SD card log")
    backup_log("Starting the script")
    
    debug("going to sleep 60 seconds") #Ждём, чтобы флешка примонтировалась
    sleep(60)
    debug("opening flash drive log")
    log("Flash is OK!")
    
    debug("checking GPIO")
    #Настроим каналы GPIO под концевик
    GPIO.setmode(GPIO.BCM)
    GPIO.setwarnings(False)  # Turn off warnings output
    GPIO.setup(20, GPIO.OUT) # Set pin #38 (GPIO20) to output
    GPIO.setup(21, GPIO.IN)  # Set pin #40 (GPIO21) to input
    debug("GPIO OK!")
    log("GPIO OK!")
    #Инициализируем датчик освещённости и подгрузим библу для него
    libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')
    if os.path.exists(libdir):
        sys.path.append(libdir)

    import logging
    from waveshare_TSL2591 import TSL2591

    logging.basicConfig(level=logging.INFO)

    sensor = TSL2591.TSL2591()
    debug("TSL OK!")
    log("TSL OK!")
    # sensor.SET_InterruptThreshold(0xff00, 0x0010)

    #Настройка параметров видео
    camera = picamera.PiCamera()
    camera.resolution = (1280, 720)
    camera.framerate = 30
    debug("Camera OK!")
    log("Camera OK!")
    lastAction = dt.datetime.now()
    SaveEvent = False
    Dump = False
    EndEvent =  dt.datetime.now()
    sleep(0.1)
    StartEvent =  dt.datetime.now()
    periodBetweenEvents = 30 # Защита от слишком частых срабатываний, мб будет не нужна
    before_duration = 20 # Длительность записи в секундах до срабатывания датчиков
    buffer_size = 7200 #Размер буффера записи в секундах, больше этого времени файлы мы снимать не будем
    #Логические функции активации датчиков____________________________________________________________

    def konc():
        buttonIn = GPIO.input(21)#сюда бы по хорошему трай кач
        if buttonIn == False: #Нас интересует ситуация, когда концевик разомкнут, поэтому false
            return(True)
        else:
            return(False)

    def light_sensor():
            lux = sensor.Lux
            #Тут есть примеры того, что может наш прекрасный датчик. Самая годная единица для измерения освещённости из доступных -- люкс. Отсечку на освещённость сделаем в 2 люкса
            #print('Lux: %d'%lux) 
            #sensor.TSL2591_SET_LuxInterrupt(50, 200)
            #infrared = sensor.Read_Infrared
            #print('Infrared light: %d'%infrared)
            #visible = sensor.Read_Visible
            #print('Visible light: %d'%visible)
            #full_spectrum = sensor.Read_FullSpectrum
            #print('Full spectrum (IR + visible) light: %d\r\n'%full_spectrum)
            if lux > 2:
                #print("Освещённость больше 2 Люкс")
                return(True)
            else:
                return(False)
            
    konc_status = konc()
    sensor_status = light_sensor()
    last_report = dt.datetime.now()
    
    def status_report(sensor_status, konc_status):
        global last_report
        if sensor_status:
            log("Освещённость больше 2 Люкс")
            backup_log("Освещённость больше 2 Люкс")
        else:
            log("Освещённость меньше 2 Люкс")
            backup_log("Освещённость меньше 2 Люкс")
        if konc_status:
            log("Концевик замкнут")
            backup_log("Концевик замкнут")
        else:
            log("Концевик разомкнут")
            backup_log("Концевик разомкнут")
        last_report = dt.datetime.now()
        
    def CheckEvent():
        global StartEvent
        global SaveEvent
        global EndEvent
        global Dump
        global konc_status
        global sensor_status
        global last_report
        new_konc_status = konc()
        new_sensor_status = light_sensor()
        
        if (new_sensor_status != sensor_status) or (new_konc_status != konc_status) or ((dt.datetime.now() - last_report).seconds > 10):
            sensor_status = new_sensor_status
            konc_status = new_konc_status
            status_report(sensor_status, konc_status)
        
        if new_sensor_status or new_konc_status:
            if not SaveEvent:
                Dump = False
                StartEvent = dt.datetime.now() #Время начала события
            SaveEvent = True #флаг включённости датчика
        else:
            if SaveEvent:
                Dump = True
                EndEvent = dt.datetime.now() #время окончания события
            SaveEvent = False
        
    camera.annotate_text = dt.datetime.now().strftime('%Y-%m-%d %H:%M:%S')  # Выводим дату/время
    camera.annotate_text_size = 50                                          # Размер текста из диапазона 6...160, по-умолчанию 32
    camera.annotate_foreground = picamera.Color(y=0.2, u=0, v=0)            # Цвет текста - серый
    camera.annotate_background = picamera.Color('black')                    # На черном фоне

    stream = picamera.PiCameraCircularIO(camera, seconds = buffer_size + 100)            # Создаем буфер размером в 2 часа с небольшим запасом
    camera.start_recording(stream, format='h264')                           # Начинаем запись

    #camera.start_recording(os.getcwd() + "/video.h264")                     
    start = dt.datetime.now()                                               # Обнуляем счетчик
    fileNum = 1
    
    debug("before while true")
    log("Setup complete!")
    
    while 1:                                                            # Бесконечный цикл
        annotate_text = ""
        if SaveEvent:
            annotate_text = "After event - "                            # Маркируем кадры после наступления события
        else:
            annotate_text = "Before event - "                           # Маркируем кадры до наступления события
        
        camera.annotate_text = annotate_text + dt.datetime.now().strftime('%Y-%m-%d %H:%M:%S')  # Добавляем дату/время
        CheckEvent()
        
        if Dump or ((dt.datetime.now() - StartEvent).seconds > buffer_size - before_duration): 
            log("Запись события завершена, приступаем к сохранению")
            backup_log("Запись события завершена, приступаем к сохранению")
            
            camera.stop_recording()                                     # Останавливаем запись
            
            fileName = '%03d' % fileNum + StartEvent.strftime('_%Y_%m_%d_%H_%M_%S')+'.h264' # Формируем имя файла
            #print('Видеозапись события будет сохранена в файл '+ fileName)
            log('Видеозапись события будет сохранена в файл '+ fileName)
            backup_log('Видеозапись события будет сохранена в файл '+ fileName)
            
            if Dump:
                stream.copy_to(path + fileName, seconds = (EndEvent - StartEvent).seconds + before_duration)            # Сохраняем последние секунды из буфера в файл, 2 секунды в качестве костыля, чтобы ничего не скипалось
                Dump = False
            else:
                stream.copy_to(path + fileName, seconds = (dt.datetime.now() - StartEvent).seconds + before_duration)
                StartEvent = dt.datetime.now()
            
            log('Событие сохранено')
            backup_log('Событие сохранено')
            stream.clear()                                              # Очищаем буфер
            
            camera.start_recording(stream, format='h264')               # Начинаем новую запись в буфер
            fileNum += 1                                                  # Наращиваем номер для имени файла
           
        else:
            camera.wait_recording(0.2)                                  # Ждем 0.2 секунды
except KeyboardInterrupt:
    # ...
    #print("Exit pressed Ctrl+C")                                        # Выход из программы по нажатию Ctrl+C
    log("Выход из программы по нажатию  Ctrl+C")
    backup_log(" Выход из программы по нажатию  Ctrl+C")

except:
    # ...
    stream.copy_to(backup_log_path + fileName) #резервное копирование записи на сд карту
   # print("Other Exception")                                         # Прочие исключения
    backup_log("--- Start Exception Data:")
    backup_log_path = "/home/pi/Desktop/backup/backup_videos/" + str(dt.datetime.now().date()) + " backup_log.txt"
    backup_log_file = open(backup_log_path, 'a', encoding = "utf-8")
    traceback.print_exc(limit=2, file=backup_log)
    backup_log_file.close()
    backup_log("--- End Exception Data:")
    
    log("--- Start Exception Data:")
    log_path = "/media/pi/USB_DISK/" + str(dt.datetime.now().date()) + " log.txt" #По хорошему надо бы трай кач для проверки доступности флешки,
    log_file = open(log_path, 'a', encoding = "utf-8")
    traceback.print_exc(limit=2, file=log_file) 
    log_file.close()
    log("--- End Exception Data:")
    
    traceback.print_exc(limit=2, file=sys.stdout)                       # Подробности исключения через traceback  
finally:    
    camera.stop_recording()                                                 # Завершаем запись
    camera.close()
    GPIO.cleanup()