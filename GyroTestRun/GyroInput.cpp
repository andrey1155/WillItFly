#include "GyroInput.h"
#include "Vector.h"



void GyroInput::SetupGyro(){
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");

    while (1);
  }

  Serial.print("Gyroscope sample rate = ");
  Serial.println(IMU.gyroscopeSampleRate());
  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Gyroscope in degrees/second");
  Serial.println("X\tY\tZ");
}


Vector GyroInput::ReadGyro(){
  float x, y, z;

  if (IMU.gyroscopeAvailable()){
    IMU.readGyroscope(x, y, z);
  }
  return Vector(x-dx,y-dy,z-dz);
}

void GyroInput::GetConstError(){

  int i = 0;
  float x, y, z;
  float fx = 0, fy = 0, fz = 0;

  while (i < BUFFER){
    if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(x, y, z);
    i++;
    fx += x;
    fy += y;
    fz += z;
    }
  }
  
  dx = fx/BUFFER;
  dy = fy/BUFFER; 
  dz = fz/BUFFER;
}

void GyroInput::GetConstErrorAcc(){

  int i = 0;
  float x, y, z;
  float fx = 0, fy = 0, fz = 0;

  while (i < BUFFER){
    if (IMU.accelerationAvailable()){ 
      IMU.readAcceleration(x, y, z);
    i++;
    fx += x;
    fy += y;
    fz += z;
    }
  }
  
  dax = fx/BUFFER;
  day = fy/BUFFER; 
  daz = fz/BUFFER;
}


Vector GyroInput::ReadAcc(){
  float x, y, z;

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);
  }
  return Vector(9.80665*(x-dax),9.80665*(y-day),9.80665*(z-daz+1));
}
