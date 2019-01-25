#include <IRremote.h> //ИК приемник
#include <Wire.h>  //для взаимодействия с I2C/TWI-устройствами
#include <LiquidCrystal_I2C.h> //для LCD дисплея, подключенного по I2C

LiquidCrystal_I2C lcd(0x27, 16, 2); // 0x27 - I2C адрес дисплея размером 16х2
int IRRECV_PIN = 2; //Информационный пин ИК приемника
IRrecv irrecv(IRRECV_PIN); // указываем пин, к которому подключен IR
decode_results res; //переменная для записи кодов кнопок
//Переменные для выбора цвета
byte RED = 0;
byte GREEN = 0;
byte BLUE = 0;
//Для управления цветом светодиода используем 3 ШИМ порта
byte bluePin = 5;
byte greenPin = 6;
byte redPin = 9;
byte MenuPos = 1; // Переменная для хранения текущего положения меню

void setup()
{
    //Настройка входов на выход
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    irrecv.enableIRIn();   // запускаем прием инфракрасного сигнала
    //Настройка дисплея
    lcd.init(); // Инициализация lcd
    lcd.backlight();// Включаем подсветку дисплея
    // Вывод приветствия  длительностью 3 с
    lcd.print("      RGB       ");
    lcd.setCursor(0, 1);
    lcd.print("   Nightlight    ");
    delay(3000);
    // Вывод интерактивного меню
    lcd.setCursor(0, 0);
    lcd.print("   <-Testing    ");
    lcd.setCursor(0, 1);
    lcd.print("Color Settings->");
}

void loop()
{
    if (irrecv.decode(&res)) //Если какой-либо сигнал пришел
    {
        if (res.value == 0x8AF13528) //Если нажата кнопка ОК
        {
            switch (MenuPos) // в зависимости от номера меню
            {
            case 0:
                setFadeColor(redPin, greenPin, bluePin); // Вызываем функции установки цвета и переливания из одного в другой, в качестве параметров передаем номер пина.
                setFadeColor(greenPin, bluePin, redPin); //в качестве параметров передаем номер пина.
                setFadeColor(bluePin, redPin, greenPin);
                setFadeColor(redPin, greenPin, bluePin);
                setFadeColor(greenPin, bluePin, redPin);
                setFadeColor(bluePin, redPin, greenPin);
                setColor(0, 0, 0);
                break; //запускание переливание (хамелион)
            default:
                setColor(RED, GREEN, BLUE); //устанавливаем выбранный цвет
            }
        }
        if (res.value == 0x53801EE8) //Если нажата кнопка "Вправо"
        {
            MenuPos++; //увеличиваем номер меню
            menu();//и выводим его на экран
        }
        if (res.value == 0x758C9D82)//Если нажата кнопка "Влево"
        {
            MenuPos--; //уменьшаем номер меню
            menu(); //и выводим его на экран
        }

        if (res.value == 0xC26BF044)//Если нажата кнопка "ВВерх"
        {
            switch (MenuPos) //Увеличиваем значение переменных RED,GREEN,BLUE только если находимся в 3,4 или 5 меню
            {
            case 3:
                if (RED != 255)
                {
                    RED++;
                    lcd.setCursor(0, 1);
                    lcd.print(RED);
                }
                else
                {
                    RED = 0;
                    lcd.setCursor(0, 1);
                    lcd.print("0  ");
                }
                break;  //если значение достигло максимума, сбразываем до 0 и оюновляем экран
            case 4:
                if (GREEN != 255)
                {
                    GREEN++;
                    lcd.setCursor(6, 1);
                    lcd.print(GREEN);
                }
                else
                {
                    GREEN = 0;
                    lcd.setCursor(6, 1);
                    lcd.print("0  ");
                }
                break; //иначе просто увеличиваем на 1 значение переменных и выводим их на экран
            case 5:
                if (BLUE != 255)
                {
                    BLUE++;
                    lcd.setCursor(11, 1);
                    lcd.print(BLUE);
                }
                else
                {
                    BLUE = 0;
                    lcd.setCursor(11, 1);
                    lcd.print("0  ");
                }
                break;
            }
        }
        if (res.value == 0xC4FFB646)//Если нажата кнопка "Вниз"
        {
            switch (MenuPos) //Уменьшаем значение переменных RED,GREEN,BLUE только если находимся в 3,4 или 5 меню
            {
            case 3:
                RED--;
                lcd.setCursor(0, 1);
                lcd.print(RED);
                break;
            case 4:
                GREEN--;
                lcd.setCursor(6, 1);
                lcd.print(GREEN);
                break;
            case 5:
                BLUE--;
                lcd.setCursor(11, 1);
                lcd.print(BLUE);
                break;
            }
        }
        if (res.value == 0xE13DDA28) //Если нажата кнопка "1"
        {
            setColor(255, 0, 0);    //красный цвет
        }
        if (res.value == 0xAD586662) //Если нажата кнопка "2"
        {
            setColor(0, 255, 0);   //зеленый цвет
        }
        if (res.value == 0x273009C4) //Если нажата кнопка "3"
        {
            setColor(0, 0, 255); //синий цвет
        }
        if (res.value == 0xF5999288) //Если нажата кнопка "3"
        {
            setColor(255, 255, 255); //белый цвет
        }
        if (res.value == 0x731A3E02)//Если нажата кнопка "4"
        {
            setColor(0, 0, 0); //Выключение цвета
        }

        irrecv.resume(); // принимаем следующий сигнал на ИК приемнике
    }

    // ограничения для крайних позиций меню
    if (MenuPos > 5)
    {
        MenuPos = 5;
    }
    if (MenuPos < 0)
    {
        MenuPos = 0;
    }

}

