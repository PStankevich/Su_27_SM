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
int gearWarning = 11;
int gear = 13;
int airBreak = 12;
int Noseflap = 2;
int flaps = 10;
int flapsLand = 9;
int masterWarning = 3;
int indexCount = 0;
int charIndex = 0;
int charCount = 0;
int countDelay;
int dataSize;

void setup() {
	Serial.begin(115200); // устанавливаем последовательное соединение
	pinMode(gear, OUTPUT);
	pinMode(airBreak, OUTPUT);
	pinMode(gearWarning, OUTPUT);
	pinMode(flaps, OUTPUT);
	pinMode(flapsLand, OUTPUT);
	pinMode(Noseflap, OUTPUT);
	pinMode(masterWarning, OUTPUT);
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
				/*
				Serial.print("data[");
				Serial.print(i);
				Serial.print("] = ");
				Serial.println(data[i]);
				*/
				indexCount = charIndex + 1;
			}
			if (data[0] > 0 && data[0] < 1.0) {
				digitalWrite(gearWarning, HIGH);
			}
			else {
				digitalWrite(gearWarning, LOW);
			} 
			if (data[0] == 1) {
				digitalWrite(gear, HIGH);
			}
			else {
				digitalWrite(gear, LOW);
			}
			if (data[5] == 6) {
				if (data[1] >= 0.24) {
					digitalWrite(flaps, HIGH);
				}
				else {
					digitalWrite(flaps, LOW);
				}
					if (data[1] >= 0.93) {
						digitalWrite(flapsLand, HIGH);
					}
					else {
						digitalWrite(flapsLand, LOW);
					}
			}
			else if (data[5] == 1) {
				if (data[1] >= 1) {
					digitalWrite(flaps, HIGH);
				}
				else {
					digitalWrite(flaps, LOW);
				}
			}
			

			if (data[2] == 1) {
				digitalWrite(airBreak, HIGH);
			}
			else {
				digitalWrite(airBreak, LOW);
			}
				if (data[3] >= 23) {
					digitalWrite(Noseflap, HIGH);
				}
				else {
					digitalWrite(Noseflap, LOW);
				}
			
			if (data[4] == 1) {
				digitalWrite(masterWarning, HIGH);
			}
			else {
				digitalWrite(masterWarning, LOW);
			}

			Data = "";
		}
	}
	indexCount = 0;
	charIndex = 0;
}
