#ifndef ENGINES
#define ENGINES
#include <Arduino.h>

// Номера портов для движков
#define SPEED_1 3
#define SPEED_2 9
#define SPEED_3 10
#define SPEED_4 6


// Коэффициент пересчёта для функции 
#define KENG 1

#define NODIR


void Init();
void runEngine(int, int, int, int);



#endif
