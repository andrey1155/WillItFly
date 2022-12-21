#ifndef FILTERS
#define FILTERS
#include <Arduino.h>

#include "Input.h"



    MeanFilter meanX(4,1);
    MeanFilter meanY(8,1);
    MeanFilter meanZ(8,1);
    
    AsyncNode asyncX(1.f,0.1f);
    AsyncNode asyncY(1.f,0.1f);
    AsyncNode asyncZ(1.f,0.1f);
    
    AsyncNode asyncA(1,0.2f);
    AsyncNode asyncB(1,0.2f);
    
    SaturationNode satX(0.08f);
    SaturationNode satY(0.08f);
    SaturationNode satZ(0.08f);
    
    Integrator intX;
    Integrator intY;
    Integrator intZ;

    AsyncNode asyncH(1,0.2f);

    Integrator intHu;
    PD HPD(1,1);
    PD XrPD(-0.01f,-0.01f);
    PD YrPD(-0.01f,-0.01f);
    PD ZrPD(-0.01f,-0.01f);

#endif
