#include "CompassManager.h"
#include "MotorController.h"

const int led = LED_BUILTIN;
MotorController motors;
CompassManager compass;

void Calibrate(){
  for(int i = 0; i < 100; i++){
    motors.Write(i, -i);
    compass.UpdateReadings();
    Serial.println(compass.GetHeading());
    delay(10);
  }
  
  for(int i = 0; i < 100; i++){
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

  pinMode(led, OUTPUT);
  
  Serial.println("Initializing Compass...");
  compass.Initialize();

  Calibrate();
}

void loop() {
  for(int i = 0; i < 100; i++){
    motors.Write(i, i);
    compass.UpdateReadings();
    Serial.println(compass.GetHeading());
    delay(10);
  }
  
  for(int i = 100; i > -100; i--){
    motors.Write(i, i);
    compass.UpdateReadings();
    Serial.println(compass.GetHeading());
    delay(10);
  }
  
  for(int i = -100; i < 0; i++){
    motors.Write(i, i);
    compass.UpdateReadings();
    Serial.println(compass.GetHeading());
    delay(10);
  }
}
