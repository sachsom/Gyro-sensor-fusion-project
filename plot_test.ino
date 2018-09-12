#include "Wire.h"
//#include "SoftwareSerial.h"
//SoftwareSerial Bluetooth(10, 9);
const int address = 0x68;
int16_t acc_x, acc_y, acc_z;
int16_t gyro_x, gyro_y, gyro_z;
int16_t temperature;


void setup() {
  Serial.begin(38400);
 // Bluetooth.begin(9600);
  Wire.begin();
  Wire.beginTransmission(address);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
}
void loop() {
  Wire.beginTransmission(address);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(address, 7 * 2, true);


  acc_x = Wire.read() << 8 | Wire.read();
  acc_y = Wire.read() << 8 | Wire.read();
  acc_z = Wire.read() << 8 | Wire.read();
  temperature = Wire.read() << 8 | Wire.read();
  gyro_x = Wire.read() << 8 | Wire.read();
  gyro_y = Wire.read() << 8 | Wire.read();
  gyro_z = Wire.read() << 8 | Wire.read();

  // print out data
   Serial.print(acc_x);Serial.print(",");
   Serial.print(acc_y);Serial.print(",");
   Serial.print(acc_z);Serial.print(",");Serial.println();
  //Bluetooth.print("1");
  
// Serial.print(" | gX = "); Serial.print(gyro_x);
// Serial.print(" | gY = "); Serial.print(gyro_y);
//    Serial.print(" | gZ = "); Serial.print(gyro_z);
//  Serial.println();

  // delay
  delay(100);




  
}
