#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Servo.h>

#include <AccelStepper.h>


#define throttlePin 3
#define aileronPin 2
#define elevatorPin 4

#define prop1pin 6
#define prop2pin 9

Adafruit_MPU6050 mpu;

Servo prop1;  //cw
Servo prop2;  //ccw

int counter;

//int motor[] = {0, 1};

//AccelStepper wheel(1, 8,9);
//int motorDirPin[] = {3,4};
//int motorPulPin[] = {8,9};






volatile long throttleprev = 0;
volatile long throttlecurrent = 0;
volatile long throttlepulses = 0;
volatile long throttletmp = 0;
int throttle = 0;


volatile long aileronprev = 0;
volatile long aileroncurrent = 0;
volatile long aileronpulses = 0;
volatile long ailerontmp = 0;
int aileron = 0;

volatile long elevatorprev = 0;
volatile long elevatorcurrent = 0;
volatile long elevatorpulses = 0;
volatile long elevatortmp = 0;
int elevator = 0;


//control
int throttle1 = 0;
int throttle2 = 0;

//int throttle1min = 90;
//int throttle2min = 90;

int aileronAdj = 0;

const int imu_address = 0x68;  //I2C address of imu
//accelerometer

float acc_x;
float acc_y;
float acc_z;
float gyro_x;
float gyro_y;
float gyro_z;
int temperature;

int mot1 = 7;
int mot2 = 8;
int motpwm = 5;


int elevatorSlowdownCount = 0;


void setup() {

  Serial.begin(115200);
  while (!Serial) {
    delay(10);  // will pause Zero, Leonardo, etc until serial console opens
  }

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  pinMode(mot1, OUTPUT);
  pinMode(mot2, OUTPUT);
  pinMode(motpwm, OUTPUT);



  pinMode(throttlePin, INPUT_PULLUP);
  pinMode(aileronPin, INPUT_PULLUP);
  pinMode(elevatorPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(throttlePin), throttleCHECK, CHANGE);
  attachInterrupt(digitalPinToInterrupt(aileronPin), aileronCHECK, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(elevatorPin), elevatorCHECK, CHANGE);

  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.println("mpu initiation appears successfull");
  delay(100);

  // Wire.begin();
  //   Wire.beginTransmission(imu_address);
  //   Wire.write(0x6B); //PWR_MGMT_1 register
  //   Wire.write(0); //wakey wakey MPU-6050
  //   Wire.endTransmission(true);
  //  prop1.attach(prop1pin);
  //  prop2.attach(prop2pin);
  prop1.attach(prop1pin);
  prop2.attach(prop2pin);
}


