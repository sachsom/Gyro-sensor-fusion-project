#include "Wire.h"
const int address = 0x68;
int16_t acc_x, acc_y, acc_z,gyro_x, gyro_y, gyro_z,temperature;
float avg[20];
float arr_mean[20];
int count,index;
float mean,mean_mean,sd,avg_final;
int peak=-1;
int add_count,peak_sec,prev_peak;
int sig;
unsigned long t1,t2=0; 

void setup()
{
  Serial.begin(38400);
  Wire.begin();
  Wire.beginTransmission(address);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  
}

void loop()
{
 
finish_it:
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
t1=millis();
int i=0;
for(i=19;i>=1;i--)
{
avg[i]=avg[i-1];
arr_mean[i]=arr_mean[i-1];
sig=0;
}
avg[0]=gyro_x;
arr_mean[0]=mean;
count++;
  
if(count<20)
{
  goto finish_it;
}
    
  if((abs(avg[0])-abs(mean))>((3.5*sd)))
  {
    peak++;
    sig=100;
    
    if (peak%2==0)
    
//    Serial.print(">>>>>>>>>>>>>>>>>>===============<<<<<<<<<<<<<<<<");Serial.println();
//    Serial.print("abs(avg[0])=");Serial.println(abs(avg[0]));
//    Serial.print("(abs(avg[0])-abs(mean)=");Serial.println((abs(avg[0])-abs(mean)));
//    Serial.print("sd= ");Serial.println(sd);
//    Serial.print("3.5*sd=");Serial.println(3.5*sd);
//    Serial.print("peak= ");Serial.println(peak);
//    Serial.print(">>>>>>>>>>>>>>>>>>===============<<<<<<<<<<<<<<<<");Serial.println();
      Serial.print("Signal = "); Serial.print(sig);
      Serial.print(" acc_x = "); Serial.print(gyro_x);
      Serial.print(" mean = "); Serial.println(mean);
     // Serial.print(" time =");Serial.println(t1);
//      Serial.print(" time = "); Serial.println(t1-t2);
    add_count++;
    if(add_count%2==0)
    { 
    
    avg[0]=mean_mean+150;
    delay(50);
  }
  else
  {
    
    avg[0]=mean_mean-150;
   }
  }
  sig=0;
 Serial.print("Signal = "); Serial.print(sig);
      Serial.print(" acc_x = "); Serial.print(gyro_x);
      Serial.print(" mean = "); Serial.println(mean);
  sd = (square(avg[0] - mean) + square(avg[1] - mean) + square(avg[2] - mean) + square(avg[3] - mean) + square(avg[4] - mean) + square(avg[5] - mean) + square(avg[6] - mean) + square(avg[7] - mean) + square(avg[8] - mean) + square(avg[9] - mean) + square(avg[10] - mean) + square(avg[11] - mean) + square(avg[12] - mean) + square(avg[13] - mean) + square(avg[14] - mean) + square(avg[15] - mean) + square(avg[16] - mean) + square(avg[17] - mean) + square(avg[18] - mean) + square(avg[19] - mean));
sd = sqrt((sd) / 20);
mean = ((avg[0] + avg[1] + avg[2] + avg[3] + avg[4] + avg[5] + avg[6] + avg[7] + avg[8] + avg[9] + avg[10] + avg[11] + avg[12] + avg[13] + avg[14] + avg[15] + avg[16] + avg[17] + avg[18] + avg[19]) / 20);
  
 
  mean_mean=((arr_mean[0]+arr_mean[1]+arr_mean[2]+arr_mean[3]+arr_mean[4])/5);
  //Serial.print(" time =");Serial.println(t1);
  delay(150);
  
if((unsigned long)(t1-t2)>1000)
{
  t2=t1;
 peak_sec=peak-prev_peak;
 prev_peak=peak;
 //Serial.print("No of peaks per second is =");Serial.println(peak_sec); 
}
}




