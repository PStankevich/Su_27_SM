#include <LiquidCrystal_I2C.h>

//#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

char incomingByte = 0;  // переменная для хранения полученного байта
boolean flag = true;
int in;
String Data;
String DataNum;
int gear = 5;
int indexCount = 0;
int charIndex = 0;
int charCount = 0;
int countDelay;
int dataSize;

void setup() {
  Serial.begin(57600); // устанавливаем последовательное соединение
  pinMode(gear, OUTPUT);
  pinMode(13, OUTPUT);
  while (!Serial) {
    // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.println("UART is Started!");
  //  //  lcd.init();                     // инициализация LCD
  //  lcd.backlight();                // включаем подсветку
  //  lcd.clear();                    // очистка дисплея
  //  lcd.setCursor(2, 0);
  //  lcd.print("FlySim :)");          // вывод надписи
  //  lcd.setCursor(0, 1);            // устанавливаем курсор на 2 строку, 1 символ
  //  lcd.print("Wait DCS...");
}

void loop() {

  if (Serial.available() > 0) {  //если есть доступные данные
    incomingByte = (char)(Serial.read());
    if (incomingByte != '/') {
      Data = Data + incomingByte;
    } else {
      charIndex = Data.indexOf(' '); //индекс пробела
      dataSize = Data.substring(0, charIndex).toInt();
      indexCount = charIndex + 1;
      float data[dataSize];
      char floatbufVar[32];
      String blin;
      for (int i = 0; i < dataSize; i++) {
        charIndex = Data.indexOf(' ', indexCount); //индекс пробела
        blin = Data.substring(indexCount, charIndex);
        blin.toCharArray(floatbufVar, sizeof(floatbufVar));
        data[i] = atof(floatbufVar);
        //          charIndex++;
        indexCount = charIndex + 1;
      }
      if (data[0] == 1) {
        Serial.print("data[0] == ");
        Serial.println(data[0]);
        digitalWrite(13, HIGH);
      } else {
        digitalWrite(13, LOW);
        Serial.print("data[0] == ");
        Serial.println(data[0]);
      }
      Data = "";
    }
  }
  indexCount = 0;
  charIndex = 0;
}
