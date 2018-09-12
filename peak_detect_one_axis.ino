#include "Wire.h"
const int address = 0x68;
int16_t acc_x, acc_y, acc_z,gyro_x, gyro_y, gyro_z,temperature;
int avg[5];
float arr_mean[5];
int count,index;
float mean,mean_mean,sd,avg_final;
int peak=-1;
int add_count;
int sig;

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

int i=0;
for(i=4;i>=1;i--)
{
avg[i]=avg[i-1];
arr_mean[i]=arr_mean[i-1];
sig=0;
}
avg[0]=gyro_x;
arr_mean[0]=mean;
count++;
  
if(count<5)
{
  goto finish_it;
}
    
  if((abs(avg[0])-abs(mean))>((4*sd)))
  {
    peak++;
    sig=1;
    Serial.print(">>>>>>>>>>>>>>>>>>===============<<<<<<<<<<<<<<<<");Serial.println();
    Serial.print("abs(avg[0])=");Serial.println(abs(avg[0]));
    Serial.print("(abs(avg[0])-abs(mean)=");Serial.println((abs(avg[0])-abs(mean)));
    Serial.print("sd= ");Serial.println(sd);
    Serial.print("3.5*sd=");Serial.println(3.5*sd);
    Serial.print("peak= ");Serial.println(peak);
    Serial.print(">>>>>>>>>>>>>>>>>>===============<<<<<<<<<<<<<<<<");Serial.println();
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
  sd=(square(avg[0]-mean)+square(avg[1]-mean)+square(avg[2]-mean)+square(avg[3]-mean)+square(avg[4]-mean));
  sd=sqrt(sd)/2;
  mean=((avg[0]+avg[1]+avg[2]+avg[3]+avg[4])/5);
  mean_mean=((arr_mean[0]+arr_mean[1]+arr_mean[2]+arr_mean[3]+arr_mean[4])/5);
  delay(250);
}


