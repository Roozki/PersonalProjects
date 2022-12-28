#include <Servo.h>
#define onePin 9
#define twoPin 5


#define throttlePin 2
#define aileronPin 3
#define elevatorPin 4
Servo one;
Servo two;

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


int ir[4];
void setup() {
   pinMode(onePin, OUTPUT);
  pinMode(twoPin, OUTPUT);
 one.attach(onePin);
  two.attach(twoPin);

  // put your setup code here, to run once:
Serial.begin(9600);
// while(Serial.available() == 0){
//   Serial.println("OFFLINE");
  
// }

Serial.println(". ");
Serial.println("..");
delay(1000);
Serial.println("...initiation succsefull. Preliminary sensor check:");
sensorCheck();
diagnostics();

 pinMode(throttlePin, INPUT_PULLUP);
  pinMode(aileronPin, INPUT_PULLUP);
   //pinMode(elevatorPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(throttlePin), throttleCHECK, CHANGE);
  attachInterrupt(digitalPinToInterrupt(aileronPin), aileronCHECK, CHANGE);
    //attachInterrupt(digitalPinToInterrupt(elevatorPin), elevatorCHECK, CHANGE);

 

}

void loop() {
  // put your main code here, to run repeatedly:
  sensorCheck();
  //diagnostics();
  //PrintRCdata();


  if (throttletmp < 2000) {
    throttle = throttletmp;
  }
  if (ailerontmp < 2000) {
    aileron = ailerontmp;
  }
 if (elevatortmp < 2000){
    elevator = elevatortmp;
  }



//rc control
int stoobilizer = map(aileron, 1100, 1900, -400, 400);
int onepow = throttle - stoobilizer;
int twopow = throttle + stoobilizer;
onepow = map(onepow, 1100, 1900, 1200, 1800);
twopow = map(twopow, 1100, 1900, 1800, 1200);

one.writeMicroseconds(onepow);
two.writeMicroseconds(twopow);






}

void sensorCheck(){
  for(int i = 0; i < 5; i++){
    ir[i] = analogRead(i);
  }
}

void diagnostics(){
  for(int i = 0; i < 5; i++){
    Serial.print("   IR");
    Serial.print(i);
    Serial.print("  =  ");
    Serial.print(ir[i]);

  }
      Serial.println("   ");

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


void PrintRCdata() {
  Serial.print("throttle:         ");
  Serial.print(throttle);
  Serial.print("     aileron:   ");
  Serial.print(aileron);
  Serial.print("     elevator:  ");
  Serial.println(elevator);
}