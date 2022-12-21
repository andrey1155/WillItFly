#include "Engines.h"

#ifdef NODIR

  void Init(){
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);  
  }

  void runEngine(int in1, int in2, int in3, int in4){
    
    analogWrite(SPEED_1, in1);
    analogWrite(SPEED_2, in2);
    analogWrite(SPEED_3, in3);
    analogWrite(SPEED_4, in4);
  }
#endif

#ifdef DIR

  void Init(){
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);  
  }

  void runEngine(int in1, int in2, int in3, int in4){

    digitalWrite(DIR_1, HIGH);
    digitalWrite(DIR_2, HIGH);
    digitalWrite(DIR_3, HIGH);
    digitalWrite(DIR_4, HIGH);
    
    analogWrite(SPEED_1, in1);
    analogWrite(SPEED_2, in2);
    analogWrite(SPEED_3, in3);
    analogWrite(SPEED_4, in4);
  }
#endif
