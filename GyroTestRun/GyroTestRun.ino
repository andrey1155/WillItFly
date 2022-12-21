#define DT 15  // шаг интегрирования в миллисекундах
const float dt = static_cast<float>(DT)/1000; // переводим в секунды

const float K = 0.18f; // Коэффициент комплиментарного фильтра

#define KL 10 // коэфициент пересчета по углу крена и тангажа с ПИД-регулятора в двигатель
#define KA 0 // коэфициент пересчета по углу рыскания с ПИД-регулятора в двигатель

 // ограничение на время теста
#define LOG

#define START_OFFSET 0 // Задержка перед запуском в миллисекундах
#define MIN_HEIGHT_INPUT 100 // Минимальный сигнал, который должен подаваться на двигатель


#include <TimerMs.h> // Библиотека для таймера
#include "Input.h" // Библиотека с динамическими звеньями
#include "GyroInput.h" // Библиотека со считыванием данных с гироскопа
#include "Filters.h" // Библиотека с определением фильтров аппериодики
#include "Engines.h" // Код для двигателей

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
      
    #ifdef STOP
      Serial.println("Start");
    #endif
    
    startMillis = millis(); // Начало отсчёта времени
  
    pinMode(trigPin, OUTPUT); // Настраиваем пин триггера дальномера на выход
    pinMode(echoPin, INPUT); // Настраиваем пин эхо дальномера на вход
    
}

// Функция вызывается раз в DT мс и содержит все вычисления
void Calculate() {

    counter++; // Счётчик иттераций
    T += DT; // Общее время

    Vector gyrOut = gi.ReadGyro(); // Считывание значения с гироскопа
    Vector accOut = gi.ReadAcc(); // Считывание значения с акселерометра
  
    gyrOut = FilterRot(gyrOut); // Фильтр от шума  
    Vector rot = CalculateRotation(gyrOut, accOut); // Интегрирование угловых скоростей
    Vector ru  = CalculateRotationOutput(rot,gyrOut); // Управляющий сигнал по углам
    CalculateEngines(0, ru);
    
    #ifdef LOG
    // Вывод данных каждые 15 тиков, если LOG определён
      if(counter%20 == 0) {
       Serial.print("dR = ");
       PrintVector(gyrOut);
      }
    #endif
  
    #ifdef STOP // Идентифицируем конец программы, если определён STOP и время слишком большое
      if(T >= 15000) {
          Serial.println("Finish");
          Serial.println("Fixed calculation time elapsed:");
          Serial.println(T);
          Serial.println("Timer calculation time elapsed:");
          Serial.println((millis() - startMillis));   
          while(1){}
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


// Считает и возвращает вектор выходного состояния
Vector FilterRot(Vector r){
    float X = r.GetX(); // Компонента X вектора, подаваемого на вход (r)
    float Y = r.GetY(); // Компонента Y вектора, подаваемого на вход (r)
    float Z = r.GetZ(); // Компонента Z вектора, подаваемого на вход (r)
    
    X = asyncX.Integrate(X,dt); // Апериодическое звено
    X = satX.Filter(X); // Зона нечувствительности
    X = intX.Integrate(X,dt); // Интеграл
    
    Y = asyncY.Integrate(Y,dt); // Апериодическое звено
    Y = satY.Filter(Y); // Зона нечувствительности
    Y = intY.Integrate(Y,dt); // Интеграл
    
    Z = asyncZ.Integrate(Z,dt); // Апериодическое звено
    Z = satZ.Filter(Z); // Зона нечувствительности
    Z = intZ.Integrate(Z,dt); // Интеграл
    
    return Vector(X,Y,Z); 
}

// Поправка по акселерометрам
Vector CalculateRotation(Vector r, Vector a){

    float X = r.GetX();
    float Y = r.GetY(); 
    float Z = r.GetZ();
   
    float Yrot = 180 / 3.1415f * asin(a.GetX()/9.80665);
    float Xrot = 180 / 3.1415f * asin(a.GetY()/9.80665);


    if(!isnan(Yrot))
      Y = K*Yrot + (1-K)*Y;
      
    if(!isnan(Xrot))
      X = K*Xrot + (1-K)*X;
    
    a = Vector(Y,-Z,X);
  
    return a;
}

// Расчёт для каждой оси вращения управляющего сигнала (после ПД-регуляторА)
Vector CalculateRotationOutput(Vector rot, Vector dr){
    float x = XrPD.IntegrateDirect(rot.GetX(),+dr.GetY());
    float y = YrPD.IntegrateDirect(rot.GetY(),-dr.GetZ());
    float z = ZrPD.IntegrateDirect(rot.GetZ(),+dr.GetX());
    
    return  Vector(x,y,z);
}

// Управление двигателем
void CalculateEngines(float heightInput, Vector r){
  
    // Скорости вращения для каждого из 4 винтов
    float w1 = 0.25f * heightInput - KL * r.GetZ() - KA * r.GetY();
    float w2 = 0.25f * heightInput - KL * r.GetX() + KA * r.GetY();
    float w3 = 0.25f * heightInput + KL * r.GetZ() - KA * r.GetY();
    float w4 = 0.25f * heightInput + KL * r.GetX() + KA * r.GetY();
  
    // Расчёт значения подаваемого на ШИМ
    int u1 = static_cast<int>(KENG * w1);
    int u2 = static_cast<int>(KENG * w2);
    int u3 = static_cast<int>(KENG * w3);
    int u4 = static_cast<int>(KENG * w4);
  
    analogWrite(SPEED_1, u1);
    analogWrite(SPEED_2, u2);
    analogWrite(SPEED_3, u3);
    analogWrite(SPEED_4, u4);
}

// Расчёт высоты с поправкой на угол наклона коптера
float CalculateHeight(float A1, float A2){
    float h = static_cast<float>(getDistSm()); // Считывание высотомера
  
    h *= cos(A1)*cos(A2); // Поправка
    float dh = getFlightTask() - h; // Расчёт ошибки по высоте
    return HPD.Integrate(asyncH.Integrate(dh,dt),dt) + 0.1f*intHu.Integrate(dh,dt); // Выход с ПИД-регулятора
}



// Ограничивает минимальное значение X
int bound(int X, int low, int high){
    if(X < low)
      X = low;
  
    return X;
}


// Управление двигателем
void CalculateEngines(float heightInput, Vector r){
  
    // Скорости вращения для каждого из 4 винтов
    float w1 = 0.25f * heightInput - KL * r.GetZ() - KA * r.GetY();
    float w2 = 0.25f * heightInput - KL * r.GetX() + KA * r.GetY();
    float w4 = 0.25f * heightInput + KL * r.GetZ() - KA * r.GetY();
    float w3 = 0.25f * heightInput + KL * r.GetX() + KA * r.GetY();
  
    // Расчёт значения подаваемого на ШИМ
    int u1 = static_cast<int>(KENG * w1);
    int u2 = static_cast<int>(KENG * w2);
    int u3 = static_cast<int>(KENG * w3);
    int u4 = static_cast<int>(KENG * w4);

    u1 = bound(u1,0,255);
    u2 = bound(u2,0,255);
    u3 = bound(u3,0,255);
    u4 = bound(u4,0,255);
    
    analogWrite(SPEED_1, u1);
    analogWrite(SPEED_2, u2);
    analogWrite(SPEED_3, u3);
    analogWrite(SPEED_4, u4);
}


// Ограничивает минимальное значение X
int bound(int X, int low, int high){
    if(X < low)
      X = low;
  
    return X;
}
