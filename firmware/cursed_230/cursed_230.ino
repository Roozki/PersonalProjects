


//#include "GY521.h"
#include <ros.h>
#include <Wire.h>
#include <Servo.h>
#include <std_msgs/Int16.h>

//ros stuff

ros::NodeHandle nh;

std_msgs::Int16 heart;

ros::Publisher heartbeat("heartbeat", &heart);

Servo servo1; 
int counter;
int sdrivepin = 7; //small drive m

const int imu_address = 0x68; //I2C address of imu

int16_t acc_x, acc_y, acc_z; //exactly 16 bits
int16_t gyro_x, gyro_y, gyro_z;
int16_t temperature; //hot hot hot

char tmp_str[7];

char* convert_int16_to_str(int16_t i) {
 sprintf(tmp_str, "%6d", i);
   return tmp_str;
}

void setup() {
  // put your setup code here, to run once:
 counter ++;
 nh.initNode();
 nh.advertise(heartbeat);
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(imu_address);
  Wire.write(0x6B); //PWR_MGMT_1 register
  Wire.write(0); //wakey wakey MPU-6050
  Wire.endTransmission(true);
servo1.attach(7);
  
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
}

void loop() {   

    heart.data ++;

  heartbeat.publish( &heart);
nh.spinOnce();
delay(100);

  Wire.beginTransmission(imu_address);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(imu_address, 7*2, true); // request a total of 7*2=14 registers
  

// "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
  acc_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
  acc_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
  acc_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
  temperature = Wire.read()<<8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
  gyro_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  gyro_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
  gyro_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
  
  // print out data
  Serial.print("aX = "); Serial.print(convert_int16_to_str(acc_x));
  Serial.print(" | aY = "); Serial.print(convert_int16_to_str(acc_y));
  Serial.print(" | aZ = "); Serial.print(convert_int16_to_str(acc_z));
  // the following equation was taken from the documentation [MPU-6000/MPU-6050 Register Map and Description, p.30]
  Serial.print(" | tmp = "); Serial.print(temperature/340.00+36.53);
  Serial.print(" | gX = "); Serial.print(convert_int16_to_str(gyro_x));
  Serial.print(" | gY = "); Serial.print(convert_int16_to_str(gyro_y));
  Serial.print(" | gZ = "); Serial.print(convert_int16_to_str(gyro_z));
  Serial.println();
  
  // delay
  delay(10);
servo1.write(map(acc_x, -25000, 25000, 20, 150));
  
}













//if (Serial.available() > 0){
//
//  Serial.print(char(Serial.read()));
//  
//  if(Serial.available() == 0){
//    Serial.println("");
//    Serial.println("STRING READ SUCCESSFULL");
//  }
//  
//}
//
//if(Serial2.available() > 0){
//  Serial.println(Serial2.read());
//   if(Serial2.available() == 0){
//    Serial.println("");
//    Serial.println("STRING READ SUCCESSFULL");
//  }
//}





