#ifndef GYROINPUT
#define GYROINPUT
#include <Arduino.h>

#include "Vector.h"
#include <Arduino_LSM6DS3.h>

#define BUFFER 100
class GyroInput{
  private:
    float dx,dy,dz;
    float dax,day,daz;
  public:

    void GetConstError();
    void GetConstErrorAcc();
    Vector ReadGyro();
    Vector ReadAcc();
    void SetupGyro();
  
};


#endif
