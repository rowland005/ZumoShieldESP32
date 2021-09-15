#include "CompassManagerMinIMU9.h"
#include "IRArray.h"
#include "MotorController.h"

const int led = 26;
MotorController motors;
CompassManager compass;

void Calibrate(){
  for(int i = 0; i < 100; i++){
    motors.Write(i, -i);
    compass.UpdateReadings();
    Serial.println(compass.GetHeading());
    delay(10);
  }
  
  for(int i = 100; i > -100; i--){
    motors.Write(i, -i);
    compass.UpdateReadings();
    Serial.println(compass.GetHeading());
    delay(10);
  }
  
  for(int i = -100; i < 0; i++){
    motors.Write(i, -i);
    compass.UpdateReadings();
    Serial.println(compass.GetHeading());
    delay(10);
  }
}

void setup() {
  Serial.begin(115200);

  Serial.println("Initializing IR Array...");
  IRArray::Initialize();
  
  Serial.println("Initializing Compass...");
  compass.Initialize();

  //Calibrate();
}

void printIR(){
  IRArray::ReadArray();
  
  float lr1 = IRArray::ReadLR1();
  float lr2 = IRArray::ReadLR2();
  float lr3 = IRArray::ReadLR3();
  float lr4 = IRArray::ReadLR4();
  float lr5 = IRArray::ReadLR5();
  float lr6 = IRArray::ReadLR6();
  
  Serial.print(lr1, 3); Serial.print("\t");
  Serial.print(lr2, 3); Serial.print("\t");
  Serial.print(lr3, 3); Serial.print("\t");
  Serial.print(lr4, 3); Serial.print("\t");
  Serial.print(lr5, 3); Serial.print("\t");
  Serial.print(lr6, 3); Serial.println("\t");
}

void loop() {
  printIR();

  /*
  for(int i = 0; i < 100; i++){
    //motors.Write(i, i);
    compass.UpdateReadings();
    Serial.println(compass.GetHeading()); Serial.print("\t");
    //printIR();
    delay(10);
  }
  
  for(int i = 100; i > -100; i--){
    //motors.Write(i, i);
    compass.UpdateReadings();
    Serial.println(compass.GetHeading()); Serial.print("\t");
    //printIR();
    delay(10);
  }
  
  for(int i = -100; i < 0; i++){
    //motors.Write(i, i);
    compass.UpdateReadings();
    Serial.println(compass.GetHeading()); Serial.print("\t");
    //printIR();
    delay(10);
  }*/
}
