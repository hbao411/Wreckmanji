#include <Servo.h>
const int servo1 = 8;
const int servo2 = 9;
Servo bottomServo;
Servo topServo;
const int readSwitch = 11;
typedef enum State {
  WAITING,
  LEFT,
  RIGHT, 
  UP,
  DOWN
} states;
int x;
int y;
State state;
void setup() {
  // put your setup code here, to run once:
  bottomServo.attach(servo1);
  topServo.attach(servo2);
  x = 0;
  y = 100;
  state = WAITING;
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
