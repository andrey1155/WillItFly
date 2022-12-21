#ifndef VECTOR
#define VECTOR
#include <Arduino.h>



class Vector{
  public:

      Vector(){
      x = 0;
      y = 0;
      z = 0;
    }

    Vector(float x, float y, float z){
      this->x = x;
      this->y = y;
      this->z = z;
    }

    float GetX()const{return x;}
    float GetY()const{return y;}
    float GetZ()const{return z;} 

    Vector Mul(float a){
      return Vector(x*a,y*a,z*a);
    }
    
    private:

      float x, y, z;
};



#endif
