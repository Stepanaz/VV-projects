#include <Adafruit_NeoPixel.h>

#define PIN 10              // указываем пин для подключения ленты
#define NUMPIXELS 15  // указываем количество светодиодов в ленте
#define PINkek 5 //пин для пищалки

// change this to make the song slower  or faster
int tempo = 114;

// change this to whichever pin you want to use
int buzzer = PINkek;

int divider = 0, noteDuration = 0;

// создаем объект strip с нужными характеристиками
Adafruit_NeoPixel strip (NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int val;
bool pishalka;
bool rick_roll;
void setup() {
   rick_roll = true;
   pinMode(PINkek, OUTPUT);
   Serial.begin(115200);
   strip.begin();                     // инициализируем ленту
   strip.setBrightness(200);  // указываем яркость светодиодов (максимум 255)
}

void loop() {

   if (Serial.available())
   {
      val = Serial.read();
      switch (val)
      {
          case '2':
            {
             for (int i = 0; i < NUMPIXELS; i++)
                {
                strip.setPixelColor(i, strip.Color(255, 0, 0));
                }
             strip.show();
             Serial.println("RED light activated"); 
             break;
            }
          case '3':
            {
             strip.clear();
             strip.show();
             Serial.println("RED light deactivated");
             break;
            }
          case '4':
             {
             for (int i = 0; i < NUMPIXELS; i++)
                {
                strip.setPixelColor(i, strip.Color(255, 100, 0));
                }
             strip.show();
             Serial.println("YELLOW light activated");
             break;
            }
          case '5':
            {
             strip.clear();
             strip.show();
             Serial.println("YELLOW light deactivated");
             break;
            }
          case '6':
          {
             for (int i = 0; i < NUMPIXELS; i++)
                {
                  strip.setPixelColor(i, strip.Color(0, 255, 0));
                }
            strip.show();
            Serial.println("GREEN light activated");
            break;
            }
          case '7':
            {
            strip.clear();
            strip.show();
            Serial.println("GREEN light deactivated");
            break;
            }
          case '8':{
              for (int i = 0; i < NUMPIXELS; i++) {
                  strip.setPixelColor(i, strip.Color(255, 0, 0));
                  strip.show();
                  delay(100);
              }

   // поочередно включаем жёлтый цвет
              for (int i = 0; i < NUMPIXELS; i++) {
                    strip.setPixelColor(i, strip.Color(255, 100, 0));
                    strip.show();
                    delay(100);
              }

   // поочередно включаем зелёный цвет
              for (int i = 0; i < NUMPIXELS; i++) {
                    strip.setPixelColor(i, strip.Color(0, 255, 0));
                    strip.show();
                    delay(100);
              }
          Serial.println("ALL lights activated");
          break;
          }
          case '9':
            {
            strip.clear();
            strip.show();
            Serial.println("ALL lights deactivated");
            break;
            }
          case '0':
          {
            pishalka = true;
            Serial.println("SOUND activated");
            break;
          }
          case '1':
          {
           pishalka = false;
           Serial.println("SOUND deactivated");
           break;
          }

          default:
                // выполнить, если значение не совпадает ни с одним из case
            break;
      }
   }
    //delay(1000);
if (rick_roll) {
    rick_roll = false;
    noTone(PINkek);
    
              for (int i = 0; i < NUMPIXELS; i++) {
                  strip.setPixelColor(i, strip.Color(255, 0, 0));
                  strip.show();
                  //delay(100);
              }

   // поочередно включаем жёлтый цвет
              for (int i = 0; i < NUMPIXELS; i++) {
                    strip.setPixelColor(i, strip.Color(255, 100, 0));
                    strip.show();
                    //delay(100);
              }

   // поочередно включаем зелёный цвет
              for (int i = 0; i < NUMPIXELS; i++) {
                    strip.setPixelColor(i, strip.Color(0, 255, 0));
                    strip.show();
                    //delay(100);
              }
      strip.clear();
      strip.show();
}
if (pishalka) 
  {
     tone (PINkek, 600); // включаем на пьезодинамик 600 Гц

     delay(1000); // ждем 1 секунду

     tone(PINkek, 900); // включаем на пьезодинамик 900 Гц

     delay(1000); // ждем 1 секунду

     //noTone(PINkek); // отключаем пьезодинамик на пин 11

     // ждем 1 секунду
  }
else noTone(PINkek);
}
