#define DT 15  // шаг интегрирования в миллисекундах
const float dt = static_cast<float>(DT)/1000; // переводим в секунды

const float K = 0.18f; // Коэффициент комплиментарного фильтра

#define START_OFFSET 0 // Задержка перед запуском в миллисекундах
#define MIN_HEIGHT_INPUT 100 // Минимальный сигнал, который должен подаваться на двигатель


#include <TimerMs.h> // Библиотека для таймера
#include "GyroInput.h" // Библиотека со считыванием данных с гироскопа

TimerMs tmr(DT,1,1); // Создаём объект таймера
GyroInput gi; // Объект для связи с гироскопом
unsigned long T = 0; // Время работы программы
int counter = 0; // Тики интегрирования

int startMillis; // Момент запуска

void setup() {
  
    //while(millis() < START_OFFSET){} // Ждёт пока программа прокрутится в течении времени START_OFFSET
    
    Serial.begin(9600);
    //инициализация таймера
    tmr.setPeriodMode(); // Штука для работы таймера
    tmr.start(); // Запуск таймера
  
    gi.SetupGyro(); // инициал-ия гироскопа
    gi.GetConstError(); // среднее значние (считет ощибку)
    gi.GetConstErrorAcc();// для акселерометра (считает ошибку)
   
}

// Функция вызывается раз в DT мс и содержит все вычисления
void Calculate() {

    counter++; // Счётчик иттераций
    T += DT; // Общее время

    Vector gyrOut = gi.ReadGyro(); // Считывание значения с гироскопа

    #ifdef LOG
    // Вывод данных каждые 15 тиков, если LOG определён
      if(counter%20 == 0) {
       Serial.print("dR = ");
       PrintVector(gyrOut);
      }
    #endif
}

// Функция выводящая в консоль значения компонент вектора
void PrintVector(Vector r){
    Serial.print(r.GetX());
    Serial.print("\t\t");
    Serial.print(r.GetY());
    Serial.print("\t\t");
    Serial.println(r.GetZ());
}



// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Основной цикл программы
void loop() {
    if(tmr.tick())
      Calculate();
}
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
