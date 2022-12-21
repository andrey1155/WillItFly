#ifndef INIT
#define INIT
#include <Arduino.h>

#include <TimerOne.h>
#include <TimerMs.h>

void InitializePins(){
  
}

void InitializeOtherStuff(void* Calculate){
  Timer1.initialize(60*1000);
  Timer1.attachInterrupt(Calculate);
  Serial.begin(9600);
}

#endif
