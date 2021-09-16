#include <ArduinoJson.h>

#include "DHT.h"

#define DHTPIN1 8
#define DHTPIN2 9//пин для датчика влажности и температуры

DHT dht1(DHTPIN1, DHT11); //инициализация датчика влажности/температуры
DHT dht2(DHTPIN2, DHT22);

String command; // освобождаем память в контроллере для переменой



float h , t;
int pins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
boolean sens[2];
boolean avl[18];
boolean flag[18];
unsigned long last_press[18];
boolean cmdRecognized;

void setup()
{
  Serial.begin(115200, SERIAL_8N1); // подключаем последовательный порт
  delay(150);

  //  pinMode(relay_pin1, INPUT);
  //  Serial.println(relay_pin1);
  //  Serial.println(digitalRead(relay_pin1));

  //  delay(150);
  for (byte i = 0; i < 18; i = i + 1) {
    avl[i] = false;
    flag[i] = 0;
    last_press[i] = millis() - 60000;
  }
  sens[0] = false;
  sens[1] = false;
  //    dht1.begin();//включение работы датчика влажности
  //    dht2.begin();
}

void loop()
{

  for (byte i = 0; i < 18; i++) {
    if (avl[i] and flag[i] and (millis() - last_press[i] >= 60000)) {
      digitalWrite(pins[i], 0);
      flag[i] = 0;
    }
  }
//  if (sens[0]) {
//    Serial.println(dht1.readTemperature());
//    Serial.println(dht1.readHumidity());
//  }

  if (Serial.available())
  {
    command = Serial.readString(); // переменная val равна полученной команде
    //Serial.println(command);
    if (command != "") {
      StaticJsonDocument<200> doc;
      cmdRecognized = false;
      //      if (command == "tsensor 1 get_temp\n")
      //      {
      //        cmdRecognized = true;
      //        t = dht1.readTemperature();//Измеряем температуру
      //        if (isnan(t)) {  // Проверка. Если не удается считать показания, выводится «Ошибка считывания», и программа завершает работу
      //          doc["code"] = "err";
      //          doc["message"] = "dht1 error";
      //          serializeJson(doc, Serial);
      //          Serial.println();
      //          //Serial.println("Error");
      //          return;
      //        }
      //        doc["code"] = "ok";
      //        JsonArray data = doc.createNestedArray("data");
      //        data.add("Temperature");
      //        data.add(t);
      //        data.add("*C");
      //        serializeJson(doc, Serial);
      //        Serial.println();
      //        //        Serial.print("Temperature: ");
      //        //        Serial.print(t);
      //        //        Serial.println(" *C "); //Вывод показателей на экран
      //      }
      //      if (command == "tsensor 2 get_temp\n")
      //      {
      //        cmdRecognized = true;
      //        t = dht2.readTemperature();//Измеряем температуру
      //        if (isnan(t)) {  // Проверка. Если не удается считать показания, выводится «Ошибка считывания», и программа завершает работу
      //          doc["code"] = "err";
      //          doc["message"] = "dht2 error";
      //          serializeJson(doc, Serial);
      //          Serial.println();
      //          //Serial.println("Error");
      //          return;
      //        }
      //        doc["code"] = "ok";
      //        JsonArray data = doc.createNestedArray("data");
      //        data.add("Temperature");
      //        data.add(t);
      //        data.add("*C");
      //        serializeJson(doc, Serial);
      //        Serial.println();
      //        //        Serial.print("Temperature: ");
      //        //        Serial.print(t);
      //        //        Serial.println(" *C "); //Вывод показателей на экран
      //      }
      //      if (command == "hsensor 1 get_humidity\n")
      //      {
      //        cmdRecognized = true;
      //        h = dht1.readHumidity();//Измеряем температуру
      //        if (isnan(h)) {  // Проверка. Если не удается считать показания, выводится «Ошибка считывания», и программа завершает работу
      //          doc["code"] = "err";
      //          doc["message"] = "dht1 error";
      //          serializeJson(doc, Serial);
      //          Serial.println();
      //          //Serial.println("Error");
      //          return;
      //        }
      //        doc["code"] = "ok";
      //        JsonArray data = doc.createNestedArray("data");
      //        data.add("Humidity");
      //        data.add(h);
      //        data.add("%");
      //        serializeJson(doc, Serial);
      //        Serial.println();
      //        //        Serial.print("Humidity: ");
      //        //        Serial.print(h);
      //        //        Serial.println(" %");
      //
      //      }
      //
      //      if (command == "hsensor 2 get_humidity\n")
      //      {
      //        cmdRecognized = true;
      //        h = dht2.readHumidity();//Измеряем температуру
      //        if (isnan(h)) {  // Проверка. Если не удается считать показания, выводится «Ошибка считывания», и программа завершает работу
      //          doc["code"] = "err";
      //          doc["message"] = "dht2 error";
      //          serializeJson(doc, Serial);
      //          Serial.println();
      //          //          Serial.println("Error");
      //          return;
      //        }
      //        doc["code"] = "ok";
      //        JsonArray data = doc.createNestedArray("data");
      //        data.add("Humidity");
      //        data.add(h);
      //        data.add("%");
      //        serializeJson(doc, Serial);
      //        Serial.println();
      //        //        Serial.print("Humidity: ");
      //        //        Serial.print(h);
      //        //        Serial.println(" %");
      //      }

      String dev = getValue(command, ' ', 0);
      String pin = getValue(command, ' ', 1);
      String idd = getValue(command, ' ', 2);
      String cmd = getValue(command, ' ', 3);

      if (dev == "tsensor" or dev == "hsensor" or dev =="sensor") {
        long iPin = pin.toInt() - 2;
        if (iPin > -1 and iPin < 19) {
          if (cmd == "set\n") {
            if (idd == "1") {
              dht1 = DHT(pins[iPin], DHT11);
              dht1.begin();
              sens[0] = true;
              doc["code"] = "ok";
              doc["data"] = "sensor " + idd + " has been seted!";
              serializeJson(doc, Serial);
              Serial.println();
              cmdRecognized = true;
              return;
            }
            if (idd == "2") {
              dht2 = DHT(pins[iPin], DHT22);
              dht2.begin();
              sens[1] = true;
              doc["code"] = "ok";
              doc["data"] = "sensor " + idd + " has been seted!";
              serializeJson(doc, Serial);
              Serial.println();
              cmdRecognized = true;
              return;
            }
          }
          if (cmd == "get_temp\n") {
            if (idd == "1" and sens[0]) {
              cmdRecognized = true;
              t = dht1.readTemperature();//Измеряем температуру
              if (isnan(t)) {  // Проверка. Если не удается считать показания, выводится «Ошибка считывания», и программа завершает работу
                doc["code"] = "err";
                doc["message"] = "dht1 error";
                serializeJson(doc, Serial);
                Serial.println();
                //Serial.println("Error");
                return;
              }
              doc["code"] = "ok";
              JsonArray data = doc.createNestedArray("data");
              data.add("Temperature");
              data.add(t);
              data.add("*C");
              serializeJson(doc, Serial);
              Serial.println();
            }
            if (idd == "2" and sens[1]) {
              cmdRecognized = true;
              t = dht2.readTemperature();//Измеряем температуру
              if (isnan(t)) {  // Проверка. Если не удается считать показания, выводится «Ошибка считывания», и программа завершает работу
                doc["code"] = "err";
                doc["message"] = "dht2 error";
                serializeJson(doc, Serial);
                Serial.println();
                //Serial.println("Error");
                return;
              }
              doc["code"] = "ok";
              JsonArray data = doc.createNestedArray("data");
              data.add("Temperature");
              data.add(t);
              data.add("*C");
              serializeJson(doc, Serial);
              Serial.println();
            }
          }
          if (cmd == "get_humidity\n") {
            if (idd == "1" and sens[0]) {
              cmdRecognized = true;
              h = dht1.readHumidity();//Измеряем температуру
              if (isnan(h)) {  // Проверка. Если не удается считать показания, выводится «Ошибка считывания», и программа завершает работу
                doc["code"] = "err";
                doc["message"] = "dht1 error";
                serializeJson(doc, Serial);
                Serial.println();
                //          Serial.println("Error");
                return;
              }
              doc["code"] = "ok";
              JsonArray data = doc.createNestedArray("data");
              data.add("Humidity");
              data.add(h);
              data.add("%");
              serializeJson(doc, Serial);
              Serial.println();
            }
            if (idd == "2" and sens[1]) {
              cmdRecognized = true;
              h = dht2.readHumidity();//Измеряем температуру
              if (isnan(h)) {  // Проверка. Если не удается считать показания, выводится «Ошибка считывания», и программа завершает работу
                doc["code"] = "err";
                doc["message"] = "dht2 error";
                serializeJson(doc, Serial);
                Serial.println();
                //          Serial.println("Error");
                return;
              }
              doc["code"] = "ok";
              JsonArray data = doc.createNestedArray("data");
              data.add("Humidity");
              data.add(h);
              data.add("%");
              serializeJson(doc, Serial);
              Serial.println();
            }
          }
        }
      }

      if (dev == "lock") {
        //        Serial.println(dev);
        //        Serial.println(pin);
        //        Serial.println(cmd);

        long iPin = pin.toInt() - 2;
        if (iPin > -1 and iPin < 19) {
          if (cmd == "set\n") {

            //            Serial.print("pin is ");
            //            Serial.println(iPin);


            pinMode(pins[iPin], OUTPUT);
            digitalWrite(pins[iPin], 0);
            avl[iPin] = true;
            doc["code"] = "ok";
            doc["data"] = "lock " + idd + " has been seted!";
            serializeJson(doc, Serial);
            Serial.println();
            //            Serial.print("lock ");
            //            Serial.print(iPin);
            //            Serial.println(" has been seted!");
            cmdRecognized = true;
            return;

          }

          if (cmd == "open\n") {
            cmdRecognized = true;
            if (!avl[iPin])
            {
              doc["code"] = "err";
              doc["message"] = "lock " + idd + " is disconnected!";
              serializeJson(doc, Serial);
              Serial.println();
              //              Serial.print("lock ");
              //              Serial.print(iPin);
              //              Serial.println(" is disconnected!");
              return;
            }
            if (!flag[iPin])
            {
              flag[iPin] = 1;
              last_press[iPin] = millis();
              digitalWrite(pins[iPin], 1);
              doc["code"] = "ok";
              doc["data"] = "lock " + idd + " has been opened";
              serializeJson(doc, Serial);
              Serial.println();
              //              Serial.print("lock ");
              //              Serial.print(iPin);
              //              Serial.println(" has been opened");
            } else {
              doc["code"] = "err";
              doc["message"] = "lock " + idd + " is already open!";
              doc["milliseconds"] = millis() - last_press[iPin];
              serializeJson(doc, Serial);
              Serial.println();
              //              Serial.print("lock ");
              //              Serial.print(iPin);
              //              Serial.println(" is already open!");
            }
            return;
          }

          if (cmd == "close\n") {
            cmdRecognized = true;
            if (!avl[iPin])
            {
              doc["code"] = "err";
              doc["message"] = "lock " + idd + " is disconnected!";
              serializeJson(doc, Serial);
              Serial.println();
              //              Serial.print("lock ");
              //              Serial.print(iPin);
              //              Serial.println(" is disconnected!");
              return;
            }
            flag[iPin] = 0;
            digitalWrite(pins[iPin], 0);
            doc["code"] = "ok";
            doc["data"] = "lock " + idd + " has been closed";
            serializeJson(doc, Serial);
            Serial.println();
            //            Serial.print("lock ");
            //            Serial.print(iPin);
            //            Serial.println(" has been closed");
            return;
          }

          if (cmd == "status\n") {
            cmdRecognized = true;

            if (!avl[iPin])
            {
              doc["code"] = "err";
              doc["message"] = "lock " + idd + " is disconnected!";
              serializeJson(doc, Serial);
              Serial.println();
              //              Serial.print("lock ");
              //              Serial.print(iPin);
              //              Serial.println(" is disconnected!");
              return;
            }

            if (flag[iPin]) {
              doc["code"] = "ok";
              doc["data"] = "lock " + idd + " is open" ;
              doc["milliseconds"] = millis() - last_press[iPin];
              serializeJson(doc, Serial);
              Serial.println();
              //              Serial.print("lock ");
              //              Serial.print(iPin);
              //              Serial.println(" is open");
            }
            else {
              doc["code"] = "ok";
              doc["data"] = "lock " + idd + " is closed";
              serializeJson(doc, Serial);
              Serial.println();
              //              Serial.print("lock ");
              //              Serial.print(iPin);
              //              Serial.println(" is closed");
            }
            return;
          }
        }
      }

      if (!cmdRecognized) {
        doc["code"] = "err";
        doc["message"] = "unknown command! " + dev + " " + idd + " " + cmd;
        serializeJson(doc, Serial);
        Serial.println();
        //        Serial.print("wrong command! ");
        //        Serial.println(command);
      }
    }
  }
  delay(50);
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
