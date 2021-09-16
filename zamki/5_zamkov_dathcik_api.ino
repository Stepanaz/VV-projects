
#include "DHT.h"
#define relay_pin1 2 // пины мосфетов/реле  
#define relay_pin2 3
#define relay_pin3 4
#define relay_pin4 5
#define relay_pin5 6

#define DHTPIN1 8
#define DHTPIN2 9//пин для датчика влажности и температуры
String command; // освобождаем память в контроллере для переменой

DHT dht1(DHTPIN1, DHT11); //инициализация датчика влажности/температуры
DHT dht2(DHTPIN2, DHT22);
boolean flag1 = 0;           // флажок режима
boolean flag2 = 0;
boolean flag3 = 0;
boolean flag4 = 0;
boolean flag5 = 0;

float h , t;
boolean con1, con2, con3, con4, con5;
unsigned long last_press1;
unsigned long last_press2;
unsigned long last_press3;
unsigned long last_press4;
unsigned long last_press5;
void setup()
{
  Serial.begin(115200); // подключаем последовательный порт
  pinMode(relay_pin1, OUTPUT);          // пин реле как выход
  pinMode(relay_pin2, OUTPUT);
  pinMode(relay_pin3, OUTPUT);
  pinMode(relay_pin4, OUTPUT);
  pinMode(relay_pin5, OUTPUT);
  last_press1 = millis() - 60000;
  last_press2 = millis() - 60000;
  last_press3 = millis() - 60000;
  last_press4 = millis() - 60000;
  last_press5 = millis() - 60000;
  dht1.begin();//включение работы датчика влажности
  dht2.begin();

  //реализуем проверку подключённых замков
  digitalWrite(relay_pin1, 1);
  delay(1000);
  int value1 = analogRead(A5);
  Serial.println(value1);
  if (value1 > 300) con1 = true;
  else con1 = false;
  digitalWrite(relay_pin1, 0);

  digitalWrite(relay_pin2, 1);
  delay(1000);
  int value2 = analogRead(A4);
  Serial.println(value2);
  if (value2 > 300) con2 = true;
  else con2 = false;
  digitalWrite(relay_pin2, 0);

  digitalWrite(relay_pin3, 1);
  delay(1000);
  int value3 = analogRead(A3); 
  Serial.println(value3);
  if (value3 > 300) con3 = true;
  else con3 = false;
  digitalWrite(relay_pin3, 0);

  digitalWrite(relay_pin4, 1);
  delay(1000);
  int value4 = analogRead(A2);
  Serial.println(value4);
  if (value4 > 300) con4 = true;
  else con4 = false;
  digitalWrite(relay_pin4, 0);

  digitalWrite(relay_pin5, 1);
  delay(1000);
  int value5 = analogRead(A1);
  Serial.println(value5);
  if (value5 > 300) con5 = true;
  else con5 = false;
  digitalWrite(relay_pin5, 0);

}

