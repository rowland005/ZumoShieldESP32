#include <Wire.h>
#include "LSM6.h"
#include "LIS3MDL.h"

class CompassManager{
private:
  LSM6 imu;
  LIS3MDL mag;
  LSM6::vector<int16_t> accRunningMin = {32767, 32767, 32767}, accRunningMax = {-32768, -32768, -32768};
  LSM6::vector<int16_t> gyrRunningMin = {32767, 32767, 32767}, gyrRunningMax = {-32768, -32768, -32768};
  LIS3MDL::vector<int16_t> magRunningMin = {32767, 32767, 32767}, magRunningMax = {-32768, -32768, -32768};

  float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }

public:
  typedef struct Vector3D{
    float X = 0.0f;
    float Y = 0.0f;
    float Z = 0.0f;  
  } Vector3D;
  
  CompassManager(){}

  void Initialize(){
    Wire.begin();
    imu.init();
    imu.enableDefault();

    mag.init();
    mag.enableDefault();
  }

  void UpdateReadings(){
    imu.read();
    mag.read();
  }
  
  Vector3D GetMagnetometerData(){
    Vector3D magData;
    
    if(millis() < 10000){
      Serial.print("Calibrating: \t");
      
      magRunningMin.x = min(magRunningMin.x, mag.m.x);
      magRunningMin.y = min(magRunningMin.y, mag.m.y);
      magRunningMin.z = min(magRunningMin.z, mag.m.z);
    
      magRunningMax.x = max(magRunningMax.x, mag.m.x);
      magRunningMax.y = max(magRunningMax.y, mag.m.y);
      magRunningMax.z = max(magRunningMax.z, mag.m.z);
    }
  
    magData.X = mapf(mag.m.x, magRunningMin.x, magRunningMax.x, -100.0f, 100.0f);
    magData.Y = mapf(mag.m.y, magRunningMin.y, magRunningMax.y, -100.0f, 100.0f);
    magData.Z = mapf(mag.m.z, magRunningMin.z, magRunningMax.z, -100.0f, 100.0f);
    
    return magData;
  }

  Vector3D GetAccelerometerData(){
    Vector3D accData;

    if(millis() < 10000){
      accRunningMin.x = min(accRunningMin.x, imu.a.x);
      accRunningMin.y = min(accRunningMin.y, imu.a.y);
      accRunningMin.z = min(accRunningMin.z, imu.a.z);
    
      accRunningMax.x = max(accRunningMax.x, imu.a.x);
      accRunningMax.y = max(accRunningMax.y, imu.a.y);
      accRunningMax.z = max(accRunningMax.z, imu.a.z);
    }
  
    accData.X = mapf(imu.a.x, accRunningMin.x, accRunningMax.x, -100.0f, 100.0f);
    accData.Y = mapf(imu.a.y, accRunningMin.y, accRunningMax.y, -100.0f, 100.0f);
    accData.Z = mapf(imu.a.z, accRunningMin.z, accRunningMax.z, -100.0f, 100.0f);
    
    return accData;
  }
  
  Vector3D GetGyroscopeData(){
    Vector3D gyrData;

    if(millis() < 10000){
      gyrRunningMin.x = min(gyrRunningMin.x, imu.g.x);
      gyrRunningMin.y = min(gyrRunningMin.y, imu.g.y);
      gyrRunningMin.z = min(gyrRunningMin.z, imu.g.z);
    
      gyrRunningMax.x = max(gyrRunningMax.x, imu.g.x);
      gyrRunningMax.y = max(gyrRunningMax.y, imu.g.y);
      gyrRunningMax.z = max(gyrRunningMax.z, imu.g.z);
    }
  
    gyrData.X = mapf(imu.g.x, accRunningMin.x, accRunningMax.x, -100.0f, 100.0f);
    gyrData.Y = mapf(imu.g.y, accRunningMin.y, accRunningMax.y, -100.0f, 100.0f);
    gyrData.Z = mapf(imu.g.z, accRunningMin.z, accRunningMax.z, -100.0f, 100.0f);
    
    return gyrData;
  }

  void PrintMagnetometer(){
    Vector3D magData = GetMagnetometerData();
    
    Serial.print(magData.X);
    Serial.print("\t");
    Serial.print(magData.Y);
    Serial.print("\t");
    Serial.println(magData.Z);
  }
  
  void PrintAccelerometer(){
    Vector3D accData = GetAccelerometerData();
    
    Serial.print(accData.X);
    Serial.print("\t");
    Serial.print(accData.Y);
    Serial.print("\t");
    Serial.println(accData.Z);
  }

  void PrintGyroscope(){
    Vector3D gyrData = GetGyroscopeData();
    
    Serial.print(gyrData.X);
    Serial.print("\t");
    Serial.print(gyrData.Y);
    Serial.print("\t");
    Serial.println(gyrData.Z);
  }
  
  float GetHeading(){
    Vector3D magData = GetMagnetometerData();
    Vector3D accData = GetAccelerometerData();
    Vector3D hIO;//hard iron offsets

    //Magnetometer direct reading
    //float heading = fmod(atan2(magData.Z, magData.Y) * 180.0f / 3.14159f, 360.0f);

    //Magnetometer tilt compensation
    float roll = atan2(accData.Y, accData.Z);
    float pitch = atan(-accData.X / (accData.Y * sin(roll) + accData.Z * cos(roll)));
    float yaw = atan2((magData.Z - hIO.Z) * sin(roll) - (magData.Y - hIO.Y) * cos(roll), 
                      (magData.X - hIO.X) * cos(pitch) + (magData.Y - hIO.Y) * sin(pitch) * sin(roll) + (magData.Z - hIO.Z) * sin(pitch) * cos(roll));
    
    if(yaw < 0) yaw += 2.0f * 3.14159f;

    yaw = yaw * 180.0f / 3.14159f;

    return yaw;
  }
};
