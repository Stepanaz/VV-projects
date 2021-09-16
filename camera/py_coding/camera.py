import picamera
from time import sleep
import os
import datetime as dt
import RPi.GPIO as GPIO
import sys

try:
    #Директория для записи файлов
    path = "/media/pi/USB_DISK/videos/" #По хорошему надо бы трай кач для проверки доступности флешки,
    os.chdir(path)
    print(os.getcwd())
    
    log_path = "/media/pi/USB_DISK/" + str(dt.datetime.now().date()) + " log.txt"
    backup_log = "/home/pi/Desktop/backup/backup_videos/" + str(dt.datetime.now().date()) + " backup_log.txt"
    
    log_file = open(log_path, 'a', encoding = "utf-8")
    backup_log = open(backup_log, 'a', encoding = "utf-8")
    
    
    #Настроим каналы GPIO под концевик
    GPIO.setmode(GPIO.BCM)
    GPIO.setwarnings(False)  # Turn off warnings output
    GPIO.setup(20, GPIO.OUT) # Set pin #38 (GPIO20) to output
    GPIO.setup(21, GPIO.IN)  # Set pin #40 (GPIO21) to input

    #Инициализируем датчик освещённости и подгрузим библу для него
    libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')
    if os.path.exists(libdir):
        sys.path.append(libdir)

    import logging
    from waveshare_TSL2591 import TSL2591

    logging.basicConfig(level=logging.INFO)

    sensor = TSL2591.TSL2591()
    # sensor.SET_InterruptThreshold(0xff00, 0x0010)

    #Настройка параметров видео
    camera = picamera.PiCamera()
    camera.resolution = (1280, 720)
    camera.framerate = 30

    lastAction = dt.datetime.now()
    SaveEvent = False
    startEvent =  dt.datetime.now()
    periodBetweenEvents = 30
    before_duration = 10
    #Логические функции активации датчиков____________________________________________________________

    def konc():
        buttonIn = GPIO.input(21)#сюда бы по хорошему трай кач
        if buttonIn == False: #Нас интересует ситуация, когда концевик разомкнут, поэтому false
            print("Концевик разомкнут")
            log_file.write(dt.datetime.now().strftime('%Y-%m-%d %H:%M:%S') + " Концевик разомкнут" + "\n")
            backup_log.write(dt.datetime.now().strftime('%Y-%m-%d %H:%M:%S') + " Концевик разомкнут" + "\n")
            return(True)
        else:
            print("Концевик замкнут")
            log_file.write(dt.datetime.now().strftime('%Y-%m-%d %H:%M:%S') + " Концевик замкнут" + "\n")
            backup_log.write(dt.datetime.now().strftime('%Y-%m-%d %H:%M:%S') + " Концевик замкнут" + "\n")
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
                print("Освещённость больше 2 Люкс")
                log_file.write(dt.datetime.now().strftime('%Y-%m-%d %H:%M:%S') + " Освещённость больше 2 Люкс" + "\n")
                backup_log.write(dt.datetime.now().strftime('%Y-%m-%d %H:%M:%S') + " Освещённость больше 2 Люкс" + "\n")
                return(True)
            else:
                print("Освещённость меньше 2 Люкс")
                log_file.write(dt.datetime.now().strftime('%Y-%m-%d %H:%M:%S') + " Освещённость меньше 2 Люкс" + "\n")
                backup_log.write(dt.datetime.now().strftime('%Y-%m-%d %H:%M:%S') + " Освещённость меньше 2 Люкс" + "\n")
                return(False)
            
    def CheckEvent():
        global StartEvent
        global SaveEvent
        if light_sensor() or konc():
            if not SaveEvent:
                StartEvent = dt.datetime.now()
            SaveEvent = True
        else:
            SaveEvent = False
        
    camera.annotate_text = dt.datetime.now().strftime('%Y-%m-%d %H:%M:%S')  # Выводим дату/время
    camera.annotate_text_size = 50                                          # Размер текста из диапазона 6...160, по-умолчанию 32
    camera.annotate_foreground = picamera.Color(y=0.2, u=0, v=0)            # Цвет текста - серый
    camera.annotate_background = picamera.Color('black')                    # На черном фоне

    stream = picamera.PiCameraCircularIO(camera, seconds=40)                # Создаем буфер размером в 25 секунд
    camera.start_recording(stream, format='h264')                           # Начинаем запись

    #camera.start_recording(os.getcwd() + "/video.h264")                     
    start = dt.datetime.now()                                               # Обнуляем счетчик
    fileNum = 1

    while 1:                                                            # Бесконечный цикл
        annotate_text = ""
        if SaveEvent:
            annotate_text = "After event - "                            # Маркируем кадры после наступления события
        else:
            annotate_text = "Before event - "                           # Маркируем кадры до наступления события
        
        camera.annotate_text = annotate_text + dt.datetime.now().strftime('%Y-%m-%d %H:%M:%S')  # Добавляем дату/время
        if not SaveEvent:
            CheckEvent()
        if SaveEvent and ((dt.datetime.now()-StartEvent).seconds > before_duration) and ((dt.datetime.now()-lastAction).seconds > periodBetweenEvents):
            print("Запись события завершена, приступаем к сохранению")
            log_file.write(dt.datetime.now().strftime('%Y-%m-%d %H:%M:%S') + " Запись события завершена, приступаем к сохранению"  + "\n")
            backup_log.write(dt.datetime.now().strftime('%Y-%m-%d %H:%M:%S') + " Запись события завершена, приступаем к сохранению"  + "\n")
            
            camera.stop_recording()                                     # Останавливаем запись
            fileName = '%03d' % fileNum + StartEvent.strftime('_%Y_%m_%d_%H_%M_%S')+'.h264' # Формируем имя файла
            print('Видеозапись события будет сохранена в файл '+ fileName)
            log_file.write(dt.datetime.now().strftime('%Y-%m-%d %H:%M:%S') + ' Видеозапись события будет сохранена в файл '+ fileName  + "\n")
            backup_log.write(dt.datetime.now().strftime('%Y-%m-%d %H:%M:%S') + ' Видеозапись события будет сохранена в файл '+ fileName  + "\n")
            stream.copy_to(path + fileName, seconds = periodBetweenEvents + 2)            # Сохраняем последние секунды из буфера в файл, 2 секунды в качестве костыля, чтобы ничего не скпиалось
            print('Событие сохранено')
            log_file.write(dt.datetime.now().strftime('%Y-%m-%d %H:%M:%S') + ' Событие сохранено' + "\n")
            backup_log.write(dt.datetime.now().strftime('%Y-%m-%d %H:%M:%S') + ' Событие сохранено' + "\n")
            stream.clear()                                              # Очищаем буфер
            
            camera.start_recording(stream, format='h264')               # Начинаем новую запись в буфер
            fileNum += 1                                                  # Наращиваем номер для имени файла
           
            lastAction = dt.datetime.now()                              # Дата/время последнего снимка
            CheckEvent()                                                # Проверяем сигнал с датчиков
        else:
            camera.wait_recording(0.2)                                  # Ждем 0.2 секунды