void loop()
{
  if (millis() - last_press1 >= 60000)
  {
    digitalWrite(relay_pin1, 0);
    flag1 = 0;
  }
  if (millis() - last_press2 >= 60000)
  {
    digitalWrite(relay_pin2, 0);
    flag2 = 0;
  }
  if (millis() - last_press3 >= 60000)
  {
    digitalWrite(relay_pin3, 0);
    flag3 = 0;
  }
  if (millis() - last_press4 >= 60000)
  {
    digitalWrite(relay_pin4, 0);
    flag4 = 0;
  }
  if (millis() - last_press5 >= 60000)
  {
    digitalWrite(relay_pin5, 0);
    flag5 = 0;
  }
  // проверяем, поступают ли какие-то команды
  if (Serial.available())
  {
    command = Serial.readString(); // переменная val равна полученной команде
    if (command == "lock 1 open")
    {
      if (!flag1 and con1)
      {
        flag1 = !flag1;
        last_press1 = millis();
        digitalWrite(relay_pin1, 1);
        Serial.println("lock 1 has been opened");
      }

    }
    if (command == "lock 2 open")
    {
      if (!flag2 and con2)
      {
        flag2 = !flag2;
        last_press2 = millis();
        digitalWrite(relay_pin2, 1);
        Serial.println("lock 2 has been opened");
      }
    }
    if (command == "lock 3 open")
    {
      if (!flag3 and con3)
      {
        flag3 = !flag3;
        last_press3 = millis();
        digitalWrite(relay_pin3, 1);
        Serial.println("lock 3 has been opened");
      }
    }

    if (command == "lock 4 open")
    {
      if (!flag4 and con4)
      {
        flag4 = !flag4;
        last_press4 = millis();
        digitalWrite(relay_pin4, 1);
        Serial.println("lock 4 has been opened");
      }
    }
    if (command == "lock 5 open")
    {
      if (!flag5 and con5)
      {
        flag5 = !flag5;
        last_press5 = millis();
        digitalWrite(relay_pin5, 1);
        Serial.println("lock 5 has been opened");
      }
    }

    if ((command == "lock 1 close") and con1)
    {
      flag1 = 0;
      digitalWrite(relay_pin1, 0);
      Serial.println("lock 1 has been closed");
    }
    if ((command == "lock 2 close") and (con2))
    {
      flag2 = 0;
      digitalWrite(relay_pin2, 0);
      Serial.println("lock 2 has been closed");
    }
    if ((command == "lock 3 close") and con3)
    {
      flag3 = 0;
      digitalWrite(relay_pin3, 0);
      Serial.println("lock 3 has been closed");
    }
    if ((command == "lock 4 close") and (con4))
    {
      flag4 = 0;
      digitalWrite(relay_pin4, 0);
      Serial.println("lock 4 has been closed");
    }
    if ((command == "lock 5 close") and (con5))
    {
      flag5 = 0;
      digitalWrite(relay_pin5, 0);
      Serial.println("lock 5 has been closed");
    }
    if (command == "lock 1 status")
    {
      if (con1)
      {
        if (flag1) Serial.println("lock 1 is open");
        else Serial.println("lock 1 is closed");
      }
      else Serial.println("lock 1 disconnected");
    }
    if (command == "lock 2 status")
    {
      if (con2)
      {
        if (flag2) Serial.println("lock 2 is open");
        else Serial.println("lock 2 is closed");
      }
      else Serial.println("lock 2 disconnected");
    }
    if (command == "lock 3 status")
    {
      if (con3)
      {
        if (flag3) Serial.println("lock 3 is open");
        else Serial.println("lock 3 is closed");
      }
      else Serial.println("lock 3 disconnected");
    }
    if (command == "lock 4 status")
    {
      if (con4)
      {
        if (flag4) Serial.println("lock 4 is open");
        else Serial.println("lock 4 is closed");
      }
      else Serial.println("lock 4 disconnected");
    }
    if (command == "lock 5 status")
    {
      if (con5)
      {
        if (flag1) Serial.println("lock 5 is open");
        else Serial.println("lock 5 is closed");
      }
      else Serial.println("lock 5 disconnected");
    }

    if (command == "tsernsor 1 get_temp")
    {
      t = dht1.readTemperature();//Измеряем температуру
      if (isnan(t)) {  // Проверка. Если не удается считать показания, выводится «Ошибка считывания», и программа завершает работу
        Serial.println("Error");
        return;
      }
      Serial.print("Temperature: ");
      Serial.print(t);
      Serial.println(" *C "); //Вывод показателей на экран
    }
    if (command == "tsernsor 2 get_temp")
    {
      t = dht2.readTemperature();//Измеряем температуру
      if (isnan(t)) {  // Проверка. Если не удается считать показания, выводится «Ошибка считывания», и программа завершает работу
        Serial.println("Error");
        return;
      }
      Serial.print("Temperature: ");
      Serial.print(t);
      Serial.println(" *C "); //Вывод показателей на экран
    }
    if (command == "hsernsor 1 get_humidity")
    {
      h = dht1.readHumidity();//Измеряем температуру
      if (isnan(h)) {  // Проверка. Если не удается считать показания, выводится «Ошибка считывания», и программа завершает работу
        Serial.println("Error");
        return;
      }
      Serial.print("Humidity: ");
      Serial.print(h);
      Serial.println(" %");

    }

    if (command == "hsernsor 2 get_humidity")
    {
      h = dht2.readHumidity();//Измеряем температуру
      if (isnan(h)) {  // Проверка. Если не удается считать показания, выводится «Ошибка считывания», и программа завершает работу
        Serial.println("Error");
        return;
      }
      Serial.print("Humidity: ");
      Serial.print(h);
      Serial.println(" %");
    }
  }
}
