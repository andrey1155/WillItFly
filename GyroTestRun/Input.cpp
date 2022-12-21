#include "Input.h"  
    
    
#if INTEGRATOR == ADAMS2

   Integrator::Integrator(){
      Y = 0;
   }
   float Integrator::Integrate(float X, float dt){
      Y += X*dt;
      return Y;
   }

#endif

#if INTEGRATOR == TRAP
  Integrator::Integrator(){
      Y = 0;
      prevX = 0;
   }
   float Integrator::Integrate(float X, float dt){
      Y += dt*(X+prevX)/2;
      prevX = X;
      return Y;
   }

#endif

#if INTEGRATOR == ADAMS1
  Integrator::Integrator(){
      Y = 0;
      prevX = 0;
   }
   float Integrator::Integrate(float X, float dt){
      Y += dt*(3/2*X - 1/2*prevX);
      prevX = X;
      return Y;
   }
#endif

#if INTEGRATOR == ADAMS2
  Integrator::Integrator(){
      Y = 0;
      prevX1 = 0;
      prevX2 = 0;
   }
   float Integrator::Integrate(float X, float dt){
      Y += dt*(23/12*X - 4/3*prevX1 + 5/12*prevX2);
      prevX2 = prevX1;
      prevX1 = X;
      return Y;
   }
#endif

#if INTEGRATOR == ADAMS3
  Integrator::Integrator(){
      Y = 0;
      prevX1 = 0;
      prevX2 = 0;
      prevX3 = 0;
   }
   float Integrator::Integrate(float X, float dt){
      Y += dt*(55/24*X - 59/24*prevX1 + 37/24*prevX2 -3/8*prevX3);
      prevX3 = prevX2;
      prevX2 = prevX1;
      prevX1 = X;
      return Y;
   }
#endif



    float AsyncNode::Integrate(float input, float dt){
        float dy = 1/T*(k*input - Y);
        Y = int_.Integrate(dy,dt);
        return Y;
    }
    
    AsyncNode::AsyncNode(float k, float T){
      this->T = T;
      this->k = k;
    }


    MeanFilter::MeanFilter(int L, int ofst){
      arr = new float[L];
      Length = L;
      this->ofst = ofst;
      index = 0;

      for(int i = 0; i < Length;i++)
        arr[i] = 0;
      
    }

    MeanFilter::~MeanFilter(){
      delete[] arr;
    }
    
    float MeanFilter::Filter(float input){ 
      arr[index] = input;

      index++;
      if(index == Length)
        index = 0;

      float* tmp = new float[Length]; 

      for(int i = 0; i < Length;i++)
        tmp[i] = arr[i];

      for(int i = 0; i < Length-1;i++)
        for(int j = 1; j < Length;j++){
          if(tmp[i] > arr[i])
          {
            float t = arr[i];
            arr[i] = tmp[i];
            tmp[i] = t;
          }
        }

      float S = 0;
      for(int i = ofst; i < Length - ofst; i++)
          S += tmp[i];
        
      delete[] tmp;

      return S / (Length - 2*ofst);
    }

    ModAsync::ModAsync(float cutoff,float K){
      this->cutoff = cutoff;
      this->prevOutput = 0;
      this->K = K;
    }
 
    float ModAsync::Integrate(float input,float dt){

      input -= prevOutput;
      
      int k = input > 0 ? 1 : -1;
      input = abs(input);

      if (input > cutoff)
        input = cutoff;
      
      input *= k*K;
     
      input = I.Integrate(input,dt);
      
      prevOutput = input;
      return  input;
    }


    float PD::Integrate(float u, float dt){

      float ret = kp*u + kd*(u - prevU)/dt;
      prevU = u;
      return ret;
      
    }
    
    PD::PD(float kp, float kd){
      this->kp = kp;
      this->kd = kd;
    }

    float PD::IntegrateDirect(float u, float du){
      return u*kp + du*kd;
    }

    