except KeyboardInterrupt:
    # ...
    print("Exit pressed Ctrl+C")                                        # Выход из программы по нажатию Ctrl+C
    log_file.write(dt.datetime.now().strftime('%Y-%m-%d %H:%M:%S') +  " Выход из программы по нажатию  Ctrl+C"  + "\n")
    backup_log.write(dt.datetime.now().strftime('%Y-%m-%d %H:%M:%S') +  " Выход из программы по нажатию  Ctrl+C"  + "\n")

except:
    # ...
    print("Other Exception")                                            # Прочие исключения
    log_file.write(dt.datetime.now().strftime('%Y-%m-%d %H:%M:%S') +  " --- Start Exception Data:" + "\n")
    backup_log.write(dt.datetime.now().strftime('%Y-%m-%d %H:%M:%S') +  " --- Start Exception Data:" + "\n")
    print("--- Start Exception Data:")
    traceback.print_exc(limit=2, file=sys.stdout)                       # Подробности исключения через traceback
    traceback.print_exc(limit=2, file=log_file)  
    traceback.print_exc(limit=2, file=backup_log)                       # Подробности исключения через traceback
    log_file.write(dt.datetime.now().strftime('%Y-%m-%d %H:%M:%S') +  " --- End Exception Data:" + "\n")
    backup_log.write(dt.datetime.now().strftime('%Y-%m-%d %H:%M:%S') +  " --- End Exception Data:" + "\n")
    print("--- End Exception Data:")
finally:    
    camera.stop_recording()                                                 # Завершаем запись
    camera.close()
    GPIO.cleanup()
    log_file.close()
    backup_log.close()
#camera.stop_preview()                                                   # Закрываем окно предпросмотра

