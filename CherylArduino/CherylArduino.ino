#include <AccelStepper.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

#define NUM_MOTORS 2

byte speeds[NUM_MOTORS];
int index;
int ledPin = 13;
bool test;

//Setup Stacked Shields
Adafruit_MotorShield AFMSbot(0x61); // Rightmost jumper closed
Adafruit_MotorShield AFMStop(0x60); // Default address, no jumpers

// On the top shield, connect a steppers to port M3/M4 with 200 steps 
Adafruit_StepperMotor *myStepper2 = AFMStop.getStepper(200, 2);

// On the bottom shield connect a stepper to port M3/M4 with 200 steps
Adafruit_StepperMotor *myStepper1 = AFMSbot.getStepper(200, 2);

// you can change these to DOUBLE or INTERLEAVE or MICROSTEP!
void forwardstep1() {  
  myStepper1->onestep(FORWARD, MICROSTEP);
}
void backwardstep1() {  
  myStepper1->onestep(BACKWARD, MICROSTEP);
}

// you can change these to DOUBLE or INTERLEAVE or MICROSTEP!
void forwardstep2() {  
  myStepper2->onestep(FORWARD, MICROSTEP);
}
void backwardstep2() {  
  myStepper2->onestep(BACKWARD, MICROSTEP);
}

AccelStepper Astepper1(forwardstep1, backwardstep1); // use functions to step
AccelStepper Astepper2(forwardstep2, backwardstep2); // use functions to step

AccelStepper* steppers[NUM_MOTORS];


void setup()
{  
   Serial.begin(9600);           // set up Serial library at 9600 bps
   Serial.println("Stepper test!");
  
  AFMSbot.begin();  // create with the default frequency 1.6KHz
  AFMStop.begin();
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  steppers[0] = &Astepper1;
  steppers[1] = &Astepper2;
  for(int i = 0; i < NUM_MOTORS; i++) {
    speeds[i] = 0;
  }
  index = 0;
  Astepper1.setSpeed(speeds[0]);
  pinMode(ledPin, OUTPUT);
  test = true;
}

void loop()
{
  while(Serial.available()) {
    speeds[index] = Serial.read();
    index++;
  }
  if(index == NUM_MOTORS) {
    test = !test;
    index = 0;
    for(int i = 0; i < NUM_MOTORS; i++) {
      float val = map(speeds[i], 0, 256, -500, 500);
      if (abs(val) < 0.01) val = 0;
      steppers[i]->setSpeed(val);
    }

  }

  if(test) {
      digitalWrite(ledPin, HIGH);
  } else {
      digitalWrite(ledPin, LOW);
  }

  for(int i = 0; i < NUM_MOTORS; i++) {
    steppers[i]->runSpeed();
  }
}

int lerp(int a, int b, float rate) {
  return a + rate * (b - a);
}

