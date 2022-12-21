#ifndef ENGINES
#define ENGINES
#include <Arduino.h>

// Номера портов для движков
#define SPEED_1 5
#define SPEED_2 6
#define SPEED_3 9
#define SPEED_4 10


// Коэффициент пересчёта для функции 
#define KENG 1

#define NODIR


void Init();
void runEngine(int, int, int, int);



#endif
