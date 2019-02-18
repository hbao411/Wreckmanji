#include <Servo.h>
#include<math.h>
const int servo1 = 8;
const int servo2 = 9;
Servo bottomServo;
Servo topServo;
const int readSwitch = 11;
const int bottom_arm = 100;
const int top_arm = 100;
typedef enum State {
  WAITING,
  MOVEMENT,
  LEFT,
  RIGHT,
  UP,
  DOWN
} states;
//these are the coordinates you want to get to
int x;
int y;
//the current x and y coordinates of the servos
int curr_x;
int curr_y;
int new_top_angle;
int new_bottom_angle;
State state;
void setup() {
  // put your setup code here, to run once:
  bottomServo.attach(servo1);
  topServo.attach(servo2);
  curr_x = 0;
  curr_y = 0;
  x = 100;
  y = 100;
  int new_top_angle = 90;
  int new_bottom_angle = 90;
  state = WAITING;
}

void loop() {
  // put your main code here, to run repeatedly:
  //eventually we'll implement the other stuff, for now only WAITING and MOVEMENt
  switch (state) {
    //calculate the angle you need to move to and set state to movement
    case WAITING:
      if (x != curr_x || y != curr_y) {
        if (x > 0) {
          new_top_angle = (int) 1.5 * PI - acos((pow(x, 2) + pow(y, 2) - pow(top_arm, 2) - pow(bottom_arm, 2))/(2 * bottom_arm*top_arm));
          new_bottom_angle = (int) (atan((float) y/x) - atan(top_arm*cos(new_top_angle)/(bottom_arm-top_arm*sin(new_top_angle)));
          
        } else {
          new_top_angle = (int) acos((pow(x, 2) + pow(y, 2) - pow(top_arm, 2) - pow(bottom_arm, 2))/(2 * bottom_arm*top_arm) - PI/2);
          new_bottom_angle = (int) (PI + atan((float) y/x) - atan(top_arm*cos(new_top_angle)/(bottom_arm-top_arm*sin(new_top_angle)));
        }
      }
      state = MOVEMENT;
      break;
    case MOVEMENT:
      bottomServo.write(new_bottom_angle);
      delay(500);
      topServo.write(new_top_angle);
      delay(500);
      curr_x = x;
      curr_y = y;
      state = WAITING;
  }
      
    
  
}
