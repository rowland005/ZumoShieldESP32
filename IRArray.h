#pragma once

class IRArray{
private:
  //UNO: 4 3, 11, 0, 2, 5
  //ESP32 D1 R32: 
  static const unsigned byte pinLR1 = 17;
  static const unsigned byte pinLR2 = 34;
  static const unsigned byte pinLR3 = 23;
  static const unsigned byte pinLR4 = 2;
  static const unsigned byte pinLR5 = 36;
  static const unsigned byte pinLR6 = 16;
  
public:
  static float readAnalog(unsigned byte pin){
    return ((float)analogRead(pin)) / 1023.0f;
  }

  static float ReadLR1(){
    readAnalog(pinLR1);
  }

  static float ReadLR2(){
    readAnalog(pinLR2);
  }

  static float ReadLR3(){
    readAnalog(pinLR3);
  }

  static float ReadLR4(){
    readAnalog(pinLR4);
  }

  static float ReadLR5(){
    readAnalog(pinLR5);
  }

  static float ReadLR6(){
    readAnalog(pinLR6);
  }
};
