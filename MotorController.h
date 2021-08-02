class MotorController{
private:
  const int freq = 5000;
  const int resolution = 10;
  const int channelL = 0;
  const int channelR = 1;
  const int maxDutyCycle = (int)(pow(2, resolution) - 1);
  const int M1DIR = 14;//14 7
  const int M1PWM = 13;//13 9
  const int M2DIR = 12;//12 8
  const int M2PWM = 5;//5 10

  float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }
  
public:
  MotorController(){
    pinMode(M1DIR, OUTPUT);
    pinMode(M2DIR, OUTPUT);
    
    ledcSetup(channelL, freq, resolution);
    ledcSetup(channelR, freq, resolution);
    
    ledcAttachPin(M1PWM, channelL);
    ledcAttachPin(M2PWM, channelR);
  }
  
  void Write(float leftMotor, float rightMotor){
    if(leftMotor  < 0){ digitalWrite(M1DIR, HIGH); }
    else{ digitalWrite(M1DIR, LOW); }
    if(rightMotor < 0){ digitalWrite(M2DIR, HIGH); }
    else{ digitalWrite(M2DIR, LOW); }
    
    int leftWrite = constrain(mapf(abs(leftMotor), 0.0f, 100.0f, 0.0f, 1023.0f), 0, maxDutyCycle);
    int rightWrite = constrain(mapf(abs(rightMotor), 0.0f, 100.0f, 0.0f, 1023.0f), 0, maxDutyCycle);
    
    ledcWrite(channelL, leftWrite);
    ledcWrite(channelR, rightWrite);
  }
};