// Функция вызова меню в зависимости от текущей позиции меню - MenuPos
void menu()
{
    if (MenuPos == 0)
    {
        lcd.setCursor(0, 0); //перемещение курсора
        lcd.print("Chameleon effect"); //вывод текста
        lcd.setCursor(0, 1);
        lcd.print("   Press OK     ");
    }
    else if (MenuPos == 1)
    {
        lcd.setCursor(0, 0);
        lcd.print("   <-Testing    ");
        lcd.setCursor(0, 1);
        lcd.print("Color Settings->");
    }
    else if (MenuPos == 2)
    {
        lcd.clear(); //очистка экрана
        lcd.setCursor(0, 0);
        lcd.print("RED|GREEN|BLUE->");
        lcd.setCursor(0, 1);
        lcd.print(RED);
        lcd.setCursor(6, 1);
        lcd.print(GREEN);
        lcd.setCursor(11, 1);
        lcd.print(BLUE);

    }
    else if (MenuPos == 3)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("RED           ->");
        lcd.setCursor(0, 1);
        lcd.print(RED);
        lcd.setCursor(6, 1);
        lcd.print(GREEN);
        lcd.setCursor(11, 1);
        lcd.print(BLUE);
    }
    else if (MenuPos == 4)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("<-    GREEN   ->");
        lcd.setCursor(0, 1);
        lcd.print(RED);
        lcd.setCursor(6, 1);
        lcd.print(GREEN);
        lcd.setCursor(11, 1);
        lcd.print(BLUE);
    }
    else if (MenuPos == 5)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("<-          BLUE");
        lcd.setCursor(0, 1);
        lcd.print(RED);
        lcd.setCursor(6, 1);
        lcd.print(GREEN);
        lcd.setCursor(11, 1);
        lcd.print(BLUE);
    }
}
// Функция включения необходимого цвета на нашем RGB светодиоде
void setColor(byte red, byte green, byte blue)
{
    analogWrite(redPin, red); //Выдает аналоговую величину (ШИМ волну).
    analogWrite(greenPin, green);
    analogWrite(bluePin, blue);
}
// хамелион (переливание цветов)
void setFadeColor(byte red, byte green, byte blue)
{
    for (int i = 0; i <= 255; i++)
    {
        analogWrite(red, i);
        analogWrite(green, 255 - i);
        analogWrite(blue, 255);
        delay(10);
    }
}

