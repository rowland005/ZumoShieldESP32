#include <Wire.h>
#include "LSM303.h"

class CompassManager{
private:
  LSM303 compass;
  LSM303::vector<int16_t> magRunningMin = {32767, 32767, 32767}, magRunningMax = {-32768, -32768, -32768};
  LSM303::vector<int16_t> accRunningMin = {32767, 32767, 32767}, accRunningMax = {-32768, -32768, -32768};

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
    compass.init();
    compass.enableDefault();
  }

  void UpdateReadings(){
    compass.read();
  }
  
  Vector3D GetMagnetometerData(){
    Vector3D magData;
    
    if(millis() < 10000){
      Serial.print("Calibrating: \t");
      
      magRunningMin.x = min(magRunningMin.x, compass.m.x);
      magRunningMin.y = min(magRunningMin.y, compass.m.y);
      magRunningMin.z = min(magRunningMin.z, compass.m.z);
    
      magRunningMax.x = max(magRunningMax.x, compass.m.x);
      magRunningMax.y = max(magRunningMax.y, compass.m.y);
      magRunningMax.z = max(magRunningMax.z, compass.m.z);
    }
  
    magData.X = mapf(compass.m.x, magRunningMin.x, magRunningMax.x, -100.0f, 100.0f);
    magData.Y = mapf(compass.m.y, magRunningMin.y, magRunningMax.y, -100.0f, 100.0f);
    magData.Z = mapf(compass.m.z, magRunningMin.z, magRunningMax.z, -100.0f, 100.0f);
    
    return magData;
  }

  Vector3D GetAccelerometerData(){
    Vector3D accData;

    if(millis() < 10000){
      accRunningMin.x = min(accRunningMin.x, compass.a.x);
      accRunningMin.y = min(accRunningMin.y, compass.a.y);
      accRunningMin.z = min(accRunningMin.z, compass.a.z);
    
      accRunningMax.x = max(accRunningMax.x, compass.a.x);
      accRunningMax.y = max(accRunningMax.y, compass.a.y);
      accRunningMax.z = max(accRunningMax.z, compass.a.z);
    }
  
    accData.X = mapf(compass.a.x, accRunningMin.x, accRunningMax.x, -100.0f, 100.0f);
    accData.Y = mapf(compass.a.y, accRunningMin.y, accRunningMax.y, -100.0f, 100.0f);
    accData.Z = mapf(compass.a.z, accRunningMin.z, accRunningMax.z, -100.0f, 100.0f);
    
    return accData;
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
