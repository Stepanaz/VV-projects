# VV projects
 Проекты для Вкусвилл

Zamki: здесь писались замки на ардуино для магазина самообслуживания. По команде в терминале ардуина даолжна понимать, что замок следует открыть, а по прошествии минуты закрыть.
Так же тут прикрутили поддержку датчика влажности и температуры. Эти данные по запросу ардуино отправляла в терминал. 

svetofor: Тут всё просто, это и есть банальный светофор для кассы самообслуживания, чтобы сигнализировать покупателям о том, что касса свободна
Алгоритм Тот же, слушаем терминал линукса, включаем соответствующий режим подстветки. В одной из версий при включении ардуина через динамик могла включать рик-ролл

Два проекта сверху написаны на C#. Понятно, что пришлось подзапариться с линуксом и  COM портами, чтобы заставить это работать

Camera: Кейс: водители часто крадут товар при разгрузке машины. Давайте воткнём в них камеры и датчики движения/освещённости/открытия дверей и будем писать процесс разгрузки. А давайте!
Для реализации берём Raspberry PI. Пишем код на питоне в этот раз. Видео забрасываем на флешку, пишем все события в лог. Один на USB И бэкапный на SD карту малинки. При непредвиденных обстоятельствах всё сбрасываем на СД карту малинки.
Ещё немного пришлось пострадать, чтобы заставить малину запускать скрипт при загрузке. Raspbian -- UNIX подобная система, так что пользуясь демоном crontab это можно сделать. Если дебажить код несколько вечеров подряд, ища ошибки в процессе исполнения программы с помощью файла логов.
