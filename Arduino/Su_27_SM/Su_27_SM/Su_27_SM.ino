/*
 Name:		Su_27_SM.ino
 Created:	24.07.2016 18:37:07
 Author:	General
*/

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
	Serial.begin(19200); // устанавливаем последовательное соединение
	pinMode(gear, OUTPUT);
	pinMode(13, OUTPUT);
	pinMode(12, OUTPUT);
	pinMode(10, OUTPUT);
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
		}
		else {
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
				Serial.print("data[");
				Serial.print(i);
				Serial.print("] = ");
				Serial.println(data[i]);
				indexCount = charIndex + 1;
			}
			if (data[0] == 1) {
				digitalWrite(13, HIGH);
			}
			else {
				digitalWrite(13, LOW);
			} 
			if (data[1] == 1) {
				digitalWrite(10, HIGH);
			}
			else {
				digitalWrite(10, LOW);
			}
			if (data[2] == 1) {
				digitalWrite(12, HIGH);
			}
			else {
				digitalWrite(12, LOW);
			}

			Data = "";
		}
	}
	indexCount = 0;
	charIndex = 0;
}
