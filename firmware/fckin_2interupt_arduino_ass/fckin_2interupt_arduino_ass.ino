#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Servo.h>

#include <AccelStepper.h>


#define throttlePin 2 // eliminating
#define aileronPin 3
#define elevatorPin 4

#define prop1pin 6
#define prop2pin 9

Adafruit_MPU6050 mpu;
sensors_event_t a, g, temp;

Servo prop1;  //cw
Servo prop2;  //ccw

int counter;

//int motor[] = {0, 1};

AccelStepper wheel(1, 5,7);
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

// float acc_x;
// float acc_y;
// float acc_z;
// float gyro_x;
// float gyro_y;
// float gyro_z;
// int temperature;

int mot1 = 79;
int mot2 = 89;
int motpwm = 59;


int elevatorSlowdownCount = 0;


void setup() {
  wheel.setMinPulseWidth(20);
wheel.setAcceleration(1000);
wheel.setMaxSpeed(2000);



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

  prop1.attach(prop1pin);
  prop2.attach(prop2pin);
}


void loop() {

  //diagnostics
  //PrintACCdata();
  //PrintThrottle();
  //PrintRCdata();//////////////////////////////////////////////////////////////////;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

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
 wheel.run();





  mpu.getEvent(&a, &g, &temp);






  if (throttletmp < 2000) {
    throttle = throttletmp;
  }
  if (ailerontmp < 2000) {
    aileron = ailerontmp;
  }

  // if (elevatortmp < 2000){
  //   elevator = elevatortmp;
  // }
 wheel.run();

  // if (elevatorSlowdownCount > 15) {
  //   elevator = pulseIn(elevatorPin, HIGH);
  //   elevatorSlowdownCount = 0;
  // }
 wheel.run();


  //analogWrit



  //manual lift
  //try with servo
  //prop1.write(throttle1);
  //prop2.write(throttle2);
  //analogwrite got weird calibration
  //do pwm with stuff that makes sense

  
   



  //input maps
  
  aileronAdj = map(aileron, 1100, 2000, 30, -30);
  throttle1 = map(throttle, 1100, 2000, 0, 900);
  throttle2 = map(throttle, 1100, 2000, 0, 900);
  //adjustment
  throttle1 -= (aileronAdj);
   throttle2 += aileronAdj;

  //stabilization code and power to props

  //lat roll 
  int rollConstantR = 60;
  int rollConstantL = 40;

  if(throttle > 1250){
  if (a.acceleration.y > 0.5){
   throttle1 = throttle1 + rollConstantR*(0 + a.acceleration.y);
   throttle2 = throttle2 + rollConstantR*(0 - a.acceleration.y);

  }else if (a.acceleration.y < -0.5){
   throttle1 = throttle1 + rollConstantL*(0 + a.acceleration.y);
   throttle2 = throttle2 + rollConstantL*(0 - a.acceleration.y);

  }
  }
  //forward/backward
  // if (a.acceleration.z > 0.1){
  //  throttle1 = throttle1*(1 + a.acceleration.z);
  //  throttle2 = throttle2*(1 - gyro_x);

  // }else if (a.acceleration.z < -0.1){
  //  throttle1 = throttle1*(1 + gyro_x);
  //  throttle2 = throttle2*(1 - gyro_x);

  // }
//final adjustments

throttle1 = map(throttle1, 0, 900, 1500, 2000);
throttle2 = map(throttle2, 0, 900, 1500, 1900);


  prop1.writeMicroseconds(throttle1);
  prop2.writeMicroseconds(throttle2);


wheel.run();

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
 wheel.setCurrentPosition(10000);
wheel.moveTo(20000);
wheel.run();


  // digitalWrite(mot1, LOW);
  // digitalWrite(mot2, HIGH);
  //analogWrite(motpwm, 100);
}

void backward() {
   wheel.setCurrentPosition(10000);
  wheel.moveTo(0);
wheel.runToPosition();

  // digitalWrite(mot1, HIGH);
  // digitalWrite(mot2, LOW);
  //analogWrite(motpwm, 200);
}

void stop() {
    wheel.moveTo(10000);

  wheel.setCurrentPosition(10000);
  wheel.run();

  //digitalWrite(mot1, LOW);
  //digitalWrite(mot2, LOW);
  //analogWrite(motpwm, 0);
}






void PrintACCdata() {
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
  Serial.print("Temp:");
  Serial.print(temp.current);
  Serial.println("");
}





void PrintThrottle(){
  Serial.print(" input:  ");
  Serial.print(throttle);
   Serial.print("   Throttle 1:  ");
  Serial.print(throttle1);
  Serial.print("   Throttle 2:  ");
  Serial.println(throttle2);

}


void PrintRCdata() {
  Serial.print("throttle:         ");
  Serial.print(throttle);
  Serial.print("     aileron:   ");
  Serial.print(aileron);
  Serial.print("     elevator:  ");
  Serial.println(elevator);
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