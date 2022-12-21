#ifndef DATAINPUT
#define DATAINPUT
#include <Arduino.h>

#define RECT 0
#define TRAP 1
#define ADAMS1 2
#define ADAMS2 3
#define ADAMS3 4

#define INTEGRATOR ADAMS3




#if INTEGRATOR == RECT
  class Integrator{
    private:
      float Y;
    public: 
      Integrator();
      float Integrate(float, float);
  };
#endif

#if INTEGRATOR == TRAP
  class Integrator{
    private:
      float Y, prevX;
    public: 
      Integrator();
      float Integrate(float, float);
  };
#endif

#if INTEGRATOR == ADAMS1
  class Integrator{
    private:
      float Y, prevX;
    public: 
      Integrator();
      float Integrate(float, float);
  };
#endif

#if INTEGRATOR == ADAMS2
  class Integrator{
    private:
      float Y, prevX1, prevX2;
    public: 
      Integrator();
      float Integrate(float, float);
  };
#endif

#if INTEGRATOR == ADAMS3
  class Integrator{
    private:
      float Y, prevX1, prevX2, prevX3;
    public: 
      Integrator();
      float Integrate(float, float);
  };
#endif

class AsyncNode{
  private:
    float Y, k, T;
    Integrator int_;
  public:
    float Integrate(float,float);
    AsyncNode(float,float);
};

class MeanFilter{
  private:
    float* arr;
    int Length,ofst;
    int index;
  public:
    ~MeanFilter();
    MeanFilter(int,int);
    float Filter(float);
};


class SaturationNode{
  public:
    float Filter(float in){
        
      if(abs(in) < sat){
        in = 0;
      }

      return in;
    }
    SaturationNode(float ofst){
      sat = ofst;
    }
  private:
    float sat;
};


class ModAsync{
  public:
    ModAsync(float,float);
    float Integrate(float,float);
  private:
    float K;
    Integrator I;
    float cutoff;
    float prevOutput;

};

class PD{
  public:
    float Integrate(float,float);
    float IntegrateDirect(float,float);
    PD(float,float);
  private:
    float prevU;
    float kp, kd;
};


#endif
