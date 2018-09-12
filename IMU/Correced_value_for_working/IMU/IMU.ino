#include<Wire.h>
const int MPU_addr = 0x68;// I2C address of the MPU-6050
int16_t RxAcc, RyAcc, RzAcc, Tmp, GyX, GyY, GyZ;
float cACCX, cACCY, cACCZ, cGYX, cGYY, cGYZ, Racc, Racc_norm, Racc_abs, R;
float RxEst, RyEst, RzEst, Rest;
float RwAcc[3];  
float gyro_ds[3];  
float RwGyro[3];        
float Awz[2];          
float RwEst[3];
boolean firstSample = true;
int lastTime = 0;
int interval = 0;
float wGyro = 10.0;
//float Rest = [RxEst,RyEst,RzEst];
void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the the mpu-6050)
  Wire.endTransmission(true);
  Serial.begin(38400);
}
void loop() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)>>> reffer to the documentatiion for registers part 
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers
  RxAcc = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  RyAcc = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  RzAcc = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  cACCX = (RxAcc / 1600);
  cACCY = (RyAcc / 1600); /// sensitvity need not be accurate check it from documention part page 13 
  cACCZ = (RzAcc / 1600);
  cGYX = (GyX + 418) / 131;
  cGYY = (GyY + 185) / 16;
  cGYZ = (GyZ + 120) / 16;
  RwAcc[0] = cACCX;
  RwAcc[1] = cACCY;
  RwAcc[2] = cACCZ;
  gyro_ds[0] = cGYX;
  gyro_ds[1] = cGYX;
  gyro_ds[2] = cGYX;
  Racc_abs = abs(Racc);
  Racc_norm = sqrt((cACCX * cACCX) / (Racc_abs * Racc_abs) + (cACCY * cACCY) / (Racc_abs * Racc_abs) + (cACCZ * cACCZ) / (Racc_abs * Racc_abs));
  R = Racc_norm;
  Racc = sqrt((cACCX * cACCX) + (cACCY * cACCY) + (cACCZ * cACCZ));
  // Serial.print("RxAcc = "); Serial.print(cACCX);
  // Serial.print(" | RyAcc = "); Serial.print(cACCY);
  // Serial.print(" | RzAcc = "); Serial.print(cACCZ);
  
  //   Serial.print(" | GyX = "); Serial.print(cGYX);
  //   Serial.print(" | GyY = "); Serial.print(cGYY);
  //   Serial.print(" | GyZ = "); Serial.println(cGYZ);
  // Serial.print("RxAcc = "); Serial.print(RxAcc);
  // Serial.print(" | RyAcc = "); Serial.print(RyAcc);
  // Serial.print(" | RzAcc = "); Serial.print(RzAcc);// used for debugging purpose
  //   Serial.print(" | GyX = "); Serial.print(GyX);
  //   Serial.print(" | GyY = "); Serial.print(GyY);
  //   Serial.print(" | GyZ = "); Serial.println(GyZ);
  // Serial.print(" | Racc = "); Serial.println(Racc);
  // Serial.print(" | Racc normalised = "); Serial.println(Racc_norm);
  getInclnination();
//  Serial.print(" RwEst[0] = "); Serial.print(RwEst[0]);
//  Serial.print(" | RwEst[1] = "); Serial.print(RwEst[1]);
//  Serial.print(" | RwEst[2] = "); Serial.println(RwEst[2]);
    serialPrintFloatArr(RwAcc, 3);
    serialPrintFloatArr(gyro_ds, 3);
    serialPrintFloatArr(RwGyro, 3);
    serialPrintFloatArr(Awz, 2);
    serialPrintFloatArr(RwEst, 3);
    Serial.println();
  delay(333);
}


float squared(float x)
{
  return x * x;
}

void getInclnination()
{
  int w = 0;
  float tmpf = 0.0;
  int currentTime, signRzGyro;

  currentTime = millis();
  interval = currentTime - lastTime;
  lastTime = currentTime;

  if (firstSample)
  {
    for (w = 0; w <= 2; w++)
    {
      RwEst[w] = RwAcc[w];
    }
  }
  else
  {
    if (abs(RwEst[2]) < 0.1)
    {
      RwGyro[w] = RwEst[w];

    }
    else {
      for (w = 0; w <= 1; w++)
      {
        tmpf = gyro_ds[w];
        tmpf = tmpf * (interval / 1000.0f);
        Awz[w] = atan2(RwEst[w], RwEst[2]) * 180 / PI;
        Awz[w] = Awz[w] + tmpf;

      }
      signRzGyro = (cos(Awz[0] * PI / 180) >= 0) ? 1 : -1;
    }
    for (w = 0; w <= 1; w++) {
      RwGyro[0] = sin(Awz[0] * PI / 180);
      RwGyro[0] /= sqrt( 1 + squared(cos(Awz[0] * PI / 180)) * squared(tan(Awz[1] * PI / 180)) );
      RwGyro[1] = sin(Awz[1] * PI / 180);
      RwGyro[1] /= sqrt( 1 + squared(cos(Awz[1] * PI / 180)) * squared(tan(Awz[0] * PI / 180)) );
    }
    RwGyro[2] = signRzGyro * sqrt(1 - squared(RwGyro[0]) - squared(RwGyro[1]));
    for (w = 0; w <= 2; w++)
    {
      RwEst[w] = (RwAcc[w] + wGyro * RwGyro[w]) / (1 + wGyro);
    }
  } firstSample = false;
}

void serialPrintFloatArr(float * arr, int length) {
  for(int i=0; i<length; i++) {
    serialFloatPrint(arr[i]);
    Serial.print(",");
  }
}

///data send to processing for visualization
void serialFloatPrint(float f) {
  byte * b = (byte *) &f;
  Serial.print("f:");
  for(int i=0; i<4; i++) {
    byte b1 = (b[i] >> 4) & 0x0f;
    byte b2 = (b[i] & 0x0f);
    char c1 = (b1 < 10) ? ('0' + b1) : 'A' + b1 - 10;
    char c2 = (b2 < 10) ? ('0' + b2) : 'A' + b2 - 10;
    Serial.print(c1);
    Serial.print(c2);
  }
}