void loop() {
  elevatorSlowdownCount++;

  //wheel control
  if (elevator > 1600) {
    forward();

    analogWrite(motpwm, map(elevator, 1600, 1900, 0, 255));
  } else if (elevator < 1400) {
    backward();
    analogWrite(motpwm, map(elevator, 1400, 1100, 0, 255));

  } else {
    stop();
  }





  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
/*
 Serial.print("AccelX:");
  Serial.print(a.acceleration.x);
  Serial.print(",");
  Serial.print("AccelY:");
  Serial.print(a.acceleration.y);
  Serial.print(",");
  Serial.print("AccelZ:");
  Serial.print(a.acceleration.z);
  Serial.print(", ");
  Serial.print("GyroX:");
  Serial.print(g.gyro.x);
  Serial.print(",");
  Serial.print("GyroY:");
  Serial.print(g.gyro.y);
  Serial.print(",");
  Serial.print("GyroZ:");
  Serial.print(g.gyro.z);
  Serial.println("");

*/


  if (throttletmp < 2000) {
    throttle = throttletmp;
  }
  if (ailerontmp < 2000) {
    aileron = ailerontmp;
  }

  // if (elevatortmp < 2000){
  //   elevator = elevatortmp;
  // }

  if (elevatorSlowdownCount > 20) {
    elevator = pulseIn(elevatorPin, HIGH);
    elevatorSlowdownCount = 0;
  }


  // Serial.print("throttle:         ");
  // Serial.print(throttle);
  // Serial.print("     aileron:   ");
  // Serial.print(aileron);
  // Serial.print("     elevator:  ");
  // Serial.println(elevator);


  throttle1 =  map(throttle, 1100, 2000, 0, 900);
  throttle2 = map(throttle, 1100, 2000, 0, 900);;
//   throttle1 = map(throttle1, 1200, 1900, 95, 103);
// throttle2 = map(throttle2, 1200, 1900, 95, 101);

aileronAdj = map(aileron, 1100, 2000, 70, -70);



throttle1 = map(throttle1, 0, 900, 1500, 2000);
throttle2 = map(throttle2, 0, 900, 1500, 2000);

throttle1 += (aileronAdj + 20);

throttle2 -= aileronAdj;
  //analogWrit

// Serial.print(throttle1);
// Serial.print("   ");
// Serial.println(throttle2);
  //manual lift
  //try with servo
  //prop1.write(throttle1); 
  //prop2.write(throttle2);
  //analogwrite got weird calibration
  //do pwm with stuff that makes sense
  prop1.writeMicroseconds(throttle1);
  prop2.writeMicroseconds(throttle2);









  Wire.beginTransmission(imu_address);
  Wire.write(0x3B);                            // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false);                 // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(imu_address, 7 * 2, true);  // request a total of 7*2=14 registers


  //"Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
  acc_x = Wire.read() << 8 | Wire.read();        // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
  acc_y = Wire.read() << 8 | Wire.read();        // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
  acc_z = Wire.read() << 8 | Wire.read();        // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
  temperature = Wire.read() << 8 | Wire.read();  // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
  gyro_x = Wire.read() << 8 | Wire.read();       // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  gyro_y = Wire.read() << 8 | Wire.read();       // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
  gyro_z = Wire.read() << 8 | Wire.read();       // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)

  //print out data
  // Serial.print("aX = "); Serial.print(String(acc_x));

  // Serial.print(" | aY = "); Serial.print(String(acc_y));
  // Serial.print(" | aZ = "); Serial.print(String(acc_z));
  // // the following equation was taken from the documentation [MPU-6000/MPU-6050 Register Map and Description, p.30]
  // //Serial.print(" | tmp = "); Serial.print(temperature/340.00+36.53);
  // Serial.print(" | gX = "); Serial.print(String(gyro_x));
  // Serial.print(" | gY = "); Serial.print(String(gyro_y));
  // Serial.print(" | gZ = "); Serial.print(String(gyro_z));
  //  Serial.println();
  //Serial.println(IR1.data);





  //servo1.write(map(acc_x, -25000, 25000, 20, 150));
}

void throttleCHECK() {
  throttlecurrent = micros();
  if (throttlecurrent > throttleprev) {
    throttletmp = throttlecurrent - throttleprev;
    throttleprev = throttlecurrent;
  }
}

void aileronCHECK() {

  aileroncurrent = micros();
  if (aileroncurrent > aileronprev) {
    ailerontmp = aileroncurrent - aileronprev;
    aileronprev = aileroncurrent;
  }
}

void elevatorCHECK() {

  elevatorcurrent = micros();
  if (elevatorcurrent > elevatorprev) {
    elevatortmp = elevatorcurrent - elevatorprev;
    elevatorprev = elevatorcurrent;
  }
}

void forward() {
  digitalWrite(mot1, LOW);
  digitalWrite(mot2, HIGH);
  //analogWrite(motpwm, 100);
}

void backward() {
  digitalWrite(mot1, HIGH);
  digitalWrite(mot2, LOW);
  //analogWrite(motpwm, 200);
}

void stop() {
  digitalWrite(mot1, LOW);
  digitalWrite(mot2, LOW);
  //analogWrite(motpwm, 0);
}






















//if (Serial.available() > 0){
//
//  Serial.print(char(Serial.read()));
//
//  if(Serial.available() == 0){
//    Serial.println("");
//    Serial.println("String READ SUCCESSFULL");
//  }
//
//}
//
//if(Serial2.available() > 0){
//  Serial.println(Serial2.read());
//   if(Serial2.available() == 0){
//    Serial.println("");
//    Serial.println("String READ SUCCESSFULL");
//  }
//}