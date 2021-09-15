#pragma once

#include <QTRSensors.h>

class IRArray{
private:
  static QTRSensors qtr;
  
  static const uint8_t SensorCount = 6;
  static uint16_t sensorValues[SensorCount];
  
  //UNO: 4 3, 11, 0, 2, 5
  //ESP32 D1 R32: 
  static const uint8_t pinLR1 = 17;
  static const uint8_t pinLR2 = 34;
  static const uint8_t pinLR3 = 23;
  static const uint8_t pinLR4 = 2;
  static const uint8_t pinLR5 = 36;
  static const uint8_t pinLR6 = 16;
  static const uint8_t pinOn = 26;

  static float irLR1;
  static float irLR2;
  static float irLR3;
  static float irLR4;
  static float irLR5;
  static float irLR6;
  
public:
  static void Initialize(){
    qtr.setTypeRC();
    qtr.setSensorPins((const uint8_t[]){pinLR1, pinLR2, pinLR3, pinLR4, pinLR5, pinLR6}, SensorCount);
    qtr.setEmitterPin(pinOn);
  }

  static void ReadArray(){
    qtr.read(sensorValues);
    
    irLR1 = ((float)sensorValues[0]) / 2500.0f;
    irLR2 = ((float)sensorValues[1]) / 2500.0f;
    irLR3 = ((float)sensorValues[2]) / 2500.0f;
    irLR4 = ((float)sensorValues[3]) / 2500.0f;
    irLR5 = ((float)sensorValues[4]) / 2500.0f;
    irLR6 = ((float)sensorValues[5]) / 2500.0f;
  }

  static float ReadLR1(){
    return irLR1;
  }

  static float ReadLR2(){
    return irLR2;
  }

  static float ReadLR3(){
    return irLR3;
  }

  static float ReadLR4(){
    return irLR4;
  }

  static float ReadLR5(){
    return irLR5;
  }

  static float ReadLR6(){
    return irLR6;
  }
};

float IRArray::irLR1 = 0.0f;
float IRArray::irLR2 = 0.0f;
float IRArray::irLR3 = 0.0f;
float IRArray::irLR4 = 0.0f;
float IRArray::irLR5 = 0.0f;
float IRArray::irLR6 = 0.0f;

QTRSensors IRArray::qtr;
uint16_t IRArray::sensorValues[SensorCount];
